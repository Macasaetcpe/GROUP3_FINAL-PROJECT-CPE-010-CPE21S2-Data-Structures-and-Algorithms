#ifndef UTILS_H
#define UTILS_H

#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>

// Shift settings (minutes after midnight): 08:00 start, 17:00 end
const int SHIFT_START = 8 * 60;
const int SHIFT_END = 17 * 60;

namespace Utils {

inline std::string toLower(std::string s) {
    for (char& c : s) c = (char)std::tolower((unsigned char)c);
    return s;
}

inline std::string trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && std::isspace((unsigned char)s[a])) a++;
    while (b > a && std::isspace((unsigned char)s[b - 1])) b--;
    return s.substr(a, b - a);
}

// Reads one line. If input is closed, the program ends safely.
inline std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string line;
    if (!std::getline(std::cin, line)) {
        std::cout << "\nInput closed. Exiting.\n";
        std::exit(0);
    }
    return trim(line);
}

// Turns text into a whole number. Returns false if it is not a clean number.
inline bool tryParseInt(const std::string& text, int& out) {
    if (text.empty()) return false;
    std::istringstream in(text);
    long long v;
    char extra;
    if (!(in >> v) || (in >> extra)) return false;
    if (v < -1000000000LL || v > 1000000000LL) return false;
    out = (int)v;
    return true;
}

// Keeps asking until the user types a whole number between lo and hi.
inline int readInt(const std::string& prompt, int lo, int hi) {
    while (true) {
        int v;
        if (tryParseInt(readLine(prompt), v) && v >= lo && v <= hi) return v;
        std::cout << "  [Invalid] Please enter a number from " << lo << " to " << hi << ".\n";
    }
}

inline std::string readNonEmpty(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        if (!s.empty()) return s;
        std::cout << "  [Invalid] This cannot be empty.\n";
    }
}

// Reads a time like 08:15 (24-hour). Returns minutes after midnight.
inline int readTime(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.size() == 5 && s[2] == ':' && std::isdigit((unsigned char)s[0]) &&
            std::isdigit((unsigned char)s[1]) && std::isdigit((unsigned char)s[3]) &&
            std::isdigit((unsigned char)s[4])) {
            int h = (s[0] - '0') * 10 + (s[1] - '0');
            int m = (s[3] - '0') * 10 + (s[4] - '0');
            if (h <= 23 && m <= 59) return h * 60 + m;
        }
        std::cout << "  [Invalid] Use 24-hour format HH:MM (example 08:15).\n";
    }
}


// Current clock time as minutes after midnight
inline int nowMinutes() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    return local->tm_hour * 60 + local->tm_min;
}

// Like readTime, but pressing Enter uses the current time.
inline int readTimeOrNow(const std::string& prompt) {
    while (true) {
        std::string s = readLine(prompt);
        if (s.empty()) return nowMinutes();
        if (s.size() == 5 && s[2] == ':' && std::isdigit((unsigned char)s[0]) &&
            std::isdigit((unsigned char)s[1]) && std::isdigit((unsigned char)s[3]) &&
            std::isdigit((unsigned char)s[4])) {
            int h = (s[0] - '0') * 10 + (s[1] - '0');
            int m = (s[3] - '0') * 10 + (s[4] - '0');
            if (h <= 23 && m <= 59) return h * 60 + m;
        }
        std::cout << "  [Invalid] Use HH:MM (example 08:15) or press Enter for now.\n";
    }
}

inline std::string formatTime(int minutes) {
    if (minutes < 0) return "--:--";
    char buf[16];
    std::snprintf(buf, sizeof(buf), "%02d:%02d", minutes / 60, minutes % 60);
    return std::string(buf);
}

}  // namespace Utils

#endif
