from subprocess import call

call("..\\Dependencies\\AStyle\\AStyle.exe --style=allman --recursive -n ..\\Source\\*.cpp ..\\Source\\*.h ..\\Source\\*.inl", shell=True)
call("..\\Dependencies\\AStyle\\AStyle.exe --style=allman --recursive -n ..\\Tests\\*.cpp ..\\Tests\\*.h ..\\Tests\\*.inl", shell=True)
call("..\\Dependencies\\AStyle\\AStyle.exe --style=allman --recursive -n ..\\..\\Projects\\*.cpp ..\\..\\Projects\\*.h ..\\..\\Projects\\*.inl", shell=True)