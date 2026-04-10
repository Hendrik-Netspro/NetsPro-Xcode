cmake_minimum_required(VERSION 4.1)
project(Netspro)

set(CMAKE_CXX_STANDARD 14)

add_executable(Netspro
        Netspro/main.cpp
        Plugins/JARVIS/python_runner.cpp
)

set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/release)

# Executable in neuen Ordner legen
set_target_properties(Netspro PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
)

# Nach Build: Ordner erstellen + Python kopieren
add_custom_command(TARGET Netspro POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory
        "${OUTPUT_DIR}/Plugins/JARVIS"

        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/Plugins/JARVIS/AI.py"
        "${OUTPUT_DIR}/Plugins/JARVIS/AI.py"
)