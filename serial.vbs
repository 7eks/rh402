dim Shell
set Shell=createobject("wscript.shell")
userProfilePath = Shell.ExpandEnvironmentStrings("%UserProfile%")
directory = userProfilePath & "\Documents\RH402"
Shell.CurrentDirectory = directory
shell.run "start.bat " & Chr(34), 0
set shell=nothing
