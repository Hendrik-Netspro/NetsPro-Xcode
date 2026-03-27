//
//  main.cpp
//  Netspro
//
//  Created by Hendrik Hanking on 31.12.25.
//  COPYRIGHT Hendrik Vincent Hanking 2025
//  All rights reserved
//

#include <chrono>
#include <iostream>
#include <thread>
#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <random>
#include <iomanip>

// ===================== Declarations =====================
void terminalLeeren();
void benutzereingabeAbfragen();
void hauptbildschirm();
void verarbeiteEingabe();
std::string holeSeriennummer();
void anmeldung();
void tokenInitialisierer();
void logo();
bool dev_mode = false;
bool developer = false;
// AkteViewer
void fake_loading(double zeit);
void editor_interaktiv();
void editor_datei(const std::string& dateiname);
std::string neue_datei();
// Crypto / File Utils
std::string verschluesseln(const std::string& text);
std::string entschluesseln(const std::string& text);
std::string buchstaben_verschieben(const std::string& eingabe, const std::vector<int>& verschiebungen);
std::string buchstaben_entschluesseln(const std::string& text, const std::vector<int>& verschiebungen);
std::string trim(const std::string& s);
std::string to_lower(std::string s);
std::string to_upper(std::string s);
std::string remove_spaces(const std::string& s);
std::string read_file_utf8(const std::string& path);
void write_file_utf8(const std::string& path, const std::string& content);
std::string build_initials(const std::string& vollername);
std::string shuffle_string(std::string s);
long long random_int64(long long lo, long long hi);
std::mt19937& rng();

// ===================== Variables =====================
std::string eingabe;
std::string version = "DEV1.1.1.3.ax20262A";
std::string short_version = "DEV1.1.1.3";
std::string seriennummer;
std::string benutzername;
std::string passwort;
std::string gebildetesToken;

// tokens
std::vector<std::string> userTokens;
std::vector<std::string> devTokens;

static const std::vector<int> SHIFT = {9, 9, 4, 13, 2, 10, 3};




// ===================== Main ============================================================  BEGINN des codes
int main() {
    terminalLeeren();
    tokenInitialisierer();
    seriennummer = holeSeriennummer();
    anmeldung();

    while (true) {
        benutzereingabeAbfragen();
        if (trim(eingabe) == "exit") break;
        verarbeiteEingabe();
    }

    return 0;
}

// ===================== System Infos =====================
std::string holeSeriennummer() {
    std::string result;


#if defined(_WIN32) || defined(_WIN64)


    char buffer[256];

    // moderner Ersatz für WMIC
    const char* cmd =
    "powershell -NoProfile -Command \"(Get-CimInstance Win32_BIOS).SerialNumber\"";

    FILE* pipe = _popen(cmd, "r");
    if (!pipe) return "Unknown";

    if (fgets(buffer, sizeof(buffer), pipe)) {
        result = buffer;
        result.erase(result.find_last_not_of(" \n\r\t") + 1);
    }

    _pclose(pipe);

#elif defined(__APPLE__)

    char buffer[256];
    FILE* pipe = popen(
        "system_profiler SPHardwareDataType | awk '/Serial/ {print $4}'", "r"
    );

    if (!pipe) return "Unknown";

    if (fgets(buffer, sizeof(buffer), pipe)) {
        result = buffer;
        result.erase(result.find_last_not_of(" \n\r\t") + 1);
    }

    pclose(pipe);

#elif defined(__linux__)

    char buffer[256];
    FILE* pipe = popen("cat /sys/class/dmi/id/product_serial", "r");

    if (!pipe) return "Unknown";

    if (fgets(buffer, sizeof(buffer), pipe)) {
        result = buffer;
        result.erase(result.find_last_not_of(" \n\r\t") + 1);
    }
#else

    result = "Unknown";

#endif

    if (result.empty())
        result = "Unknown";

    return result;
}

void terminalLeeren() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#else
    std::system("clear");
#endif
}

void benutzereingabeAbfragen() {
    std::cout << "NETSPECTREPROCPP@" << seriennummer;
    if (dev_mode) std::cout << "[DEV]";
    std::cout << ">>> ";
    std::getline(std::cin, eingabe);
}

// ===================== Command Handling =====================
static void help() {
    std::cout << "\nCommands:\n";
    std::cout << "  info                - Version/Serial\n";
    std::cout << "  cls | clear          - Terminal leeren\n";
    std::cout << "  akte                - AkteViewer (fragt Datei/neu)\n";
    std::cout << "  akte neu            - neue Akte erstellen + öffnen\n";
    std::cout << "  akte open <Name>    - <Name>.persondata öffnen\n";
    std::cout << "  help                - diese Hilfe\n";
    std::cout << "  exit                - beenden\n";
    std::cout << "\nDev Commands:\n";
    std::cout << "  devinfo             - zeigt Dev-Status\n";
    std::cout << "  devmode on|off      - (nur DEV) toggle\n\n";
}

static bool starts_with(const std::string& s, const std::string& prefix) {
    return s.rfind(prefix, 0) == 0;
}

void verarbeiteEingabe() {
    std::string cmd = trim(eingabe);

    if (cmd == "info") {
        std::cout << "NetSpectre Pro Terminal C++ edition\n";
        std::cout << "V." << version << "\n";
        std::cout << "Serial number: " << seriennummer << "\n";
        std::cout << "Dev mode: " << (dev_mode ? "true" : "false") << "\n";
    }
    else if (cmd == "cls" || cmd == "clear") {
        terminalLeeren();
    }
    else if (cmd == "help") {
        help();
    }
    else if (cmd == "akte") {
        editor_interaktiv();
        terminalLeeren();
        hauptbildschirm();
    }
    else if (cmd == "akte neu") {
        std::string dateiname = neue_datei();
        editor_datei(dateiname);
        terminalLeeren();
        hauptbildschirm();
    }
    else if (starts_with(cmd, "akte open ")) {
        std::string name = trim(cmd.substr(std::string("akte open ").size()));
        if (name.empty()) {
            std::cout << "ERROR: Bitte Dateiname angeben.\n";
            return;
        }
        editor_datei(name);
        terminalLeeren();
        hauptbildschirm();
    }
    else if (cmd == "devinfo") {
        std::cout << "Developer mode is " << (dev_mode ? "ON" : "OFF") << "\n";
        std::cout << "User: " << benutzername << "\n";
    }
    else if (starts_with(cmd, "devmode ")) {
        std::string arg = to_lower(trim(cmd.substr(std::string("devmode ").size())));
        if (!developer) {
            std::cout << "Access denied (DEV only).\n";
            return;
        }
        if (arg == "on") dev_mode = true;
        else if (arg == "off") dev_mode = false;
        else std::cout << "Usage: devmode on|off\n";
    }
    else if (cmd == "exit") {
        // main handles exit
    }
    else {
        std::cout << "Command not found :(\n";
        std::cout << "Tipp: 'help'\n";
    }
}

// ===================== Login =====================
void tokenInitialisierer() {
    // normale Nutzer
    userTokens.emplace_back("Laurenz.Flecki66");
    userTokens.emplace_back("Hendrik.Hoppel10");
    userTokens.emplace_back("Joerg.hamburg");

    // Entwickler
    devTokens.emplace_back("Hendrik_dev.Hoppel10");
}

void anmeldung() {
    logo();
    std::cout << "\nUsername: ";
    std::getline(std::cin, benutzername);
    terminalLeeren();

    logo();
    std::cout << "\nPassword for " << benutzername << ": ";
    std::getline(std::cin, passwort);

    gebildetesToken = benutzername + "." + passwort;

    if (std::find(devTokens.begin(), devTokens.end(), gebildetesToken) != devTokens.end()) {
        dev_mode = true;
        developer = true;
        terminalLeeren();
        std::cout << "[Developer Mode]\n";
        hauptbildschirm();
        return;
    }

    if (std::find(userTokens.begin(), userTokens.end(), gebildetesToken) != userTokens.end()) {
        dev_mode = false;
        developer = false;
        terminalLeeren();
        hauptbildschirm();
        return;
    }

    std::cout << "Username or password not recognized.\n";
    std::this_thread::sleep_for(std::chrono::seconds(4));
    terminalLeeren();
    anmeldung();
}

void hauptbildschirm() {
    if (dev_mode) {
        std::cout << "Version: " << version << "\n";
    }
    else {
        std::cout << "Welcome to NETSPECTRE PRO C++\n";
        std::cout << "Version: " << short_version << "\n";
        std::cout << "Feedback: Hendrik.Hanking@icloud.com.\n";
    }
    logo();
    std::cout << "\n";
}

void logo() {
    if (dev_mode) {
        std::cout << R"(   _  __    __    ____             __            ___  ___  ____ )" << "\n";
        std::cout << R"(  / |/ /__ / /_  / __/__  ___ ____/ /________   / _ \/ _ \/ __ \)" << "\n";
        std::cout << R"( /    / -_) __/ _\ \/ _ \/ -_) __/ __/ __/ -_) / ___/ , _/ /_/ /)" << "\n";
        std::cout << R"(/_/|_/\__/\__/ /___/ .__/\__/\__/\__/_/  \__/ /_/  /_/|_|\____/ )" << "\n";
        std::cout << R"(C++ EDITION       /_/                     Welcome developer! :) )" << "\n";
    }
    else {
        std::cout << R"(   _  __    __    ____             __            ___  ___  ____ )" << "\n";
        std::cout << R"(  / |/ /__ / /_  / __/__  ___ ____/ /________   / _ \/ _ \/ __ \)" << "\n";
        std::cout << R"( /    / -_) __/ _\ \/ _ \/ -_) __/ __/ __/ -_) / ___/ , _/ /_/ /)" << "\n";
        std::cout << R"(/_/|_/\__/\__/ /___/ .__/\__/\__/\__/_/  \__/ /_/  /_/|_|\____/ )" << "\n";
        std::cout << R"(C++ EDITION       /_/                                           )" << "\n";
    }
}

// ===================== AkteViewer - UX =====================
void fake_loading(double zeit) {
    const int steps = 10;
    double warten = zeit / steps;

    for (int fsteps = 0; fsteps <= steps; ++fsteps) {
        terminalLeeren();
        std::cout << std::string(fsteps, 'X') << std::string(steps - fsteps, '0') << "\n";
        auto ms = static_cast<int>(warten * 1000.0);
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

void editor_interaktiv() {
    terminalLeeren();
    std::string dateiname;
    std::cout << "Welche Datei (Name) soll geoeffnet werden? (Neu = neue Akte): ";
    std::getline(std::cin, dateiname);
    dateiname = trim(dateiname);

    if (to_lower(dateiname) == "neu") {
        dateiname = neue_datei();
    }

    editor_datei(dateiname);
}

void editor_datei(const std::string& dateiname) {
    if (trim(dateiname).empty()) {
        std::cout << "ERROR: Dateiname leer.\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return;
    }

    fake_loading(1);
    terminalLeeren();

    std::string fullname = dateiname + ".persondata";
    std::string content_encrypted;

    try {
        content_encrypted = read_file_utf8(fullname);
    } catch (...) {
        std::cout << "ERROR: Datei nicht gefunden: " << fullname << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));
        return;
    }

    std::string content_plain = entschluesseln(content_encrypted);

    terminalLeeren();
    std::cout << content_plain << "\n";
    std::cout << "---- Neue Eintraege (leer = speichern & beenden) ----\n";

    while (true) {
        std::string neuer_eintrag;
        std::cout << "> ";
        std::getline(std::cin, neuer_eintrag);

        if (trim(neuer_eintrag).empty()) {
            fake_loading(1);

            std::string verschluesselt_text = verschluesseln(content_plain);
            try {
                write_file_utf8(fullname, verschluesselt_text);
            } catch (...) {
                std::cout << "ERROR: Konnte Datei nicht speichern.\n";
                std::this_thread::sleep_for(std::chrono::seconds(2));
                return;
            }

            terminalLeeren();
            std::cout << "Gespeichert.\n";
            std::cout << "ENTER...";
            std::string dummy;
            std::getline(std::cin, dummy);
            return;
        }

        content_plain += "\n" + neuer_eintrag;

        terminalLeeren();
        std::cout << content_plain << "\n";
        std::cout << "---- Neue Eintraege (leer = speichern & beenden) ----\n";
    }
}

std::string neue_datei() {
    while (true) {
        terminalLeeren();
        std::string vorname, name;

        std::cout << "Vorname der Person: ";
        std::getline(std::cin, vorname);
        vorname = trim(vorname);

        std::cout << "Nachname der Person: ";
        std::getline(std::cin, name);
        name = trim(name);

        if (name.empty()) {
            std::cout << "ERROR: Nachname kann nicht leer sein\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }

        std::string vollername = trim(vorname + " " + name);
        std::string dateiname = remove_spaces(vollername);
        std::string fullname = dateiname + ".persondata";

        std::string key;
        std::cout << "Bitte KEY eingeben (leer = zufaelliger Key): ";
        std::getline(std::cin, key);
        key = trim(key);

        if (key.empty()) {
            std::string initialen = build_initials(vollername);
            long long number = random_int64(1000000000LL, 99999999999LL);
            std::string chars = to_lower(dateiname);
            std::string shuffled = shuffle_string(chars);
            key = initialen + std::to_string(number) + shuffled;
        }

        terminalLeeren();
        std::cout << "Der neue Key ist:\n" << key << "\n";
        std::this_thread::sleep_for(std::chrono::seconds(2));

        std::string default_plain;
        default_plain += "----- " + vollername + " -----\n";
        default_plain += "- " + key + " -\n";
        default_plain += "-Name: " + to_upper(name) + "\n";
        default_plain += "-Vorname: " + vorname + "\n";

        std::string default_encrypted = verschluesseln(default_plain);

        terminalLeeren();
        std::cout << "Datei wird erstellt...\n";

        try {
            write_file_utf8(fullname, default_encrypted);
        } catch (...) {
            std::cout << "ERROR: Konnte Datei nicht schreiben: " << fullname << "\n";
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }

        fake_loading(2);
        return dateiname;
    }
}

// ===================== Crypto =====================
std::string buchstaben_verschieben(const std::string& eingabe,
                                   const std::vector<int>& verschiebungen) {
    std::string verschluesselt;
    verschluesselt.reserve(eingabe.size());
    size_t verschiebungs_index = 0;

    for (unsigned char uc : eingabe) {
        char buchstabe = static_cast<char>(uc);

        if (std::isalpha(uc)) {
            char basis = std::isupper(uc) ? 'A' : 'a';
            int verschiebung = verschiebungen[verschiebungs_index % verschiebungen.size()];

            char neuer_buchstabe = static_cast<char>(
                ((buchstabe - basis + verschiebung) % 26) + basis
            );

            verschluesselt.push_back(neuer_buchstabe);
            ++verschiebungs_index;
        } else {
            verschluesselt.push_back(buchstabe);
        }
    }
    return verschluesselt;
}

std::string buchstaben_entschluesseln(const std::string& text,
                                      const std::vector<int>& verschiebungen) {
    std::string entschluesselt;
    entschluesselt.reserve(text.size());
    size_t verschiebungs_index = 0;

    for (unsigned char uc : text) {
        char buchstabe = static_cast<char>(uc);

        if (std::isalpha(uc)) {
            char basis = std::isupper(uc) ? 'A' : 'a';
            int verschiebung = verschiebungen[verschiebungs_index % verschiebungen.size()];

            char neuer_buchstabe = static_cast<char>(
                ((buchstabe - basis - verschiebung + 26) % 26) + basis
            );

            entschluesselt.push_back(neuer_buchstabe);
            ++verschiebungs_index;
        } else {
            entschluesselt.push_back(buchstabe);
        }
    }
    return entschluesselt;
}

std::string verschluesseln(const std::string& text) {
    return buchstaben_verschieben(text, SHIFT);
}

std::string entschluesseln(const std::string& text) {
    return buchstaben_entschluesseln(text, SHIFT);
}

// ===================== File Utils =====================
std::string read_file_utf8(const std::string& path) {
    std::ifstream in(path, std::ios::binary);
    if (!in) throw std::runtime_error("not found");
    std::string data((std::istreambuf_iterator<char>(in)),
                      std::istreambuf_iterator<char>());
    return data;
}

void write_file_utf8(const std::string& path, const std::string& content) {
    std::ofstream out(path, std::ios::binary);
    if (!out) throw std::runtime_error("cannot write");
    out.write(content.data(), static_cast<std::streamsize>(content.size()));
}

// ===================== String / Random Utils =====================
std::string trim(const std::string& s) {
    auto start = s.find_first_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    auto end = s.find_last_not_of(" \t\r\n");
    return s.substr(start, end - start + 1);
}

std::string to_lower(std::string s) {
    for (char& c : s) c = static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    return s;
}

std::string to_upper(std::string s) {
    for (char& c : s) c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    return s;
}

std::string remove_spaces(const std::string& s) {
    std::string out;
    out.reserve(s.size());
    for (unsigned char uc : s) {
        if (!std::isspace(uc)) out.push_back(static_cast<char>(uc));
    }
    return out;
}

std::mt19937& rng() {
    static std::mt19937 gen(
        static_cast<unsigned int>(
            std::chrono::high_resolution_clock::now().time_since_epoch().count()
        )
    );
    return gen;
}

long long random_int64(long long lo, long long hi) {
    std::uniform_int_distribution<long long> dist(lo, hi);
    return dist(rng());
}

std::string shuffle_string(std::string s) {
    std::shuffle(s.begin(), s.end(), rng());
    return s;
}

std::string build_initials(const std::string& vollername) {
    std::string initials;
    bool newWord = true;
    for (unsigned char uc : vollername) {
        if (std::isspace(uc)) {
            newWord = true;
        } else if (newWord) {
            initials.push_back(static_cast<char>(std::toupper(uc)));
            newWord = false;
        }
    }
    return initials;
}