cmake_minimum_required(VERSION 3.8)

option(ENABLE_FLATPAK "Enable install rules for Flatpak packaging" OFF)

if(CMAKE_BUILD_TYPE STREQUAL "Release")	
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os -fdata-sections -ffunction-sections -fno-ident" CACHE STRING "C++ Compiler flags" FORCE)
	if (WIN32)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mwindows" CACHE STRING "C++ Compiler flags" FORCE)
	endif()
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm -static-libstdc++ -static-libgcc -fPIC -fno-ident -s -Wl,--gc-sections" CACHE STRING "EXE link flags" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g3 -Wall " CACHE STRING "C++ Compiler flags" FORCE)
	set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lm " CACHE STRING "EXE link flags" FORCE)
endif()

add_subdirectory("external")
add_subdirectory("castle-mm")
if(ENABLE_FLATPAK)
    install(TARGETS castle-mm DESTINATION bin)
    install(FILES flatpak/io.github.ashrindy.castlemodmanager.desktop DESTINATION share/applications)
    install(FILES flatpak/io.github.ashrindy.castlemodmanager.png DESTINATION share/icons/hicolor/256x256/apps)
endif()
