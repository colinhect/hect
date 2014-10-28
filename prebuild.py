import os
import subprocess
import sys

build_path = "Build"

# Create the build directory if it does not exist
if not os.path.exists(build_path):
    os.makedirs(build_path)

# Change to the build directory
os.chdir(build_path)

# Invoke CMake
if sys.platform.startswith("win"):
    subprocess.call("cmake ../ -G \"Visual Studio 12 Win64\"", shell=True)
elif sys.platform.startswith("linux"):
    subprocess.call("cmake ../ -G \"Unix Makefiles\"", shell=True)
else:
    print("Platform '{0}' is not supported".format(sys.platform))
