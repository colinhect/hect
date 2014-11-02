import os
import subprocess
import sys

build_path = "Build"
build_type = "debug"

# Validate build type
if len(sys.argv) > 1:
    arg = sys.argv[1].lower()
    if arg == "debug":
        build_type = "Debug"
    elif arg == "release":
        build_type = "Release"
    else:
        raise RuntimeError("Unknown build type '{0}'".format(arg))

# Create the build directory if it does not exist
if not os.path.exists(build_path):
    os.makedirs(build_path)

# Change to the build directory
os.chdir(build_path)

# Invoke CMake
if sys.platform.startswith("win"):
    subprocess.call("cmake ../ -G \"Visual Studio 12 Win64\"", shell=True)
elif sys.platform.startswith("linux"):
    subprocess.call("cmake ../ -DCMAKE_BUILD_TYPE={0} -G \"Unix Makefiles\"".format(build_type), shell=True)
else:
    raise RuntimeError("Platform '{0}' is not supported".format(sys.platform))
