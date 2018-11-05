# Install script for directory: /Users/lennertdekeukeleere/Documents/Work/Code/Geant4GarfieldDegradInterface

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

if("${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/lennertdekeukeleere/Documents/Work/Code/Geant4GarfieldDegradInterface/Debug/InterfaceExample")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Applications/root_v6.14.00/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/src/garfield/build/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/geant4.10.4/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/Qt-5.9.1/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/lennertdekeukeleere/Documents/Work/Code/Geant4GarfieldDegradInterface/Release/InterfaceExample")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Applications/root_v6.14.00/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/src/garfield/build/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/geant4.10.4/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/Qt-5.9.1/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/lennertdekeukeleere/Documents/Work/Code/Geant4GarfieldDegradInterface/MinSizeRel/InterfaceExample")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Applications/root_v6.14.00/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/src/garfield/build/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/geant4.10.4/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/Qt-5.9.1/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      endif()
    endif()
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin" TYPE EXECUTABLE FILES "/Users/lennertdekeukeleere/Documents/Work/Code/Geant4GarfieldDegradInterface/RelWithDebInfo/InterfaceExample")
    if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample" AND
       NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/Applications/root_v6.14.00/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/src/garfield/build/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/geant4.10.4/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      execute_process(COMMAND /usr/bin/install_name_tool
        -delete_rpath "/usr/local/Qt-5.9.1/lib"
        "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      if(CMAKE_INSTALL_DO_STRIP)
        execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/strip" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/bin/InterfaceExample")
      endif()
    endif()
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/Users/lennertdekeukeleere/Documents/Work/Code/Geant4GarfieldDegradInterface/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
