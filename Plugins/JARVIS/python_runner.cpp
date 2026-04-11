//
// Created by Hendrik Hanking on 06.04.26.
//

#include "python_runner.h"
#include <cstdio>
#include <iostream>
#include <string>

std::string runPython(const std::string& input) {
    std::string sourceFile = __FILE__;
    std::size_t pos = sourceFile.find_last_of("/\\");
    std::string scriptPath = (pos == std::string::npos)
                             ? "AI.py"
                             : sourceFile.substr(0, pos + 1) + "AI.py";

    std::string command = "python3 -u \"" + scriptPath + "\" --stream \"" + input + "\"";
    std::string result;

    FILE* pipe = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    pipe = _popen(command.c_str(), "r");
#else
    pipe = popen(command.c_str(), "r");
#endif
    if (!pipe) {
        return "Error while starting Python!";
    }

    int ch;
    while ((ch = fgetc(pipe)) != EOF) {
        char c = static_cast<char>(ch);
        std::cout.put(c);
        std::cout.flush();
        result.push_back(c);
    }

#if defined(_WIN32) || defined(_WIN64)
    _pclose(pipe);
#else
    pclose(pipe);
#endif
    return result;
}
