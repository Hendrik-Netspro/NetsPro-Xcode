//
//  main.cpp
//  Netspro
//
//  Created by Hendrik Hanking on 31.12.25.
//  COPYRIGHT Hendrik Vincent Hanking 2025
//  All rights reserved

#include <chrono>
#include <iostream>
#include <thread>
#include <algorithm>

// declarations
void terminalLeeren();
void benutzereingabeAbfragen();
void hauptbildschirm();
void verarbeiteEingabe();
std::string holeSeriennummer();
void anmeldung();
void tokenInitialisierer();
void logo();

// end
// variables
std::string eingabe;
std::string version = "1.0.0a";
std::string seriennummer;
std::string benutzername;
std::string passwort;
std::string gebildetesToken;

// end
// user tokens
std::vector<std::string> tokenListe; // create list

// end

int main() {
    terminalLeeren();
    tokenInitialisierer();
    seriennummer = holeSeriennummer();
    anmeldung();
    while (true) {
        benutzereingabeAbfragen();
        if (eingabe == "exit") {
            break;
        }
        verarbeiteEingabe();
    }

    return 0;
}

std::string holeSeriennummer() {
    std::string result;
#if defined(_WIN32) || defined(_WIN64)
    char buffer[128];
    FILE* pipe = _popen("wmic bios get serialnumber", "r");
    if (!pipe) return "Unknown";
    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        std::string line(buffer);
        if (line.find("SerialNumber") == std::string::npos && !line.empty()) {
            // Clean newline
            line.erase(line.find_last_not_of(" \n\r") + 1);
            if (!line.empty()) result = line;
        }
    }
    _pclose(pipe);
#elif defined(__APPLE__)
    char buffer[256];
    FILE* pipe = popen("system_profiler SPHardwareDataType | awk '/Serial/ {print $4}'", "r");
    if (!pipe) return "Unknown";
    if (fgets(buffer, sizeof(buffer), pipe)) {
        result = buffer;
        // Remove possible newline
        result.erase(result.find_last_not_of(" \n\r") + 1);
    }
    pclose(pipe);
#elif defined(__linux__)
    char buffer[256];
    FILE* pipe = popen("cat /sys/class/dmi/id/product_serial", "r");
    if (!pipe) return "Unknown";
    if (fgets(buffer, sizeof(buffer), pipe)) {
        result = buffer;
        result.erase(result.find_last_not_of(" \n\r") + 1);
    }
    pclose(pipe);
#else
    result = "Unknown";
#endif
    return result.empty() ? "Unknown" : result;
}

void terminalLeeren() {
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

void benutzereingabeAbfragen() {
    std::cout << "NETSPECTREPROTERMINAL@" << seriennummer << ">>> ";
    std::getline(std::cin, eingabe);
}


void verarbeiteEingabe() {
    if (eingabe == "info") {
        std::cout << "NetSpectre Pro Terminal C++ edition" << std::endl;
        std::cout << "V." << version << std::endl;
        std::cout << "Serial number: " << seriennummer << std::endl;
    }
    else if (eingabe == "cls" || eingabe == "clear") {
        terminalLeeren();
    }
    else if (eingabe == "exit") {
        // Do nothing here, main will handle exit
    }
    else {
        std::cout << "Command not found :(" << std::endl;
    }
}

void tokenInitialisierer() {
    tokenListe.emplace_back("Laurenz.Flecki66");
    tokenListe.emplace_back("Hendrik.Hoppel10");
}


void anmeldung() {
    logo();
    std::cout << std::endl;
    std::cout << "Username: ";
    std::getline(std::cin, benutzername);
    terminalLeeren();
    
    std::cout << R"(   _  __    __    ____             __            ___  ___  ____ )" << std::endl;
    std::cout << R"(  / |/ /__ / /_  / __/__  ___ ____/ /________   / _ \/ _ \/ __ \)" << std::endl;
    std::cout << R"( /    / -_) __/ _\ \/ _ \/ -_) __/ __/ __/ -_) / ___/ , _/ /_/ /)" << std::endl;
    std::cout << R"(/_/|_/\__/\__/ /___/ .__/\__/\__/\__/_/  \__/ /_/  /_/|_|\____/ )" << std::endl;
    std::cout << R"(C++ EDITION       /_/                                           )" << std::endl;
    std::cout << std::endl;
    std::cout << "Password for " << benutzername << ": ";
    std::getline(std::cin, passwort);
    gebildetesToken = benutzername + "." + passwort;
    if (std::find(tokenListe.begin(), tokenListe.end(), gebildetesToken) != tokenListe.end()) {
        hauptbildschirm();
    } else {
        std::cout << "Username or password not recognized." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(4));
        terminalLeeren();
        anmeldung();
    }
}





void hauptbildschirm() {
    std::cout << "Welcome to NETSPECTRE PRO C++" << std::endl;
    std::cout << "Feedback: Hendrik.Hanking@icloud.com." << std::endl;
    std::cout << R"(   _  __    __    ____             __            ___  ___  ____ )" << std::endl;
    std::cout << R"(  / |/ /__ / /_  / __/__  ___ ____/ /________   / _ \/ _ \/ __ \)" << std::endl;
    std::cout << R"( /    / -_) __/ _\ \/ _ \/ -_) __/ __/ __/ -_) / ___/ , _/ /_/ /)" << std::endl;
    std::cout << R"(/_/|_/\__/\__/ /___/ .__/\__/\__/\__/_/  \__/ /_/  /_/|_|\____/ )" << std::endl;
    std::cout << R"(C++ EDITION       /_/                                           )" << std::endl;
    std::cout << " " << std::endl;
}

void logo() {
    std::cout << R"(   _  __    __    ____             __            ___  ___  ____ )" << std::endl;
    std::cout << R"(  / |/ /__ / /_  / __/__  ___ ____/ /________   / _ \/ _ \/ __ \)" << std::endl;
    std::cout << R"( /    / -_) __/ _\ \/ _ \/ -_) __/ __/ __/ -_) / ___/ , _/ /_/ /)" << std::endl;
    std::cout << R"(/_/|_/\__/\__/ /___/ .__/\__/\__/\__/_/  \__/ /_/  /_/|_|\____/ )" << std::endl;
    std::cout << R"(C++ EDITION       /_/                                           )" << std::endl;

}


