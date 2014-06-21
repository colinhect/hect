import glob
import os
import sys
import re
import xml.etree.ElementTree as ET

class Type:
    def __init__(self, header, name):
        self.header = header
        self.name = name
        
class EnumType(Type):
    def __init__(self, header, name, values):
        Type.__init__(self, header, name)
        self.values = values

def process_xml(root, namespace):
    types = []
    for compounddef in root.iter("compounddef"):
        
        # Get the name of the type
        name = compounddef.find("compoundname").text
        
        # Get the path to the header file
        header = compounddef.find("location")
        if not header is None:
            header = header.attrib["file"]
                
        # Get the kind of the type
        kind = compounddef.attrib["kind"]
        
        # If the kind is a namespace then it will be treated as an enum
        if kind == "namespace" and name != namespace:
            values = []
            for sectiondef in compounddef.iter("sectiondef"):
                for memberdef in compounddef.iter("memberdef"):
                    for enumvalue in memberdef.iter("enumvalue"):
                        values.append((enumvalue.find("name").text, name + "::" + enumvalue.find("name").text))
            name = name + "::Enum"
            types.append(EnumType(header, name, values))
    return types

if __name__ == "__main__":
    types = []

    files = sys.argv[1]
    output_dir = sys.argv[2]
    namespace = sys.argv[3]
    
    # Process each file
    for file_name in glob.glob(files):
        with open(file_name, "r") as in_file:
            root = ET.fromstring(in_file.read())
            types.extend(process_xml(root, namespace))
    
    # Remove namespaces in type names
    for type in types:
        type.name = type.name.replace(namespace + "::", "").strip()

    # Write .cpp file
    with open(os.path.join(output_dir, "_" + namespace + "_reflect.cpp"), "w") as code:
        code.write("// Generated by reflect.py\n\n")
        code.write("#include \"Hect/Reflection/Enum.h\"\n")
        for type in types:
            code.write("#include \"" + type.header + "\"\n")
        code.write("\n")
        code.write("using namespace " + namespace + ";\n\n")
        for type in types:
            code.write("// " + type.name + "\n")
            code.write("template <>\n")
            code.write(type.name + " Enum::fromString<" + type.name + ">(const std::string& string)\n")
            code.write("{\n")
            code.write("    static std::map<std::string, " + type.name + "> _stringToValue;\n")
            code.write("    if (_stringToValue.empty())\n")
            code.write("    {\n")
            if isinstance(type, EnumType):
                for value in type.values:
                    code.write("        _stringToValue[\"" + value[0] + "\"] = " + value[1] + ";\n")
            code.write("    }\n")
            code.write("    auto it = _stringToValue.find(string);\n")
            code.write("    if (it == _stringToValue.end())\n")
            code.write("    {\n")
            code.write("        throw Error(format(\"Invalid string '%s' for type '" + type.name + "'\", string.c_str()));\n")
            code.write("    }\n")
            code.write("    return it->second;\n")
            code.write("}\n\n")
            code.write("template <>\n")
            code.write("const std::string& Enum::toString<" + type.name + ">(" + type.name + " value)\n")
            code.write("{\n")
            code.write("    static std::map<" + type.name + ", std::string> _valueToString;\n")
            code.write("    if (_valueToString.empty())\n")
            code.write("    {\n")
            if isinstance(type, EnumType):
                for value in type.values:
                    code.write("        _valueToString[" + value[1] + "] = \"" + value[0] + "\";\n")
            code.write("    }\n")
            code.write("    auto it = _valueToString.find(value);\n")
            code.write("    if (it == _valueToString.end())\n")
            code.write("    {\n")
            code.write("        throw Error(\"Invalid value for type '" + type.name + "'\");\n")
            code.write("    }\n")
            code.write("    return it->second;\n")
            code.write("}\n\n")