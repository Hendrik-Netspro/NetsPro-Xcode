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

    std::string pythonCmd;
#if defined(_WIN32) || defined(_WIN64)
    pythonCmd = "py -3";
#else
    pythonCmd = "python3";
#endif

    std::string command = pythonCmd + " -u \"" + scriptPath + "\" --stream \"" + input + "\"";
#if defined(_WIN32) || defined(_WIN64)
    command += " 2>NUL";
#else
    command += " 2>/dev/null";
#endif
    std::string result;

    FILE* pipe = nullptr;
#if defined(_WIN32) || defined(_WIN64)
    pipe = _popen(command.c_str(), "r");
#else
    pipe = popen(command.c_str(), "r");
#endif
    if (!pipe) {
        return "JARVIS_ERROR: Could not start Python runtime.";
    }

    int ch;
    while ((ch = fgetc(pipe)) != EOF) {
        char c = static_cast<char>(ch);
        std::cout.put(c);
        std::cout.flush();
        result.push_back(c);
    }

#if defined(_WIN32) || defined(_WIN64)
    int exitCode = _pclose(pipe);
#else
    int exitCode = pclose(pipe);
#endif

    if (exitCode != 0) {
        return "JARVIS_ERROR: Local AI service is currently unavailable. Please verify Python, Ollama, and pulled models.";
    }

    if (result.empty()) {
        return "JARVIS_ERROR: No response from local AI service. Please check Ollama service status.";
    }

    return result;
}
