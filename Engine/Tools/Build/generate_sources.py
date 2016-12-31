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
import os

import templates

def action(input_directory, output_filename, exclude_extensions):
    filepaths = []
    for root, dirs, files in os.walk(input_directory):
        if input_directory == ".":
            root = os.path.relpath(root)
        else:
            root = os.path.relpath(root, input_directory)

        for basename in files:
            filename, extension = os.path.splitext(basename)
            if extension not in exclude_extensions:
                filepaths.append(os.path.join(root, basename))

    with open(output_filename, "w") as file:
        file.write("# Generated by Build Tool (see Engine/Tools/Build for details)\n\n")
        groups = {}
        for filepath in filepaths:
            group = os.path.split(filepath)[0]
            if not group in groups:
                groups[group] = []
            groups[group].append(filepath.replace("\\", "/"))

        keys = sorted(groups.keys(), key=lambda s: s.lower())
        for group in keys:
            group_name = resolve_group_name(group)
            group_var_name = resolve_group_var_name(group)

            source_files = []
            for file_name in groups[group]:
                source_files.append(templates.emit("CMakeSourceFile",
                    file_name=file_name))

            file.write(templates.emit("CMakeSetVar",
                var_name=group_var_name,
                value=templates.intersperse_newlines(source_files)))

            if len(source_files) > 0:
                file.write(templates.emit("CMakeSetGroup",
                    group_name=group_name,
                    var_name=group_var_name))

        source_groups = []
        for group in keys:
            group_var_name = resolve_group_var_name(group)
            source_groups.append(templates.emit("CMakeSourceVar",
                var_name=group_var_name))

        file.write(templates.emit("CMakeSetSourceFiles",
            value=templates.intersperse_newlines(source_groups)))

def resolve_group_var_name(group):
    if group == ".":
        return "SOURCE"
    else:
        group_postfix = group.replace("\\", "_").upper()
        return "SOURCE_{}".format(group_postfix)

def resolve_group_name(group):
    return group.replace("\\", "\\\\")
