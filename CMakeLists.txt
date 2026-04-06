cmake_minimum_required(VERSION 4.1)
project(Netspro)

set(CMAKE_CXX_STANDARD 14)

add_executable(Netspro
        Netspro/main.cpp
        Plugins/JARVIS/python_runner.cpp
        Plugins/JARVIS/python_runner.h)
