import os
import time
import shutil
from subprocess import call

os.chdir("../")
if os.path.exists("Docs/html"):
    shutil.rmtree("Docs/html")

call("doxygen", shell=True)