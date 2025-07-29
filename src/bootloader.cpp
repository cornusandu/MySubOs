#include "kernel.cpp"
#include "mm.hpp"

int main() {
    kernel::entryPoint();

    MemoryManager& manager = getManager((void*) nullptr, 0 , true, true);

    kernel::runOs();

    kernel::exitPoint();
}
