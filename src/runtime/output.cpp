#include <iostream>
#include <string>
#include "output.hpp"

void cout(std::string text, bool flush, char end) {
    std::cout << text << end;
    if (flush) {
        std::cout.flush();
    }
}

void cout(char** text, bool flush, char end) {
    std::cout << text << end;
    if (flush) {
        std::cout.flush();
    }
}

void flush() {
    std::cout.flush();
}

void clearscreen() {
    std::cout << "\x1B[2J\x1B[H";
}
