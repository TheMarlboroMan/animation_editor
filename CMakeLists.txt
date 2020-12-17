cmake_minimum_required(VERSION 3.5.1)

project(animation_editor)

option(BUILD_TESTS "Build test code" ON)
option(BUILD_DEBUG "Build a debug release" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(${BUILD_DEBUG})
	set(CMAKE_BUILD_TYPE Debug)
else()
	set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)


include_directories("${CMAKE_CURRENT_LIST_DIR}/include/tile_editor")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include/dfw_impl")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include/input")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include/controller")
include_directories("${CMAKE_CURRENT_LIST_DIR}/include")

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

#add the framework dependencies
add_library(lm STATIC IMPORTED)
add_library(tools STATIC IMPORTED)
add_library(ldtools STATIC IMPORTED)
add_library(dansdl2 STATIC IMPORTED)
add_library(dfw STATIC IMPORTED)

#If the libraries are not installed, tell them where they are.
set_target_properties(tools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libtools.a)
set_target_properties(lm PROPERTIES IMPORTED_LOCATION /usr/local/lib/liblm.a)
set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdansdl2.a)
set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION /usr/local/lib/libldtools.a)
set_target_properties(dfw PROPERTIES IMPORTED_LOCATION /usr/local/lib/libdfw.a)

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
#add_executable(animation_editor src/main.cpp ${SOURCE})
#set_target_properties(animation_editor PROPERTIES RUNTIME_OUTPUT_DIRECTORY build)
#add_custom_command(TARGET animation_editor POST_BUILD COMMAND cp -r ../resources/* ./build)
#target_link_libraries(animation_editor dfw tools dansdl2 ldtools lm SDL2 SDL2_ttf SDL2_mixer SDL2_image GL)
