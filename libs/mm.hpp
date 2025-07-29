#pragma once

#include <unordered_map>

class MemoryManager {
    public:
        template <typename T>
        T read_mem(uintptr_t address) const;

        template <typename T>
        int write_mem(uintptr_t address, T data);

        void* malloc(int size);

        void* calloc(int blocks, int size);
        
        MemoryManager(void* start, int size, bool allowHop = false, bool virtualMap = false);

    private:
        mutable std::unordered_map<uintptr_t, size_t> data_access_amount;
        bool virtualMap;                        // Switches to malloc() to allocate random byte, but requires extra mapping between kernel and OS/Software.
        std::unordered_map<uintptr_t, uintptr_t> vmap;  // OS address -> host address
};

static MemoryManager& getManager(void* start, int size, bool allowHop = false, bool virtualMap = false) {
    static MemoryManager manager(start, size, allowHop, virtualMap);
    return manager;
}

// TODO: Add mapping between OS memory addresses to host addresses  ----------------------   DONE
// TODO: Add garbage collection for variables that haven't been used for 150 cycles