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
types_name = sys.argv[3]

doxygen_config = {
    "ALLOW_UNICODE_NAMES" : "NO",
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

class Type:
    def __init__(self, header, id, name):
        self.header = header
        self.id = id
        self.name = name
        self.base_name = None
        self.base_id = None
        
class ClassType(Type):
    def __init__(self, header, id, name):
        Type.__init__(self, header, id, name)
        self.inner_class_ids = []
        self.is_derived_from = False
        self.is_template = False
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
                for innerclass in compounddef.iter("innerclass"):
                    class_type.inner_class_ids.append(innerclass.attrib["refid"])
                class_type.is_template = not compounddef.find("templateparamlist") is None
                class_type.is_derived_from = not compounddef.find("derivedcompoundref") is None
                basecompoundref = compounddef.find("basecompoundref")
                if basecompoundref is not None:
                    class_type.base_name = basecompoundref.text
                    if "refid" in basecompoundref.attrib:
                        class_type.base_id = basecompoundref.attrib["refid"]                    
                types.append(class_type)
    return types
    
def find_by_name(types, name):
    for type in types:
        if type.name == name:
            return type
    return None
    
def find_by_id(types, id):
    for type in types:
        if type.id == id:
            return type
    return None
    
def has_base_of(types, type, bases):
    base_type = None
    base_name = None
    if type.base_id is not None:
        base_type = find_by_id(types, type.base_id)
    elif type.base_name is not None:
        base_type = find_by_name(types, type.base_name)
            
    if base_type is not None:
        base_name = base_type.name
    else:
        base_name = type.base_name
        
    if base_name is not None:
        for base in bases:
            if base_name == base:
                return True
        if base_type is not None:
            return has_base_of(types, base_type, bases)
    return False
        
if __name__ == "__main__":
    types = []
    
    print("Hect Reflect - " + str(sys.argv[1:]))
    
    source_file_name = os.path.join(output_dir, types_name + ".cpp")    
    
    last_modified = 0
    for root, dirnames, filenames in os.walk(input_dir):
        for filename in fnmatch.filter(filenames, "*.h"):
            t = os.path.getmtime(os.path.join(root, filename))
            if t > last_modified:
                last_modified = t
    
    last_generated = 0
    if os.path.isfile(source_file_name):
        last_generated = os.path.getmtime(source_file_name)
    
    if last_generated >= last_modified:
        print("Reflect code is up to date...")
        exit()
        
    print("Generating Doxygen XML...")
    with open("Doxyfile", "w") as doxyfile:
        for key, value in doxygen_config.items():
            doxyfile.write(key + " = " + value + "\n")
    call("doxygen", shell=True)
    os.remove("Doxyfile")
    
    print("Parsing Doxygen XML...")
    for file_name in glob.glob(os.path.join(output_dir, "xml", "*.xml")):
        if os.path.isfile(file_name):
            with open(file_name, "r") as in_file:
                root = ET.fromstring(in_file.read())
                types.extend(process_xml(root))
    
    print("Generating Reflect code...")
    with open(source_file_name, "w") as f:
        f.write("// Generated by reflect.py\n")
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
        f.write("\n")            
        f.write("class " + types_name + " { };\n\n")     
        f.write("template <>\n")
        f.write("void hect::Type::registerTypes<" + types_name + ">()\n")
        f.write("{\n")
        for type in types:
            if isinstance(type, EnumType):
                f.write("    {\n")
                f.write("        hect::Type& type = hect::Type::create<" + type.name + ">(hect::Kind_Enum, \"" + type.name + "\");\n")
                f.write("        hect::Enum& enumType = type.asEnum();\n")
                for value in type.values:
                    str_value = value
                    if "_" in str_value:
                        str_value = str_value.split("_", 1)[1]
                    f.write("        enumType.addValue(" + value + ", \""  + str_value + "\");\n");
                f.write("    }\n")
        for type in types:
            if isinstance(type, ClassType):
                name_without_namespace = type.name.split("::", 1)[-1]
                if has_base_of(types, type, [ "Component", "hect::Component", "Component< " + name_without_namespace + " >", "hect::Component< " + name_without_namespace + " >"]):
                    type.is_component = True
                elif has_base_of(types, type, [ "System", "hect::System" ]):
                    type.is_system = True
                elif has_base_of(types, type, [ "GameMode", "hect::GameMode" ]):
                    type.is_gamemode = True
        for type in types:
            if isinstance(type, ClassType) and type.is_component:
                f.write("    hect::ComponentRegistry::registerType<" + type.name + ">();\n")
        for type in types:
            if isinstance(type, ClassType) and type.is_system and not type.is_derived_from:
                f.write("    hect::SystemRegistry::registerType<" + type.name + ">();\n")
        for type in types:
            if isinstance(type, ClassType) and type.is_gamemode:
                f.write("    hect::GameModeRegistry::registerType<" + type.name + ">();\n")
        f.write("}\n")
    shutil.rmtree(os.path.join(output_dir, "xml"))
            
        