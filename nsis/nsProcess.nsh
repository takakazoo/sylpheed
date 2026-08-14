!ifndef __NSPROCESS_NSH__
!define __NSPROCESS_NSH__

!include "LogicLib.nsh"

!define nsProcess::FindProcess `!insertmacro nsProcess::FindProcess`
!macro nsProcess::FindProcess _FILE _ERR
	nsExec::ExecToStack 'cmd.exe /c "tasklist /NH /FI $\"IMAGENAME eq ${_FILE}$\" | find /i $\"${_FILE}$\""'
	Pop ${_ERR}
	; 0: process found (nsProcess return code 0)
	; 603: process not found (nsProcess return code 603)
	${If} ${_ERR} == 0
		StrCpy ${_ERR} 0
	${Else}
		StrCpy ${_ERR} 603
	${EndIf}
!macroend

!define nsProcess::KillProcess `!insertmacro nsProcess::KillProcess`
!macro nsProcess::KillProcess _FILE _ERR
	nsExec::ExecToStack 'taskkill /F /IM "${_FILE}"'
	Pop ${_ERR}
!macroend

!define nsProcess::CloseProcess `!insertmacro nsProcess::CloseProcess`
!macro nsProcess::CloseProcess _FILE _ERR
	nsExec::ExecToStack 'taskkill /IM "${_FILE}"'
	Pop ${_ERR}
!macroend

!define nsProcess::Unload `!insertmacro nsProcess::Unload`
!macro nsProcess::Unload
!macroend

!endif
