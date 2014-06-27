import os
import sys
import zipfile

output_file_name = sys.argv[1]
source_dir = sys.argv[2]

try:
    os.remove(output_file_name)
except OSError:
    pass

print("Creating zip file '" + output_file_name + "' from '" + source_dir + "'...")

zip = zipfile.ZipFile(output_file_name, "a", zipfile.ZIP_DEFLATED)

for root, dir, files in os.walk(source_dir):
    for file in files:
        source = os.path.join(root, file)
        dest = os.path.relpath(source, source_dir)
        print("Adding file '" + dest + "'...")
        zip.write(source, dest)

zip.close()

print("Done")