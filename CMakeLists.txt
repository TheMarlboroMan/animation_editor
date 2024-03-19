cmake_minimum_required(VERSION 3.5.1)

project(animation_editor)

option(BUILD_REGULAR "builds regular binary" OFF)
option(BUILD_APPIMAGE "builds for appimage" OFF)
option(BUILD_TESTS "Build test code" ON)
option(BUILD_DEBUG "Build a debug release" OFF)
option(WITH_LEXICAL_NORMALIZE "just see the tile_editor or sprite table apps for this" ON)

#application version and build type
set(MAJOR_VERSION 1)
set(MINOR_VERSION 1)
set(PATCH_VERSION 6)

if(${BUILD_REGULAR})

	set(BUILD_VERSION "bin")
elseif(${BUILD_APPIMAGE})

	set(BUILD_VERSION "AppImage")
else()

	message(FATAL_ERROR "a build type regular or appimage must be chosen")
endif()

if(${BUILD_DEBUG})

	set(BUILD_VERSION "${BUILD_VERSION}.debug")
endif()

if(${CMAKE_VERSION} VERSION_LESS "3.22.0")

	add_definitions(-DMAJOR_VERSION=${MAJOR_VERSION})
	add_definitions(-DMINOR_VERSION=${MINOR_VERSION})
	add_definitions(-DPATCH_VERSION=${PATCH_VERSION})
	add_definitions(-DBUILD_VERSION=\"${BUILD_VERSION}\")
else()

	add_compile_definitions(MAJOR_VERSION=${MAJOR_VERSION})
	add_compile_definitions(MINOR_VERSION=${MINOR_VERSION})
	add_compile_definitions(PATCH_VERSION=${PATCH_VERSION})
	add_compile_definitions(BUILD_VERSION=\"${BUILD_VERSION}\")
endif()

message("will build ${MAJOR_VERSION}.${MINOR_VERSION}.${PATCH_VERSION}-${BUILD_VERSION}")

#Compiler flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(${BUILD_DEBUG})

	set(CMAKE_BUILD_TYPE Debug)
	if(${CMAKE_VERSION} VERSION_LESS "3.22.0")
		add_definitions(-DLIBDANSDL2_DEBUG)
		add_definitions(-DIS_DEBUG_BUILD)
	else()
		add_compile_definitions(LIBDANSDL2_DEBUG)
		add_compile_definitions(IS_DEBUG_BUILD)
	endif()
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

if(${BUILD_REGULAR})

	if(${CMAKE_VERSION} VERSION_LESS "3.22.0")
		add_definitions(-DAS_REGULAR)
	else()
		add_compile_definitions(AS_REGULAR)
	endif()
elseif(${BUILD_APPIMAGE})

	if(${CMAKE_VERSION} VERSION_LESS "3.22.0")
		add_definitions(-DAS_APPIMAGE)
	else()
		add_compile_definitions(AS_APPIMAGE)
	endif()
endif()

if(${WITH_LEXICAL_NORMALIZE})

	if(${CMAKE_VERSION} VERSION_LESS "3.22.0")
		add_definitions(-DWITH_LEXICAL_NORMALIZE)
	else()
		add_compile_definitions(WITH_LEXICAL_NORMALIZE)
	endif()

endif()

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#source and dependencies
include_directories("${CMAKE_CURRENT_LIST_DIR}/include")
add_subdirectory("${PROJECT_SOURCE_DIR}/src")

#add the framework dependencies
add_library(lm STATIC IMPORTED)
add_library(tools STATIC IMPORTED)
add_library(ldtools STATIC IMPORTED)
add_library(dansdl2 STATIC IMPORTED)
add_library(dfw STATIC IMPORTED)

set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.so)
set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm.so)

if(${BUILD_DEBUG})

	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2_debug.so)
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools_debug.so)
	set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw_debug.so)
else()

	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2.so)
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools.so)
	set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw.so)
endif()

#Virtual library so we don't have to build again and again for all tests...
add_library(applib STATIC ${SOURCE})
set_target_properties(applib PROPERTIES OUTPUT_NAME applib)

if(${BUILD_TESTS})

	add_executable(loader src/tests/animation_loader.cpp)
	target_link_libraries(loader applib tools ldtools dansdl2 dfw lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs)

	add_executable(saver src/tests/animation_saver.cpp)
	target_link_libraries(saver applib tools ldtools dansdl2 dfw lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs)

endif()

#Configure the executable, we want it into the "build directory" and we want all resources copied
#to the build directory.
add_executable(animation_editor src/main.cpp)
set_target_properties(animation_editor PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
add_custom_command(TARGET animation_editor POST_BUILD COMMAND mkdir -p build/resources)
add_custom_command(TARGET animation_editor POST_BUILD COMMAND cp -r ../resources/* ./build/resources)
add_custom_command(TARGET animation_editor POST_BUILD COMMAND cp -r ../example ./build)
target_link_libraries(animation_editor applib dfw tools ldtools dansdl2 lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL stdc++fs)
