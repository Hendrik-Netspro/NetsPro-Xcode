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

//ende
//variablen
bool DEV_MODE;

//ende


void clear_terminal() {
    std::cout << "\x1b[2J\x1b[H";
    std::cout.flush();
}
void eingabe_abfragung(std::string& name, bool mode) {
    if (mode == true) {
        
    }
}



int main() {
    std::cout << R"(Willcommen Bei NETSPECTRE PRO cpp
)";
    std::cout << R"(bis jetzt is Netspro cpp noch kostenlos
)";
    std::cout << R"(wenn du einen kostenlosen accout willst Gebe feedback.
)";
    std::cout << R"(Feedback an:   Hendrik.Hanking@icloud.com
)";
    std::cout << R"(   _  __    __    ____             __            ___  ___  ____
)";
    std::cout << R"(  / |/ /__ / /_  / __/__  ___ ____/ /________   / _ \/ _ \/ __ \
)";
    std::cout << R"( /    / -_) __/ _\ \/ _ \/ -_) __/ __/ __/ -_) / ___/ , _/ /_/ /
)";
    std::cout << R"(/_/|_/\__/\__/ /___/ .__/\__/\__/\__/_/  \__/ /_/  /_/|_|\____/
)";
    std::cout << R"(                  /_/                                           
)";
    
    return 0;
}


