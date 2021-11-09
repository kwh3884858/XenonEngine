cmake_minimum_required(VERSION 3.13)
Project(XenonEngine)
# aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} DIR_SRCS)
# file(GLOB DIR_SRCS *.h)

set(FLAGS "-DUNICODE" "-D_UNICODE")

# set_target_properties(yaml-cpp PROPERTIES
# IMPORTED_LOCATION_RELEASE ${CMAKE_CURRENT_BINARY_DIR}/Library/yaml-cpp/Release
# IMPORTED_LOCATION_DEBUG ${CMAKE_CURRENT_BINARY_DIR}/Library/yaml-cpp/Debug)
add_subdirectory(${PROJECT_SOURCE_DIR}/Library/yaml-cpp)
add_subdirectory(${PROJECT_SOURCE_DIR}/Library/crossguid)

IF (WIN32)
    message(STATUS "current platform: win32 ")
    add_executable(${PROJECT_NAME} WIN32)
    target_link_libraries(${PROJECT_NAME} yaml-cpp)
    target_include_directories(${PROJECT_NAME} PRIVATE yaml-cpp)

    target_link_libraries(${PROJECT_NAME} crossguid)
    target_include_directories(${PROJECT_NAME} PUBLIC crossguid)

    # add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD COMMAND ${CMAKE_COMMAND} -E )

    # find_library(STATIC_LIB1 yaml-cpp HINTS ${CMAKE_PREFIX_PATH}/lib)
    # IF(NOT STATIC_LIB1)
    #     message(FATAL_ERROR "Library YAML_CPP was not found!")
    # ELSE()
    #     target_include_directories( ${PROJECT_NAME} 
    #         PRIVATE
    #             ${yaml-cpp_INCLUDE_DIR})
    #     target_link_libraries(${PROJECT_NAME} yaml-cpp)
    # ENDIF()


target_compile_options(${PROJECT_NAME} PRIVATE "$<$<CONFIG:DEBUG>:${FLAGS}>" "$<$<CONFIG:RELEASE>:${FLAGS}>")

    # # resource compilation for mingw
    # set(CMAKE_RC_COMPILER_INIT windres)
    # ENABLE_LANGUAGE(RC)
    # SET(CMAKE_RC_COMPILE_OBJECT
    # "<CMAKE_RC_COMPILER> <FLAGS> <DEFINES> -o <OBJECT> <SOURCE>")
    # set(RC_CFLAGS "-DMINGW -Ocoff")
    # # If any sort of debugging is being enabled, add a _DEBUG define to the flags for the resource compiler
    # #if(CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
    # #   set(RC_CFLAGS "${RC_CFLAGS} -D_DEBUG")
    # #endif(CMAKE_BUILD_TYPE STREQUAL "DEBUG" OR CMAKE_BUILD_TYPE STREQUAL "RELWITHDEBINFO")
    # set_source_files_properties(${CMAKE_SOURCE_DIR}/Windows/Resources/res.rc COMPILE_FLAGS "${RC_CFLAGS}")
    # # For anything else, assumingly Visual Studio at this point, use a different set of compile flags
    # # Setup common to both the shared and static variants.

ELSEIF (UNIX)
ENDIF()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)

target_include_directories( ${PROJECT_NAME} 
    PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Library/imgui
)

file(GLOB_RECURSE DIR_SRCS 
${PROJECT_SOURCE_DIR}/Algorithms/*.cpp 
${PROJECT_SOURCE_DIR}/Algorithms/*.h
${PROJECT_SOURCE_DIR}/CrossPlatform/*.cpp 
${PROJECT_SOURCE_DIR}/CrossPlatform/*.h
${PROJECT_SOURCE_DIR}/Engine/*.cpp 
${PROJECT_SOURCE_DIR}/Engine/*.h
${PROJECT_SOURCE_DIR}/Gameplay/*.cpp 
${PROJECT_SOURCE_DIR}/Gameplay/*.h
${PROJECT_SOURCE_DIR}/MathLab/*.cpp 
${PROJECT_SOURCE_DIR}/MathLab/*.h

    $<$<BOOL:${WIN32}>:
    # for Windows
    ${PROJECT_SOURCE_DIR}/Windows/XenonEngineNatvisFile.natvis
    ${PROJECT_SOURCE_DIR}/MacWindow.licenseheader
    ${PROJECT_SOURCE_DIR}/Windows/*.cpp 
    ${PROJECT_SOURCE_DIR}/Windows/*.h
    >

    $<$<NOT:$<BOOL:${WIN32}>>:
    # for POSIX
    >
    )
target_sources(${PROJECT_NAME} PUBLIC ${DIR_SRCS})

file(
    GLOB 
    DIR_IMGUI 
    ${PROJECT_SOURCE_DIR}/Library/imgui/*.cpp 
    ${PROJECT_SOURCE_DIR}/Library/imgui/*.h
    ${PROJECT_SOURCE_DIR}/Library/imgui/backends/imgui_impl_dx11.h
    ${PROJECT_SOURCE_DIR}/Library/imgui/backends/imgui_impl_dx11.cpp
    ${PROJECT_SOURCE_DIR}/Library/imgui/backends/imgui_impl_win32.h
    ${PROJECT_SOURCE_DIR}/Library/imgui/backends/imgui_impl_win32.cpp
    ${PROJECT_SOURCE_DIR}/Library/imgui/*.cpp 
    ${PROJECT_SOURCE_DIR}/Library/imgui/*.h
    )
target_sources(${PROJECT_NAME} PUBLIC ${DIR_IMGUI})

file(
    GLOB_RECURSE
    DIR_IMGUI_FILE_DIALOG
    ${PROJECT_SOURCE_DIR}/Library/ImGuiFileDialog/*.h
    ${PROJECT_SOURCE_DIR}/Library/ImGuiFileDialog/*.cpp
    )
target_sources(${PROJECT_NAME} PUBLIC ${DIR_IMGUI_FILE_DIALOG})

message("PROJECT_SOURCE_DIR" ${PROJECT_SOURCE_DIR})
message("DEBUG_POSTFIX" ${DEBUG_POSTFIX})
message("CMAKE_CURRENT_BINARY_DIR" ${CMAKE_CURRENT_BINARY_DIR})
message("CMAKE_BINARY_DIR" ${CMAKE_BINARY_DIR})
message("CMAKE_INSTALL_INCLUDEDIR" ${CMAKE_INSTALL_INCLUDEDIR})
message("CMAKE_INSTALL_PREFIX: "  ${CMAKE_INSTALL_PREFIX})
message("CMAKE_INSTALL_BINDIR: "  ${CMAKE_INSTALL_BINDIR})
message("CMAKE_INSTALL_LIBDIR: "  ${CMAKE_INSTALL_LIBDIR})
message("CMAKE_INSTALL_DATADIR: "  ${CMAKE_INSTALL_DATADIR})




# add_subdirectory(Library/imgui)