#
# Copyright (C) 2025 https://github.com/diroverflow
#
# This is free software. You can redistribute it and/or
# modify it under the terms of the GNU General Public License
# version 3 as published by the Free Software Foundation.
#
# This software is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY.
#

cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ai-at-once)

if(CMAKE_SYSTEM_NAME STREQUAL Linux)
    add_compile_options(-Wall -Werror -Wextra)

    option(${PROJECT_NAME}_BUILD_PORTABLE   "Build portable binaries"   OFF)

    if(${PROJECT_NAME}_BUILD_PORTABLE)
        set(CMAKE_INSTALL_RPATH "\$ORIGIN")
        set(CMAKE_BUILD_WITH_INSTALL_RPATH ON)
    endif()

    ### wxWidgets ###
    # Defines
    add_definitions(-D__WXGTK__)
    # Suppress warnings 
    add_compile_options(-Wno-deprecated-copy)
    add_compile_options(-Wno-ignored-qualifiers)
    #################

elseif(CMAKE_SYSTEM_NAME STREQUAL Windows)
    add_definitions(-D__PRETTY_FUNCTION__=__FUNCSIG__)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    set(wxUSE_WEBVIEW_EDGE ON)
    ### wxWidgets ###
    # Support find_package(wxWidgets REQUIRED) as prebuilt
    set(wxWidgets_ROOT_DIR D:/Research/wxwidgets-master)
    set(wxWidgets_LIB_DIR ${CMAKE_SOURCE_DIR}/prebuilt/wxwidgets/lib)
    set(wxWidgets_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/prebuilt/wxwidgets/include)
    # Defines
    add_definitions(-DWXUSINGDLL)
    # Link options
    add_link_options(/SUBSYSTEM:WINDOWS)
    #################
endif()

find_package(wxWidgets REQUIRED COMPONENTS core base aui webview)

file(GLOB SRC_FILES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${PROJECT_NAME}.rc)

target_include_directories(${PROJECT_NAME}
    PRIVATE src
    PRIVATE ${wxWidgets_INCLUDE_DIRS}
    )

target_link_libraries(${PROJECT_NAME}
    PRIVATE ${wxWidgets_LIBRARIES}
    )

