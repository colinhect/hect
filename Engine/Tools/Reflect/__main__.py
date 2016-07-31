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
namespace = sys.argv[3]

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
        self.is_scene = False
        self.is_encodable = False
        
class EnumType(Type):
    def __init__(self, header, id, name):
        Type.__init__(self, header, id, name)
        self.values = []
        
class Property:
    def __init__(self, name):
        self.name = name
        self.is_required = False
        self.is_vector = False
        self.is_enum = False
        
def is_enum(type):
    return isinstance(type, EnumType)
    
def is_class(type):
    return isinstance(type, ClassType) and not type.is_template and not "Iterator" in type.name
    
def is_component(type):
    return isinstance(type, ClassType) and type.is_component
    
def is_system(type):
    return isinstance(type, ClassType) and type.is_system
    
def is_scene(type):
    return isinstance(type, ClassType) and type.is_scene
    
# Returns a sorted list of header files that the given types are defined in
def headers_for_types(types):
    headers = []
    for type in types:
        header_path = os.path.relpath(type.header, output_dir)
        if not header_path in headers:
            headers.append(header_path)
    return sorted(headers)
    
# Instantiate a template with the given variables
def emit_template(template, **kwargs):
    path = os.path.dirname(os.path.abspath(__file__))
    
    # Read the template text from the file
    with open(os.path.join(path, "Templates", template + ".template"), "r") as file:
        code = file.read()
        
    # Replace variables with their associated values
    for key, value in kwargs.iteritems():
        code = code.replace("$({})".format(key), value)
        
    return code
    
# Removes the namespace prefix of a type name
def sanitize_name(name):
    sanitized = name.split("::", 1)
    if len(sanitized) == 2:
        return sanitized[1]
    else:
        return name
        
# Intersperses new line characters between each item in the given list of
# strings
def intersperse_newlines(strings):
    return "\n".join(strings)
    
# Removes blank lines in the given string
def remove_blank_lines(string):
    return "\n".join([s for s in string.splitlines() if s])
    
# Generates the type registration code for the given types
def generate_reflection_code(types):
    print("Generating Reflect code...")
    
    includes = []
    headers = headers_for_types(types)
    for header in headers:
        includes.append(emit_template("Include", file_name=header))
    includes = intersperse_newlines(includes)
        
    # Register enums
    register_enums = []
    for type in filter(is_enum, types):
        add_enum_values = []
        for value in type.values:
            add_enum_values.append(emit_template("AddEnumValue",
                type_name=type.name,
                value=value))
        add_enum_values = intersperse_newlines(add_enum_values)
        register_enums.append(emit_template("RegisterEnum",
            type_name=type.name,
            add_enum_values=add_enum_values))
    register_enums = intersperse_newlines(register_enums)
            
    # Register classes
    register_classes = []
    for type in filter(is_class, types):
        set_encode_function = ""
        set_decode_function = ""
        if len(type.properties) > 0 and (type.is_component or type.is_system or type.is_encodable):
            encode_properties = []
            for property in type.properties:
                if property.is_vector:
                    encode_properties.append(emit_template("EncodePropertyVector",
                        property_name=property.name))
                elif property.is_enum:
                    encode_properties.append(emit_template("EncodePropertyEnum",
                        property_name=property.name))
                else:
                    encode_properties.append(emit_template("EncodePropertyValue",
                        property_name=property.name))
            encode_properties = intersperse_newlines(encode_properties)
            set_encode_function = emit_template("SetEncodeFunction",
                type_name=type.name,
                encode_properties=encode_properties)
            decode_properties = []
            for property in type.properties:
                is_required = str(property.is_required).lower()
                if property.is_vector:
                    decode_properties.append(emit_template("DecodePropertyVector",
                        property_name=property.name,
                        is_required=is_required))
                elif property.is_enum:
                    decode_properties.append(emit_template("DecodePropertyEnum",
                        property_name=property.name,
                        is_required=is_required))
                else:
                    decode_properties.append(emit_template("DecodePropertyValue",
                        property_name=property.name,
                        is_required=is_required))
            decode_properties = intersperse_newlines(decode_properties)
            set_decode_function = emit_template("SetDecodeFunction",
                type_name=type.name,
                decode_properties=decode_properties)
        register_classes.append(emit_template("RegisterClass",
            type_name=type.name,
            set_encode_function=set_encode_function,
            set_decode_function=set_decode_function))
    register_classes = remove_blank_lines(intersperse_newlines(register_classes))
    
    # Register components
    register_components = []
    for type in filter(is_component, types):
        print("Found component '" + type.name + "'")
        register_components.append(emit_template("RegisterComponent",
            type_name=type.name))
    register_components = intersperse_newlines(register_components)
            
    # Register systems
    register_systems = []
    for type in filter(is_system, types):
        print("Found system '" + type.name + "'")
        register_systems.append(emit_template("RegisterSystem",
            type_name=type.name))
    register_systems = intersperse_newlines(register_systems)
            
    # Register scenes
    register_scenes = []
    for type in filter(is_scene, types):
        print("Found scene '" + type.name + "'")
        register_scenes.append(emit_template("RegisterScene",
            type_name=type.name))
    register_scenes = intersperse_newlines(register_scenes)
            
    code = emit_template("Layout",
        includes=includes,
        namespace=namespace,
        register_enums=register_enums,
        register_classes=register_classes,
        register_components=register_components,
        register_systems=register_systems,
        register_scenes=register_scenes)
    return code
    
# Generates Doxygen XML given the input directory containing the C++ headers
# and the output directory to put the generated XML files
def generate_doxygen_xml(input_dir, output_dir):
    print("Generating Doxygen XML...")
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
        "JAVADOC_AUTOBRIEF" : "YES",
        "OUTPUT_DIRECTORY" : output_dir,
        "OUTPUT_LANGUAGE" : "English",
        "PROJECT_NAME" : "Hect Reflect",
        "QUIET" : "YES",
        "RECURSIVE" : "YES",
        "WARN_IF_DOC_ERROR" : "YES",
        "WARN_IF_UNDOCUMENTED" : "NO",
        "WARN_NO_PARAMDOC" : "NO",
        "WARNINGS" : "YES",
        "XML_OUTPUT" : "xml",
        "XML_PROGRAMLISTING" : "NO"
    }
    with open("Doxyfile", "w") as doxyfile:
        for key, value in doxygen_config.items():
            doxyfile.write(key + " = " + value + "\n")
        doxyfile.write("ALIASES += component=\"[component]\"\n")
        doxyfile.write("ALIASES += system=\"[system]\"\n")
        doxyfile.write("ALIASES += scene=\"[scene]\"\n")
        doxyfile.write("ALIASES += encodable=\"[encodable]\"\n")
        doxyfile.write("ALIASES += property=\"[property]\"\n")
        doxyfile.write("ALIASES += property{1}=\"[property]{\\1}\"\n")
        doxyfile.write("ALIASES += property{2}=\"[property]{\\1}{\\2}\"\n")
        doxyfile.write("ALIASES += property{3}=\"[property]{\\1}{\\2}{\\3}\"\n")
        doxyfile.write("ALIASES += property{4}=\"[property]{\\1}{\\2}{\\3}{\\4}\"\n")
    call("doxygen", shell=True)
    os.remove("Doxyfile")
    
# Parses the types in all of the generated Doxygen XML files in the specified
# directory
def parse_doxygen_xml(xml_dir):
    print("Parsing Doxygen XML...")
    types = []
    for file_name in glob.glob(os.path.join(xml_dir, "xml", "*.xml")):
        if os.path.isfile(file_name):
            with open(file_name, "r") as in_file:
                root = ET.fromstring(in_file.read())
                types.extend(parse_types(root))
    return types
    
# Parses the types in a generated Doxygen XML document
def parse_types(root):
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
                name = sanitize_name(compounddef.find("compoundname").text)
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
                            elif "[scene]" in para.text:
                                class_type.is_scene = True
                            elif "[encodable]" in para.text:
                                class_type.is_encodable = True
                templateparamlist = compounddef.find("templateparamlist")
                if not templateparamlist is None:
                    class_type.is_template = True
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
                                        if "{required}" in para.text:
                                            property.is_required = True
                                        if "{enum}" in para.text:
                                            property.is_enum = True
                                        if "{vector}" in para.text:
                                            property.is_vector = True
                                        class_type.properties.append(property)
                types.append(class_type)
    return types
    
if __name__ == "__main__":
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
        
    generate_doxygen_xml(input_dir, output_dir)
    types = parse_doxygen_xml(output_dir)
    
    shutil.rmtree(os.path.join(output_dir, "xml"))
    
    code = generate_reflection_code(types)
    
    with open(header_file_name, "w") as f:
        f.write(code)
