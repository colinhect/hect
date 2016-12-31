###############################################################################
## This source file is part of Hect.
##
## Copyright (c) 2016 Colin Hill
##
## Permission is hereby granted, free of charge, to any person obtaining a copy
## of this software and associated documentation files (the "Software"), to
## deal in the Software without restriction, including without limitation the
## rights to use, copy, modify, merge, publish, distribute, sublicense, and#or
## sell copies of the Software, and to permit persons to whom the Software is
## furnished to do so, subject to the following conditions:
##
## The above copyright notice and this permission notice shall be included in
## all copies or substantial portions of the Software.
##
## THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
## IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
## FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
## AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
## LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
## FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
## IN THE SOFTWARE.
###############################################################################
import glob
import os
import sys
import re
import xml.etree.ElementTree as ET
import shutil
import time
import fnmatch
from subprocess import call

import templates

def action(input_directory, output_directory, namespace):
    print("Generating '{}' types...".format(namespace))

    if not os.path.exists(output_directory):
        os.makedirs(output_directory)

    if not os.path.exists(os.path.join(output_directory, "xml")):
        os.makedirs(os.path.join(output_directory, "xml"))

    header_file_name = os.path.join(output_directory, "RegisterTypes.h")

    last_modified = 0
    for root, dirnames, filenames in os.walk(input_directory):
        for filename in fnmatch.filter(filenames, "*.h"):
            t = os.path.getmtime(os.path.join(root, filename))
            if t > last_modified:
                last_modified = t

    last_generated = 0
    if os.path.isfile(header_file_name):
        last_generated = os.path.getmtime(header_file_name)

    if last_generated >= last_modified:
        print("Reflection code is up to date...")
        exit()

    generate_doxygen_xml(input_directory, output_directory)
    types = parse_doxygen_xml(output_directory)

    shutil.rmtree(os.path.join(output_directory, "xml"))

    code = generate_reflection_code(types, output_directory, namespace)

    with open(header_file_name, "w") as f:
        f.write(code)

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
def headers_for_types(types, output_directory):
    headers = []
    for type in types:
        header_path = os.path.relpath(type.header, output_directory)
        if not header_path in headers:
            headers.append(header_path)
    return sorted(headers)

# Removes the namespace prefix of a type name
def sanitize_name(name):
    sanitized = name.split("::", 1)
    if len(sanitized) == 2:
        return sanitized[1]
    else:
        return name

# Removes blank lines in the given string
def remove_blank_lines(string):
    return "\n".join([s for s in string.splitlines() if s])

# Generates the type registration code for the given types
def generate_reflection_code(types, output_directory, namespace):
    print("Generating Reflect code...")

    includes = []
    headers = headers_for_types(types, output_directory)
    for header in headers:
        includes.append(templates.emit("Include", file_name=header))
    includes = templates.intersperse_newlines(includes)

    # Register enums
    register_enums = []
    for type in filter(is_enum, types):
        add_enum_values = []
        for value in type.values:
            add_enum_values.append(templates.emit("AddEnumValue",
                type_name=type.name,
                value=value))
        add_enum_values = templates.intersperse_newlines(add_enum_values)
        register_enums.append(templates.emit("RegisterEnum",
            type_name=type.name,
            add_enum_values=add_enum_values))
    register_enums = templates.intersperse_newlines(register_enums)

    # Register classes
    register_classes = []
    for type in filter(is_class, types):
        set_encode_function = ""
        set_decode_function = ""
        if len(type.properties) > 0 and (type.is_component or type.is_system or type.is_encodable):
            encode_properties = []
            for property in type.properties:
                if property.is_vector:
                    encode_properties.append(templates.emit("EncodePropertyVector",
                        property_name=property.name))
                elif property.is_enum:
                    encode_properties.append(templates.emit("EncodePropertyEnum",
                        property_name=property.name))
                else:
                    encode_properties.append(templates.emit("EncodePropertyValue",
                        property_name=property.name))
            encode_properties = templates.intersperse_newlines(encode_properties)
            set_encode_function = templates.emit("SetEncodeFunction",
                type_name=type.name,
                encode_properties=encode_properties)
            decode_properties = []
            for property in type.properties:
                is_required = str(property.is_required).lower()
                if property.is_vector:
                    decode_properties.append(templates.emit("DecodePropertyVector",
                        property_name=property.name,
                        is_required=is_required))
                elif property.is_enum:
                    decode_properties.append(templates.emit("DecodePropertyEnum",
                        property_name=property.name,
                        is_required=is_required))
                else:
                    decode_properties.append(templates.emit("DecodePropertyValue",
                        property_name=property.name,
                        is_required=is_required))
            decode_properties = templates.intersperse_newlines(decode_properties)
            set_decode_function = templates.emit("SetDecodeFunction",
                type_name=type.name,
                decode_properties=decode_properties)
        register_classes.append(templates.emit("RegisterClass",
            type_name=type.name,
            set_encode_function=set_encode_function,
            set_decode_function=set_decode_function))
    register_classes = remove_blank_lines(templates.intersperse_newlines(register_classes))

    # Register components
    register_components = []
    for type in filter(is_component, types):
        print("Found component '" + type.name + "'")
        register_components.append(templates.emit("RegisterComponent",
            type_name=type.name))
    register_components = templates.intersperse_newlines(register_components)

    # Register systems
    register_systems = []
    for type in filter(is_system, types):
        print("Found system '" + type.name + "'")
        register_systems.append(templates.emit("RegisterSystem",
            type_name=type.name))
    register_systems = templates.intersperse_newlines(register_systems)

    # Register scenes
    register_scenes = []
    for type in filter(is_scene, types):
        print("Found scene '" + type.name + "'")
        register_scenes.append(templates.emit("RegisterScene",
            type_name=type.name))
    register_scenes = templates.intersperse_newlines(register_scenes)

    code = templates.emit("Layout",
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
def generate_doxygen_xml(input_directory, output_directory):
    print("Generating Doxygen XML...")
    doxygen_config = {
        "DOXYFILE_ENCODING" : "UTF-8",
        "FILE_PATTERNS" : "*.h",
        "GENERATE_BUGLIST" : "NO",
        "GENERATE_DEPRECATEDLIST" : "NO",
        "GENERATE_HTML" : "NO",
        "GENERATE_LATEX" : "NO",
        "GENERATE_XML" : "YES",
        "INPUT" : input_directory,
        "INPUT_ENCODING" : "UTF-8",
        "JAVADOC_AUTOBRIEF" : "YES",
        "OUTPUT_DIRECTORY" : output_directory,
        "OUTPUT_LANGUAGE" : "English",
        "PROJECT_NAME" : "Hect Reflect",
        "QUIET" : "YES",
        "RECURSIVE" : "YES",
        "WARN_IF_DOC_ERROR" : "YES",
        "WARN_IF_UNDOCUMENTED" : "NO",
        "WARN_NO_PARAMDOC" : "NO",
        "WARNINGS" : "NO",
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
