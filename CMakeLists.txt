cmake_minimum_required(VERSION 3.16 FATAL_ERROR)

if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
endif()

project(phoenix_gi VERSION 1.0.0 LANGUAGES C CXX)

include(GNUInstallDirs)

option(WXBGI_BUNDLE_WXWIDGETS
    "Build and package a relocatable wxWidgets CMake SDK with phoenix_gi."
    OFF
)

if(NOT DEFINED WXBGI_ENABLE_CLEAN_TARGET)
    option(WXBGI_ENABLE_CLEAN_TARGET
        "Add an opt-in clean target that removes build* directories and third_party/installed contents except .gitignore."
        OFF
    )
endif()

if(WXBGI_ENABLE_CLEAN_TARGET)
    include(cmake_files/09-CleanTargets.cmake)
    add_custom_target(clean-all
        COMMAND ${CMAKE_COMMAND} -DPROJECT_ROOT=${CMAKE_SOURCE_DIR} -P ${CMAKE_SOURCE_DIR}/cmake_files/09-CleanTargets.cmake
        COMMENT "Remove generated build directories and installed third-party artifacts while preserving .gitignore files"
        VERBATIM
    )
    return()
endif()

include(cmake_files/01-Options.cmake)
if(WXBGI_BUNDLE_WXWIDGETS AND NOT WXBGI_ENABLE_WX)
    message(FATAL_ERROR "WXBGI_BUNDLE_WXWIDGETS requires WXBGI_ENABLE_WX=ON")
endif()

include(cmake_files/02-ThirdParty.cmake)
include(cmake_files/03-IncludeTargets.cmake)
include(cmake_files/04-LibraryTargets.cmake)
include(cmake_files/05-ExamplesTargets.cmake)
include(cmake_files/06-Docs.cmake)
include(cmake_files/07-Tests.cmake)
include(cmake_files/08-Packaging.cmake)
