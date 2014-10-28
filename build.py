import os
import subprocess
import sys

import prebuild

config = "debug"

# Validate config
if len(sys.argv) > 0:
    arg = sys.argv[1].lower()
    if arg == "debug" or arg == "release":
        config = arg
    else:
        print("Unknown config '{0}'".format(arg))

# Build using Visual Studio 2013
if sys.platform.startswith("win"):
    subprocess.call("call \"%VS120COMNTOOLS%..\\..\\VC\\vcvarsall.bat\" x86_amd64 & msbuild Hect.sln /t:Build /p:Configuration={0};Platform=x64".format(config), shell=True)
else:
    print("Platform '{0}' is not supported".format(sys.platform))