from subprocess import call

call("Dependencies\\AStyle\\AStyle.exe --style=allman --recursive -n Source\\*.cpp Hect\\Source\\*.h Hect\\Source\\.inl", shell=True)