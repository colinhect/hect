import glob
import os
import sys
import re
import xml.etree.ElementTree as ET

namespaces = []

class Type:
    def __init__(self, header, id, name):
        self.header = header
        self.id = id
        self.name = name
        
class ClassType(Type):
    def __init__(self, header, id, name):
        Type.__init__(self, header, id, name)
        self.inner_class_ids = []
        self.is_template = False
        
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
                    enum_type = EnumType(header, id, namespace + "::" + name)
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
                types.append(class_type)
    return types
        
if __name__ == "__main__":
    types = []
    input_dir = sys.argv[1]
    output_dir = sys.argv[2]
    types_name = sys.argv[3]
    for file_name in glob.glob(input_dir):
        if os.path.isfile(file_name):
            with open(file_name, "r") as in_file:
                root = ET.fromstring(in_file.read())
                types.extend(process_xml(root))
    source_file_name = os.path.join(output_dir, types_name + ".cpp")    
    with open(source_file_name, "w") as f:
        f.write("// Generated by reflect.py\n")
        f.write("#include <Hect/Reflection/Type.h>\n\n")
        headers = []
        for type in types:
            if isinstance(type, EnumType):
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
        f.write("}\n")
            
        