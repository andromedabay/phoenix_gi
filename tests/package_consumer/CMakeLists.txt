cmake_minimum_required(VERSION 3.16)

project(wx_package_consumer LANGUAGES CXX)

find_package(OpenGL REQUIRED)
find_package(wxWidgets CONFIG REQUIRED)

if(NOT TARGET wx::mono)
    message(FATAL_ERROR "The installed wxWidgets package does not export wx::mono")
endif()

add_executable(wx_package_consumer main.cpp)
target_link_libraries(wx_package_consumer PRIVATE wx::mono)