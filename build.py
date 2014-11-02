import os
import subprocess
import sys

import prebuild

# Build using Visual Studio 2013
if sys.platform.startswith("win"):
    subprocess.call("call \"%VS120COMNTOOLS%..\\..\\VC\\vcvarsall.bat\" x86_amd64 & msbuild Hect.sln /t:Build /p:Configuration={0};Platform=x64".format(prebuild.build_type), shell=True)
elif sys.platform.startswith("linux"):
    subprocess.call("make", shell=True)
else:
    raise RuntimeError("Platform '{0}' is not supported".format(sys.platform))