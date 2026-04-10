//
// Created by Hendrik Hanking on 06.04.26.
//

#include "python_runner.h"
#include <cstdio>
#include <array>
#include <string>

std::string runPython(const std::string& input) {
    std::string sourceFile = __FILE__;
    std::size_t pos = sourceFile.find_last_of("/\\");
    std::string scriptPath = (pos == std::string::npos)
                             ? "AI.py"
                             : sourceFile.substr(0, pos + 1) + "AI.py";

    std::string command = "python3 \"" + scriptPath + "\" \"" + input + "\"";

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