cmake_minimum_required(VERSION 3.20)
project(Netspro)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# macOS: automatisch Architektur des aktuellen Rechners verwenden
if(APPLE)
    execute_process(
            COMMAND uname -m
            OUTPUT_VARIABLE HOST_ARCH
            OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    if(HOST_ARCH STREQUAL "arm64")
        set(CMAKE_OSX_ARCHITECTURES "arm64" CACHE STRING "Build architecture for macOS" FORCE)
    elseif(HOST_ARCH STREQUAL "x86_64")
        set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Build architecture for macOS" FORCE)
    endif()
endif()

add_executable(Netspro
        Netspro/main.cpp
        Plugins/JARVIS/python_runner.cpp
        "Plugins/Config editor/json_editor.cpp"
        "Plugins/Config editor/json_editor.h"
)

set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/release")

set_target_properties(Netspro PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}"
)

# Nach Build: Zielordner erstellen und Python-Dateien kopieren
add_custom_command(TARGET Netspro POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${OUTPUT_DIR}/Plugins/JARVIS"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Plugins/JARVIS/AI.py"
        "${OUTPUT_DIR}/Plugins/JARVIS/AI.py"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Plugins/JARVIS/Tools.py"
        "${OUTPUT_DIR}/Plugins/JARVIS/Tools.py"
)