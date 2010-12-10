FILE(GLOB files Component/Example/*)

install(FILES ${files}
  DESTINATION share/Component/Example
  COMPONENT sdk)
  
if (WIN32)
    install(PROGRAMS Component/create.bat
      DESTINATION share/Component/
      COMPONENT sdk)
else(WIN32)
    install(PROGRAMS Component/create.sh
      DESTINATION share/Component/
      COMPONENT sdk)
endif(WIN32)