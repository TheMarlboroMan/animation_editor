add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/controller")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/app")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/dfwimpl")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/tools")
add_subdirectory("${CMAKE_CURRENT_LIST_DIR}/env")

set(SOURCE ${SOURCE} PARENT_SCOPE)
