//
// Created by Hendrik Hanking on 06.04.26.
//
#include <iostream>
#include "python_runner.h"

int main() {
    std::string input;

    std::cout << "Du: ";
    std::getline(std::cin, input);

    std::string antwort = runPython(input);

    std::cout << antwort << std::endl;

    return 0;
}