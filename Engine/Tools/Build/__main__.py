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
import sys

def generate_sources(argv):
    input_directory = argv[2]
    output_filename = argv[3]
    exclude_extensions = argv[4]
    exclude_extensions = exclude_extensions.split(",")
    import generate_sources
    generate_sources.action(input_directory, output_filename, exclude_extensions)

def generate_types(argv):
    input_directory = argv[2]
    output_directory = argv[3]
    namespace = argv[4]
    import generate_types
    generate_types.action(input_directory, output_directory, namespace)

if __name__ == "__main__":
    argv = sys.argv
    print("Build {}".format(argv[1:]))
    action = argv[1]
    if action == "GenerateSources":
        generate_sources(argv)
    elif action == "GenerateTypes":
        generate_types(argv)
