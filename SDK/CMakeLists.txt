FILE(GLOB files Component/Example/*)

install(FILES ${files}
  DESTINATION Component/Example
  COMPONENT sdk)
  
if (WIN32)
    install(FILES Component/create.bat
      DESTINATION Component/
      COMPONENT sdk)
else(WIN32)
    install(FILES Component/create.sh
      DESTINATION Component/
      COMPONENT sdk)
endif(WIN32)