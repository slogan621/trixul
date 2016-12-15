
;--------------------------------

; The name of the installer
Name "Weather Manager"

; The file to write
OutFile "wminstall.exe"

; The default installation directory
InstallDir $PROGRAMFILES\WeatherManager

; Registry key to check for directory (so if you install again, it will 
; overwrite the old one automatically)
InstallDirRegKey HKLM "Software\WeatherManager" "Install_Dir"

;--------------------------------

; Pages

Page components
Page directory
Page instfiles

UninstPage uninstConfirm
UninstPage instfiles

;--------------------------------

; The stuff to install
Section "WeatherManager"

  SectionIn RO
  
  ; Set output path to the installation directory.
  SetOutPath $INSTDIR
  
  ; Put file there
  File /r "..\..\dist\windows\release\*.dll" "..\..\dist\windows\release\feedwx.exe"
  
  ; Write the installation path into the registry
  WriteRegStr HKLM SOFTWARE\WeatherManager "Install_Dir" "$INSTDIR"
  
  ; Write the uninstall keys for Windows
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\WeatherManager" "DisplayName" "Weather Manager"
  WriteRegStr HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\WeatherManager" "UninstallString" '"$INSTDIR\uninstall.exe"'
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\WeatherManager" "NoModify" 1
  WriteRegDWORD HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\WeatherManager" "NoRepair" 1
  WriteUninstaller "uninstall.exe"
  
SectionEnd

; Optional section (can be disabled by the user)
Section "Start Menu Shortcuts"

  CreateDirectory "$SMPROGRAMS\WeatherManager"
  CreateShortCut "$SMPROGRAMS\WeatherManager\Uninstall.lnk" "$INSTDIR\uninstall.exe" "" "$INSTDIR\uninstall.exe" 0
  CreateShortCut "$SMPROGRAMS\WeatherManager\WeatherManager.lnk" "$INSTDIR\feedwx.exe" "" "$INSTDIR\feedwx.exe" 0
  
SectionEnd

; Optional section (can be disabled by the user)
Section "XML Feeds File"

  ; Set output path to the installation directory.
  SetOutPath $APPDATA\weatherxml
  
  ; Put file there
  File /r "..\wxsites.xml" 
  
SectionEnd

;--------------------------------

; Uninstaller

Section "Uninstall"
  
  ; Remove registry keys
  DeleteRegKey HKLM "Software\Microsoft\Windows\CurrentVersion\Uninstall\WeatherManager"
  DeleteRegKey HKLM SOFTWARE\WeatherManager

  ; Remove files and uninstaller
  Delete $INSTDIR\*.*
  Delete $INSTDIR\uninstall.exe

  ; Remove shortcuts, if any
  Delete "$SMPROGRAMS\WeatherManager\*.*"

  ; Remove directories used
  RMDir "$SMPROGRAMS\WeatherManager"
  RMDir "$INSTDIR"

  Delete $APPDATA\weatherxml\wxsites.xml
  Delete "$APPDATA\weatherxml\*.*"
  RMDir "$APPDATA\weatherxml"

SectionEnd
