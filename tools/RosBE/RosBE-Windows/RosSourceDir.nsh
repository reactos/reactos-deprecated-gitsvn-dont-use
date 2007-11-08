!macro CUSTOM_PAGE_ROSDIRECTORY
    PageEx custom
        Caption ""
        PageCallBacks "RosDir.show" "RosDir.leave"
    PageExEnd

    Function RosDir.show
        !insertmacro MUI_HEADER_TEXT "Select ReactOS Source Location" "Select the directory where the ReactOS sources are located or will be checked out to later."
        !insertmacro MUI_INSTALLOPTIONS_WRITE "RosSourceDir.ini" "Field 2" "State" "$INSTDIR\Source"
        !insertmacro MUI_INSTALLOPTIONS_DISPLAY "RosSourceDir.ini"
    FunctionEnd

    Function RosDir.leave
        !insertmacro MUI_INSTALLOPTIONS_READ $REACTOS_SOURCE_DIRECTORY "RosSourceDir.ini" "Field 2" "State"
    FunctionEnd
!macroend
