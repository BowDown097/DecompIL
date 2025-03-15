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
    src/decompilapplication.cpp
    src/main.cpp
    src/mainwindow.cpp
    src/settingsstore.cpp
    src/interface/interface.cpp
    src/interface/libraryhandle.cpp
    src/ui/richtextitemdelegate.cpp
    src/ui/settingsform.cpp
    src/ui/widgets/assemblytreeitem.cpp
    src/ui/widgets/assemblytreewidget.cpp
    src/ui/widgets/findbar.cpp
    src/ui/widgets/typetreeitem.cpp
    src/ui/widgets/codeeditor/codeeditor.cpp
    src/ui/widgets/codeeditor/codeeditordefinitions.cpp
    src/ui/widgets/codeeditor/codeeditorsidebar.cpp
    src/utils/stringutils.cpp
    src/utils/uiutils.cpp)

set(HEADERS
    src/decompilapplication.h
    src/languagemapping.h
    src/mainwindow.h
    src/settingsstore.h
    src/interface/interface.h
    src/interface/libraryhandle.h
    src/interface/types/marshal/decompilationinfo.h
    src/interface/types/marshal/marshalexception.h
    src/interface/types/marshal/marshalexpected.h
    src/interface/types/marshal/marshallist.h
    src/interface/types/marshal/marshalnullable.h
    src/interface/types/marshal/marshalstring.h
    src/interface/types/marshal/assemblytypes/assemblycoffheader.h
    src/interface/types/marshal/assemblytypes/assemblycommonattributes.h
    src/interface/types/marshal/assemblytypes/assemblycorheader.h
    src/interface/types/marshal/assemblytypes/assemblyinfo.h
    src/interface/types/marshal/assemblytypes/assemblymetadata.h
    src/interface/types/marshal/assemblytypes/assemblypeheader.h
    src/interface/types/marshal/assemblytypes/assemblypeheaders.h
    src/interface/types/marshal/assemblytypes/assemblysectionheader.h
    src/interface/types/marshal/assemblytypes/assemblytypemetadata.h
    src/interface/types/marshal/assemblytypes/shared.h
    src/interface/types/native/assemblycommonattributes.h
    src/interface/types/native/assemblyinfo.h
    src/interface/types/native/assemblymetadata.h
    src/interface/types/native/assemblypeheaders.h
    src/interface/types/native/assemblysectionheader.h
    src/interface/types/native/assemblytypemetadata.h
    src/ui/richtextitemdelegate.h
    src/ui/settingsform.h
    src/ui/widgets/assemblytreeitem.h
    src/ui/widgets/assemblytreewidget.h
    src/ui/widgets/findbar.h
    src/ui/widgets/typetreeitem.h
    src/ui/widgets/codeeditor/codeeditor.h
    src/ui/widgets/codeeditor/codeeditordefinitions.h
    src/ui/widgets/codeeditor/codeeditorsidebar.h
    src/utils/stringutils.h
    src/utils/uiutils.h
    src/utils/views_concat/concat.hpp
    src/utils/views_concat/utils.hpp)

set(FORMS
    src/mainwindow.ui
    src/ui/settingsform.ui
    src/ui/widgets/findbar.ui)

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

find_package(KF6SyntaxHighlighting QUIET)
if(NOT KF6SyntaxHighlighting_FOUND)
    set(BUILD_TESTING OFF CACHE BOOL "")
    set(BUILD_codeeditor OFF CACHE BOOL "")
    set(BUILD_codepdfprinter OFF CACHE BOOL "")
    set(BUILD_minimaltest OFF CACHE BOOL "")

    include(FetchContent)
    FetchContent_Declare(
        KF6SyntaxHighlighting
        GIT_REPOSITORY https://invent.kde.org/frameworks/syntax-highlighting.git
        GIT_TAG v6.11.0)
    FetchContent_MakeAvailable(KF6SyntaxHighlighting)

    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE KF6SyntaxHighlighting)
else()
    target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE KF6::SyntaxHighlighting)
endif()

include(cmake/DotnetPublish.cmake)
DotnetPublish(${CMAKE_PROJECT_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/DotnetBinaryInterface/DotnetBinaryInterface.csproj "Building C# library")

include(GNUInstallDirs)

if(WIN32)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

install(
    FILES $<TARGET_FILE_DIR:DecompIL>/${BINARY_INTERFACE_NAME}
    DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(TARGETS ${CMAKE_PROJECT_NAME}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
