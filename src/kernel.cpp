#include <string>
#include <iostream>
#include <cmath>
#include "mm.hpp"
#include "os.cpp"

namespace kernel {
    void entryPoint() {
        std::cout << "Kernel Loaded. Allocating RAM.\n";
        size_t RAM = (size_t) 4ULL * 1024 * 1024 * 1024;  // 4 GB
        auto manager = getManager(malloc(1), RAM, false, true);
        std::cout << "RAM allocated.\n";
    }

    void runOs() {
        os::run();
    }

    void exitPoint() {
        std::cout << "Exiting Kernel.\n";
    }
}
