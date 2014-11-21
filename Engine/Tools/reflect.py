import glob
import os
import sys
import re
import xml.etree.ElementTree as ET
import shutil
import time
import fnmatch
from subprocess import call

input_dir = sys.argv[1]
output_dir = sys.argv[2]
types_namespace = sys.argv[3]

doxygen_config = {
    "DOXYFILE_ENCODING" : "UTF-8",
    "FILE_PATTERNS" : "*.h",
    "GENERATE_BUGLIST" : "NO",
    "GENERATE_DEPRECATEDLIST" : "NO",
    "GENERATE_HTML" : "NO",
    "GENERATE_LATEX" : "NO",
    "GENERATE_XML" : "YES",
    "INPUT" : input_dir,
    "INPUT_ENCODING" : "UTF-8",
    "OUTPUT_DIRECTORY" : output_dir,
    "OUTPUT_LANGUAGE" : "English",
    "PROJECT_NAME" : "Hect Reflect",
    "QUIET" : "YES",
    "RECURSIVE" : "YES",
    "WARN_IF_DOC_ERROR" : "NO",
    "WARN_IF_UNDOCUMENTED" : "NO",
    "WARN_NO_PARAMDOC" : "NO",
    "WARNINGS" : "NO",
    "XML_OUTPUT" : "xml",
    "XML_PROGRAMLISTING" : "NO"
}

namespaces = []

class Property:
    def __init__(self, name):
        self.name = name
        self.is_required = False
        self.is_vector = False

class Type:
    def __init__(self, header, id, name):
        self.header = header
        self.id = id
        self.name = name
        self.is_template = False
        self.properties = []
        
class ClassType(Type):
    def __init__(self, header, id, name):
        Type.__init__(self, header, id, name)
        self.is_component = False
        self.is_system = False
        self.is_gamemode = False
        
class EnumType(Type):
    def __init__(self, header, id, name):
        Type.__init__(self, header, id, name)
        self.values = []

def process_xml(root):
    types = []
    for compounddef in root.iter("compounddef"):
        if "kind" in compounddef.attrib and compounddef.attrib["kind"] == "namespace":
            namespace = compounddef.find("compoundname").text
            for memberdef in compounddef.iter("memberdef"):
                kind = memberdef.attrib["kind"]
                if kind == "enum":
                    id = memberdef.attrib["id"]
                    name = memberdef.find("name").text
                    header = memberdef.find("location")
                    if not header is None:
                        header = header.attrib["file"]
                    enum_type = EnumType(header, id, name)
                    for enumvalue in memberdef.iter("enumvalue"):
                        enum_type.values.append(enumvalue.find("name").text)                            
                    types.append(enum_type)
        elif "kind" in compounddef.attrib and compounddef.attrib["kind"] == "class":
            if compounddef.attrib["prot"] == "public":
                id = compounddef.attrib["id"]
                name = compounddef.find("compoundname").text
                header = compounddef.find("location")
                if not header is None:
                    header = header.attrib["file"]

                class_type = ClassType(header, id, name)

                descriptions = []
                detaileddescription = compounddef.find("detaileddescription")
                if not detaileddescription is None:
                    descriptions.append(detaileddescription)
                briefdescription = compounddef.find("briefdescription")
                if not briefdescription is None:
                    descriptions.append(briefdescription)
                for description in descriptions:
                    for para in description.findall("para"):
                        if not para.text is None:
                            if "[component]" in para.text:
                                class_type.is_component = True
                            elif "[system]" in para.text:
                                class_type.is_system = True
                            elif "[gamemode]" in para.text:
                                class_type.is_gamemode = True

                templateparamlist = compounddef.find("templateparamlist")
                if not templateparamlist is None:
                    class_type.is_template = True;

                for sectiondef in compounddef.iter("sectiondef"):
                    for memberdef in sectiondef.iter("memberdef"):
                        name = memberdef.find("name").text
                        type = memberdef.find("type")
                        descriptions = []
                        detaileddescription = memberdef.find("detaileddescription")
                        if not detaileddescription is None:
                            descriptions.append(detaileddescription)
                        briefdescription = memberdef.find("briefdescription")
                        if not briefdescription is None:
                            descriptions.append(briefdescription)

                        for description in descriptions:
                            for para in description.findall("para"):
                                if not para.text is None:
                                    if "[property]" in para.text:
                                        property = Property(name)
                                        if "required" in para.text:
                                            property.is_required = True
                                        property.is_vector = not type is None and not type.text is None and type.text.startswith("std::vector")
                                        class_type.properties.append(property) 

                types.append(class_type)

    return types

def sanitize_name(name):
    sanitized = name.split("::", 1)
    if len(sanitized) == 2:
        return sanitized[1]
    else:
        return name
        
if __name__ == "__main__":
    types = []
    
    print("Hect Reflect - " + str(sys.argv[1:]))
    
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    
    if not os.path.exists(os.path.join(output_dir, "xml")):
        os.makedirs(os.path.join(output_dir, "xml"))
    
    header_file_name = os.path.join(output_dir, "RegisterTypes.h")  
    
    last_modified = 0
    for root, dirnames, filenames in os.walk(input_dir):
        for filename in fnmatch.filter(filenames, "*.h"):
            t = os.path.getmtime(os.path.join(root, filename))
            if t > last_modified:
                last_modified = t
    
    last_generated = 0
    if os.path.isfile(header_file_name):
        last_generated = os.path.getmtime(header_file_name)
    
    if last_generated >= last_modified:
        print("Reflect code is up to date...")
        exit()
        
    print("Generating Doxygen XML...")
    with open("Doxyfile", "w") as doxyfile:
        for key, value in doxygen_config.items():
            doxyfile.write(key + " = " + value + "\n")
        doxyfile.write("ALIASES += component=\"[component]\"\n")
        doxyfile.write("ALIASES += system=\"[system]\"\n")
        doxyfile.write("ALIASES += gamemode=\"[gamemode]\"\n")
        doxyfile.write("ALIASES += property=\"[property]\"\n")
        doxyfile.write("ALIASES += property{1}=\"[property]\\1\"\n")
        doxyfile.write("ALIASES += property{2}=\"[property]\\1 \2\"\n")
        doxyfile.write("ALIASES += property{3}=\"[property]\\1 \2 \3\"\n")
        doxyfile.write("ALIASES += property{4}=\"[property]\\1 \2 \3 \4\"\n")
    call("doxygen", shell=True)
    os.remove("Doxyfile")
    
    print("Parsing Doxygen XML...")
    for file_name in glob.glob(os.path.join(output_dir, "xml", "*.xml")):
        if os.path.isfile(file_name):
            with open(file_name, "r") as in_file:
                root = ET.fromstring(in_file.read())
                types.extend(process_xml(root))    

    print("Generating Reflect code...")
    with open(header_file_name, "w") as f:
        f.write("#pragma once\n")
        f.write("// Generated by reflect.py\n\n")
        f.write("#include <Hect/Logic/ComponentRegistry.h>\n")
        f.write("#include <Hect/Logic/GameModeRegistry.h>\n")
        f.write("#include <Hect/Logic/SystemRegistry.h>\n")
        f.write("#include <Hect/Reflection/Type.h>\n\n")
        headers = []
        for type in types:
            header_path = os.path.relpath(type.header, output_dir)
            if not header_path in headers:
                headers.append(header_path)
        headers = sorted(headers)
        for header in headers:
            f.write("#include \"" + header + "\"\n")
        f.write("namespace " + types_namespace + "\n")
        f.write("{\n\n")
        f.write("void registerTypes()\n")
        f.write("{\n")
        f.write("    // Enums\n")
        for type in types:
            if isinstance(type, EnumType):
                f.write("    {\n")
                f.write("        hect::Type& type = hect::Type::create<" + type.name + ">(hect::Kind_Enum, \"" + sanitize_name(type.name) + "\");\n")
                f.write("        hect::Enum& enumType = type.asEnum();\n")
                for value in type.values:
                    str_value = value
                    if "_" in str_value:
                        str_value = str_value.split("_", 1)[1]
                    f.write("        enumType.addValue(" + value + ", \""  + str_value + "\");\n");
                f.write("    }\n")
        f.write("\n    // Types\n")
        for type in types:
            if isinstance(type, ClassType) and not type.is_template and not "Iterator" in type.name:
                f.write("    {\n")
                f.write("        hect::Type& type = hect::Type::create<" + type.name + ">(hect::Kind_Class, \"" + sanitize_name(type.name) + "\");\n")
                if len(type.properties) > 0:
                    f.write("        type.setEncodeFunction([](const void* value, Encoder& encoder)\n")
                    f.write("        {\n")
                    f.write("            const " + type.name + "& typedValue = *reinterpret_cast<const " + type.name + "*>(value);\n")
                    for property in type.properties:
                        if property.is_vector:
                            f.write("            encoder << encodeVector(\"" + property.name + "\", typedValue." + property.name + ");\n")
                        else:
                            f.write("            encoder << encodeValue(\"" + property.name + "\", typedValue." + property.name + ");\n")
                    f.write("        });\n")
                    f.write("        type.setDecodeFunction([](void* value, Decoder& decoder)\n")
                    f.write("        {\n")
                    f.write("            " + type.name + "& typedValue = *reinterpret_cast<" + type.name + "*>(value);\n")
                    for property in type.properties:
                        if property.is_vector:
                            f.write("            decoder >> decodeVector(\"" + property.name + "\", typedValue." + property.name + ");\n")
                        else:
                            f.write("            decoder >> decodeValue(\"" + property.name + "\", typedValue." + property.name + ", " + str(property.is_required).lower() + ");\n")
                    f.write("        });\n")
                else:
                    f.write("        (void)type;\n")
                f.write("    }\n")
        f.write("\n    // Components\n")
        for type in types:
            if isinstance(type, ClassType) and type.is_component:
                print("Found component '" + type.name + "'")
                f.write("    hect::ComponentRegistry::registerType<" + type.name + ">();\n")
        f.write("\n    // Systems\n")
        for type in types:
            if isinstance(type, ClassType) and type.is_system:
                print("Found system '" + type.name + "'")
                f.write("    hect::SystemRegistry::registerType<" + type.name + ">();\n")
        f.write("\n    // Game modes\n")
        for type in types:
            if isinstance(type, ClassType) and type.is_gamemode:
                print("Found game mode '" + type.name + "'")
                f.write("    hect::GameModeRegistry::registerType<" + type.name + ">();\n")
        f.write("}\n\n")
        f.write("}\n")
    shutil.rmtree(os.path.join(output_dir, "xml"))
            
        
