//
//  main.cpp
//  Netspro
//
//  Created by Hendrik Hanking on 31.12.25.
//  COPYWEITE Hendrik Vincent Hanking 2025
//  Alle rechte vorberhaten

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include <cstdlib>   // system
#include <cstdio>    // std::perror

// ankündigungen
void clear_terminal();
void eingabe_abfragung();
void hauptbildschirm();
void handel_input();

//ende
//variablen
std::string input;
std::string version = "1.0.0a";

//ende
int main() {
    clear_terminal();
    hauptbildschirm();

    while (true) {
        eingabe_abfragung();
        if (input == "exit") {
            break;
        }
        handel_input();
    }

    return 0;
}



void clear_terminal() {
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #else
        std::system("clear");
    #endif
}
void eingabe_abfragung() {
    std::cout << "NETSPECTRETERMINEL (type 'exit' to quit):::>>";
    std::getline(std::cin, input);
}


void handel_input() {
    if (input == "info") {
        std::cout << "NetSpectre Pro Terminel cpp edition" << std::endl;
        std::cout << "V." << version << std::endl;
    }
    else if (input == "cls" || input == "clear") {
        clear_terminal();
    }
    else if (input == "exit") {
        // Do nothing here, main will handle exit
    }
    else {
        std::cout << "Den command haben wir nicht gefunden. :(" << std::endl;
    }
}


void hauptbildschirm() {
    std::cout << "Willcommen Bei NETSPECTRE PRO cpp" << std::endl;
    std::cout << "bis jetzt is Netspro cpp noch kostenlos" << std::endl;
    std::cout << "wenn du einen kostenlosen accout willst Gebe feedback." << std::endl;
    std::cout << "Feedback an:   Hendrik.Hanking@icloud.com." << std::endl;
    std::cout << R"(   _  __    __    ____             __            ___  ___  ____ )" << std::endl;
    std::cout << R"(  / |/ /__ / /_  / __/__  ___ ____/ /________   / _ \/ _ \/ __ \)" << std::endl;
    std::cout << R"( /    / -_) __/ _\ \/ _ \/ -_) __/ __/ __/ -_) / ___/ , _/ /_/ /)" << std::endl;
    std::cout << R"(/_/|_/\__/\__/ /___/ .__/\__/\__/\__/_/  \__/ /_/  /_/|_|\____/ )" << std::endl;
    std::cout << R"(C++ EDITION       /_/                                           )" << std::endl;
    std::cout << " " << std::endl;
}


