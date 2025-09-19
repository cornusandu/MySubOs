#include <windows.h>
#include <cstdint>
#include <iostream>
#include <fstream>
#include <cstddef>
#include "utils.hpp"

class CPU {
    uint16_t v0, pc;
    uint16_t r0, r1, r2, r3, r4, r5;

    private:
        inline uint16_t& get_reg(uint8_t r) {
            switch (r) {
                case (0): {return r0;};
                case (1): {return r1;};
                case (2): {return r2;};
                case (3): {return r3;};
                case (4): {return r4;};
                case (5): {return r5;};
                case (6): {return v0;};
                default: {
                    std::cerr << "Invalid register number" << std::endl;
                    exit(1);
                }
            }
        }

        inline uint16_t to_relative(uint16_t address, uint16_t size) {
            return size - address - 1;
        }

    public:

    void execute(std::byte* content, uint16_t size) {
        v0 = 0; pc = 0; r0 = 0; r1 = 0; r2 = 0; r3 = 0; r4 = 0; r5 = 0;

        while (true) {
            // Fetch instruction
            uint8_t instruction = byte_to_int_be(content[pc]);
            pc++;

            switch (instruction) {
                case (0): {
                    // HALT
                    return;
                } case (1): {
                    // NOP
                    __asm__("nop");
                    break;
                } case (2): {
                    // mov reg<-immediate
                    uint16_t reg = get_reg(byte_to_int_be(content[pc]));
                    pc++;
                    uint16_t imm = two_bytes_to_int_be({content[pc], content[pc + 1]});
                    pc += 2;
                    reg = imm;
                    break;
                } case (3): {
                    // mov reg<-reg
                    uint16_t &reg = get_reg(byte_to_int_be(content[pc]));
                    pc++;
                    uint16_t &reg2 = get_reg(byte_to_int_be(content[pc]));
                    pc++;
                    reg = reg2;
                    break;
                } case (4): {
                    // add destination, reg1, reg2
                    uint16_t &destination = get_reg(byte_to_int_be(content[pc]));        pc++;
                    uint16_t &reg1 = get_reg(byte_to_int_be(content[pc]));               pc++;
                    uint16_t &reg2 = get_reg(byte_to_int_be(content[pc]));               pc++;
                    destination = reg1 + reg2;
                    break;
                } case (5): {
                    // add destination, reg1, immediate
                    uint16_t &destination = get_reg(byte_to_int_be(content[pc]));        pc++;
                    uint16_t &reg1 = get_reg(byte_to_int_be(content[pc]));               pc++;
                    uint16_t imm = two_bytes_to_int_be({content[pc], content[pc + 1]});
                    pc += 2;
                    destination = reg1 + imm;
                    break;
                } case (6): {
                    // str <reg> <address>
                    uint16_t &reg = get_reg(byte_to_int_be(content[pc]));
                    pc++;
                    uint16_t address = two_bytes_to_int_be({content[pc], content[pc + 1]});
                    pc += 2;
                    reg = two_bytes_to_int_be({content[to_relative(address, size) - 1], content[to_relative(address, size)]});
                    break;
                } case (7): {
                    // str <address> <reg>
                    uint16_t address = two_bytes_to_int_be({content[pc], content[pc + 1]});
                    pc += 2;
                    uint16_t &reg = get_reg(byte_to_int_be(content[pc]));
                    pc++;
                    content[to_relative(address, size) - 1] = int_to_byte(reg) >> 8;
                    content[to_relative(address, size)] = int_to_byte(reg & 0xFF);
                    break;
                } case (8): {
                    // str <address> <immediate>
                    uint16_t address = two_bytes_to_int_be({content[pc], content[pc + 1]});
                    pc += 2;
                    uint16_t immediate = two_bytes_to_int_be({content[pc], content[pc + 1]});
                    pc += 2;
                    content[address] = int_to_byte(immediate) >> 8;
                    content[address+1] = int_to_byte(immediate & 0xFF);
                    break;
                } case (9): {
                    // print
                    std::cout << ascii_bytes_to_string(int_to_bytes((int16_t) v0));
                    break;
                } case (10) : {
                    // printliteral
                    std::cout << v0;
                    break;
                } default: {
                    std::cerr << "Invalid instruction: " << instruction << std::endl;
                    exit(1);
                }
            }
        }
    };

};

int main(int argc, char **argv) {
    char* filename = argv[1];

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        std::cerr << "Failed to open file: " << filename << "\n";
        return 1;
    }

    // get size
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // allocate memory for content
    std::byte* buffer = new std::byte[size];
    std::byte** content = &buffer; // pointer-to-pointer
    uint16_t buffer_size = static_cast<std::size_t>(size);

    // read into buffer
    if (!file.read(reinterpret_cast<char*>(buffer), size)) {
        std::cerr << "Failed to read file.\n";
        delete[] buffer;
        return 1;
    }

    CPU cpu;
    cpu.execute(buffer, buffer_size);

    delete[] buffer;
    return 0;
}
