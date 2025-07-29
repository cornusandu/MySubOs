#include "mm.hpp"
#include <cstdlib>
#include <windows.h>
#include <stdexcept>
#include <cstdint>

size_t allocate_byte(void* address, int bytes = 1) {
    void* result = VirtualAlloc(
        address,                  // desired address
        bytes,                        // size in bytes
        MEM_RESERVE | MEM_COMMIT,// reserve and commit in one call
        PAGE_READWRITE            // access protection
    );

    if (result == NULL) {
        return 1; // address is unavailable
    }

    // optional: write to it to confirm
    *((volatile uint8_t*)result) = 0x42;

    return 0; // success
}

MemoryManager::MemoryManager(void* start, int size, bool allowHop, bool virtualMap) {
    if (allowHop && virtualMap) {
        throw std::runtime_error("Cannot enable both allowHop and virtualMap.");
    }
    if (!allowHop && !virtualMap) {
        free(start);
        uintptr_t address = (uintptr_t) start;
        size_t index = 0;
        for (uintptr_t i = address; i < address + size; i++) {
            data_access_amount[address] = 0;
            vmap[index] = i;
            int status = allocate_byte((void*)i);
            if (!status) {
                for (auto a : data_access_amount) {
                    VirtualFree((void*) a.first, 0, MEM_RELEASE);
                }
                throw std::runtime_error("Attempted to claim unavailable byte. Please enable allowHop or virtualMap to mitigate errors.");
            }
            index++;
        }
    } else if (virtualMap) {
        size_t index = 0;
        for (int i = 0; i < size; i++) {
            void* byte = malloc(1);
            int count = 0;
            while (!byte) {
                void* byte = malloc(1);
                count++;
                if (count >= 5) {
                    for (auto a : data_access_amount) {
                        free((void*) a.first);
                    }
                    throw std::runtime_error("Failed 5 times in a row to claim a byte.");
                };
            }
            data_access_amount[(uintptr_t) byte] = 0;
            vmap[index] = i;
            index++;
        }
    } else if (allowHop) {
        size_t index = 0;
        uintptr_t address = (uintptr_t) start;
        while (true) {
            bool didbreak = false;
            for (uintptr_t i = address; i < address + size; i++) {
                data_access_amount[address] = 0;
                int status = allocate_byte((void*)i);
                if (!status) {
                    address = (uintptr_t)(rand() * rand());
                    didbreak = true;
                    break;
                };
                vmap[index] = i;
                index++;
            }
            if (!didbreak) {
                break;
            }
        }
    }
}

void* MemoryManager::malloc(int size) {
    // TODO
}

template <typename T>
T MemoryManager::read_mem(uintptr_t address) const {
    data_access_amount[address]++;
    return *(T*)address;
}

template <typename T>
int MemoryManager::write_mem(uintptr_t address, T data) {
    try {
        data_access_amount[address]++;
        *(T*)address = data;
    } catch (std::exception) {
        return 1;
    }
    return 0;
}

// TODO: SWITCH TO VirtualFree(start, 0, MEM_RELEASE) INSTEAD OF free() ON DAT ALLOCATED USING allocate_byte()   --------------   DONE
// TODO: MAKE DISTINGUISHMENT BETWEEN ALLOCATED AND UNNALOCATED MEMORY   ------------------------------------------------------