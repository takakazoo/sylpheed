Set WshShell = CreateObject("WScript.Shell")
Set fso = CreateObject("Scripting.FileSystemObject")
currentDir = fso.GetParentFolderName(WScript.ScriptFullName)

WshShell.Environment("PROCESS")("PATH") = "C:\msys64\mingw64\bin;" & currentDir & "\libsylph\.libs;" & currentDir & "\src\.libs;" & WshShell.Environment("PROCESS")("PATH")
If WshShell.Environment("PROCESS")("LANG") = "" Then
    WshShell.Environment("PROCESS")("LANG") = "ja_JP.UTF-8"
End If

WshShell.Run """" & currentDir & "\src\.libs\sylpheed.exe""", 0, False
