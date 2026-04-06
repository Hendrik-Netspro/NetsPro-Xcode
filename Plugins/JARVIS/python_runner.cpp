//
// Created by Hendrik Hanking on 06.04.26.
//

#include "python_runner.h"
#include <cstdio>
#include <array>
#include <string>

std::string runPython(const std::string& input) {
    std::string command = "python3 ai.py \"" + input + "\"";

    std::array<char, 256> buffer{};
    std::string result;

    FILE* pipe = popen(command.c_str(), "r");
    if (!pipe) {
        return "Fehler beim Starten von Python!";
    }

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr) {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}