#include <iostream>
#include <cmath>
#include "mm.hpp"

namespace os {
    void run() {
        auto manager = getManager((void*) nullptr, 0, true, true);
        std::cout << "Hello from the OS! :)" << std::endl;
        // Code goes here. Once this code finishes, bootloader will exit the kernel.
    }
}
