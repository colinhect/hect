from subprocess import call

call("Dependencies\\AStyle\\AStyle.exe --style=allman --recursive -n Hect\\Source\\*.cpp Hect\\Source\\*.h Hect\\Source\\.inl", shell=True)
call("Dependencies\\AStyle\\AStyle.exe --style=allman --recursive -n Zeroth\\Source\\*.cpp Zeroth\\Source\\*.h Zeroth\\Source\\.inl", shell=True)