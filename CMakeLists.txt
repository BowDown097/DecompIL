cmake_minimum_required(VERSION 3.16)
project(DecompIL
    VERSION 0.1
    DESCRIPTION "Yet another .NET decompiler."
    HOMEPAGE_URL "https://github.com/BowDown097/DecompIL"
    LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(DECOMPIL_APP_DESC "${CMAKE_PROJECT_DESCRIPTION}")
set(DECOMPIL_APP_NAME "${CMAKE_PROJECT_NAME}")
set(DECOMPIL_VERSION_NAME "${CMAKE_PROJECT_VERSION}")

if(WIN32)
    set(BINARY_INTERFACE_NAME "DotnetBinaryInterface.dll")
elseif(APPLE)
    set(BINARY_INTERFACE_NAME "DotnetBinaryInterface.dylib")
else()
    set(BINARY_INTERFACE_NAME "DotnetBinaryInterface.so")
endif()

find_package(Qt6 REQUIRED COMPONENTS Widgets)

if(APPLE)
    set(MACOSX_BUNDLE_BUNDLE_NAME "${DECOMPIL_APP_NAME}")
    set(MACOSX_BUNDLE_BUNDLE_VERSION "${DECOMPIL_VERSION_NAME}")
    set(MACOSX_BUNDLE_INFO_STRING "${DECOMPIL_APP_NAME}: ${DECOMPIL_APP_DESC}")
    set(MACOSX_BUNDLE_LONG_VERSION_STRING "${DECOMPIL_VERSION_NAME}")
    set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${DECOMPIL_VERSION_NAME}")
endif()

set(SOURCE_FILES
    src/interface.cpp
    src/libraryhandle.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/richtextitemdelegate.cpp
    src/uiutils.cpp
    src/widgets/assemblytreeitem.cpp
    src/widgets/typetreeitem.cpp
    src/widgets/codeeditor/codeeditor.cpp
    src/widgets/codeeditor/codeeditordefinitions.cpp
    src/widgets/codeeditor/codeeditorsidebar.cpp)

set(HEADERS
    src/interface.h
    src/libraryhandle.h
    src/mainwindow.h
    src/richtextitemdelegate.h
    src/uiutils.h
    src/interface_types/marshal/marshalexception.h
    src/interface_types/marshal/marshalexpected.h
    src/interface_types/marshal/marshallist.h
    src/interface_types/marshal/marshalnullable.h
    src/interface_types/marshal/marshalstring.h
    src/interface_types/marshal/assemblytypes/assemblycoffheader.h
    src/interface_types/marshal/assemblytypes/assemblycommonattributes.h
    src/interface_types/marshal/assemblytypes/assemblycorheader.h
    src/interface_types/marshal/assemblytypes/assemblyinfo.h
    src/interface_types/marshal/assemblytypes/assemblymetadata.h
    src/interface_types/marshal/assemblytypes/assemblypeheader.h
    src/interface_types/marshal/assemblytypes/assemblypeheaders.h
    src/interface_types/marshal/assemblytypes/assemblysectionheader.h
    src/interface_types/marshal/assemblytypes/assemblytypemetadata.h
    src/interface_types/marshal/assemblytypes/shared.h
    src/interface_types/native/assemblycommonattributes.h
    src/interface_types/native/assemblyinfo.h
    src/interface_types/native/assemblymetadata.h
    src/interface_types/native/assemblypeheaders.h
    src/interface_types/native/assemblysectionheader.h
    src/interface_types/native/assemblytypemetadata.h
    src/views_concat/concat.hpp
    src/views_concat/utils.hpp
    src/widgets/assemblytreeitem.h
    src/widgets/typetreeitem.h
    src/widgets/codeeditor/codeeditor.h
    src/widgets/codeeditor/codeeditordefinitions.h
    src/widgets/codeeditor/codeeditorsidebar.h)

set(FORMS
    src/mainwindow.ui)

if(WIN32)
    add_executable(${CMAKE_PROJECT_NAME} WIN32 ${SOURCE_FILES} ${HEADERS} ${FORMS})
elseif(APPLE)
    add_executable(${CMAKE_PROJECT_NAME} MACOSX_BUNDLE ${SOURCE_FILES} ${HEADERS} ${FORMS})
else()
    add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} ${HEADERS} ${FORMS})
endif()

execute_process(COMMAND git rev-parse --abbrev-ref HEAD OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE DECOMPIL_BRANCH)
execute_process(COMMAND git rev-parse --short HEAD OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE DECOMPIL_COMMIT_ID)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    DECOMPIL_APP_DESC="${DECOMPIL_APP_DESC}"
    DECOMPIL_APP_NAME="${DECOMPIL_APP_NAME}"
    DECOMPIL_BRANCH="${DECOMPIL_BRANCH}"
    DECOMPIL_COMMIT_ID="${DECOMPIL_COMMIT_ID}"
    DECOMPIL_REPO_URL="${CMAKE_PROJECT_HOMEPAGE_URL}"
    DECOMPIL_VERSION_NAME="${DECOMPIL_VERSION_NAME}"
    DECOMPIL_BINARY_INTERFACE_NAME="${BINARY_INTERFACE_NAME}")

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE src)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE Qt::Widgets)

find_package(KF6SyntaxHighlighting REQUIRED)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE KF6::SyntaxHighlighting)

include(cmake/DotnetPublish.cmake)
DotnetPublish(${CMAKE_PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/DotnetBinaryInterface/DotnetBinaryInterface.csproj "Building C# library")

if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

install(TARGETS ${CMAKE_PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
