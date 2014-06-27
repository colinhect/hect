from subprocess import call

call("..\\Dependencies\\AStyle\\AStyle.exe --style=allman --recursive -n ..\\Source\\*.cpp ..\\Source\\*.h ..\\Source\\.inl", shell=True)