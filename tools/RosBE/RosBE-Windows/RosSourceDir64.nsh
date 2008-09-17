!macro CUSTOM_PAGE_ROSDIRECTORY64
    PageEx custom
        Caption ""
        PageCallBacks "RosDir.show" "RosDir.leave"
    PageExEnd

    Function RosDir.show
        !insertmacro MUI_HEADER_TEXT "Select ReactOS 64 Bit Source Location" "Select the directory where the ReactOS 64 Bit sources are located or will be checked out to later."
        !insertmacro INSTALLOPTIONS_WRITE "RosSourceDir64.ini" "Field 2" "State" "$PROFILE\reactos"
        !insertmacro INSTALLOPTIONS_DISPLAY "RosSourceDir64.ini"
    FunctionEnd

    Function RosDir.leave
        !insertmacro INSTALLOPTIONS_READ $REACTOS_SOURCE_DIRECTORY "RosSourceDir64.ini" "Field 2" "State"
    FunctionEnd
!macroend
