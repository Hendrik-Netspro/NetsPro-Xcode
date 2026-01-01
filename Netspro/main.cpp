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
std::string hole_seriennummer();

//ende
//variablen
std::string input;
std::string version = "1.0.0a";
std::string seriennummer;

//ende
int main() {
    clear_terminal();
    seriennummer = hole_seriennummer();
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

std::string hole_seriennummer() {
    std::string result;
#if defined(_WIN32) || defined(_WIN64)
    char buffer[128];
    FILE* pipe = _popen("wmic bios get serialnumber", "r");
    if (!pipe) return "Unbekannt";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::string line(buffer);
        if (line.find("SerialNumber") == std::string::npos && !line.empty()) {
            // Bereinige Zeilenumbruch
            line.erase(line.find_last_not_of(" \n\r") + 1);
            if (!line.empty()) result = line;
        }
    }
    _pclose(pipe);
#elif defined(__APPLE__)
    char buffer[256];
    FILE* pipe = popen("system_profiler SPHardwareDataType | awk '/Serial/ {print $4}'", "r");
    if (!pipe) return "Unbekannt";
    if (fgets(buffer, sizeof(buffer), pipe)) {
        result = buffer;
        // Entferne evtl. Zeilenumbruch
        result.erase(result.find_last_not_of(" \n\r") + 1);
    }
    pclose(pipe);
#elif defined(__linux__)
    char buffer[256];
    FILE* pipe = popen("cat /sys/class/dmi/id/product_serial", "r");
    if (!pipe) return "Unbekannt";
    if (fgets(buffer, sizeof(buffer), pipe)) {
        result = buffer;
        result.erase(result.find_last_not_of(" \n\r") + 1);
    }
    pclose(pipe);
#else
    result = "Unbekannt";
#endif
    return result.empty() ? "Unbekannt" : result;
}

void clear_terminal() {
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #else
        std::system("clear");
    #endif
}
void eingabe_abfragung() {
    std::cout << "NETSPECTREPROTERMINEL@" << seriennummer << ">>>" ;
    std::getline(std::cin, input);
}


void handel_input() {
    if (input == "info") {
        std::cout << "NetSpectre Pro Terminel cpp edition" << std::endl;
        std::cout << "V." << version << std::endl;
        std::cout << "Seriennummer: " << seriennummer << std::endl;
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


