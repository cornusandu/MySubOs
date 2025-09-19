#include <cstddef>   // std::byte
#include <cstdint>   // int8_t, int16_t, int32_t
#include <array>
#include <string>
#include <vector>
#include <iostream>

// =======================================================
// Bytes -> int (big-endian)
// =======================================================
inline uint8_t byte_to_int_be(std::byte b) noexcept {
    return static_cast<uint8_t>(b);
}

inline uint16_t two_bytes_to_int_be(std::byte high, std::byte low) noexcept {
    return (static_cast<uint16_t>(static_cast<uint8_t>(high)) << 8) |
            static_cast<uint16_t>(static_cast<uint8_t>(low));
}

inline uint16_t two_bytes_to_int_be(const std::vector<std::byte>& bytes) noexcept {
    if (bytes.size() < 2) {
        return 0; // or throw, depending on your needs
    }
    return (static_cast<uint16_t>(static_cast<uint8_t>(bytes[0])) << 8) |
            static_cast<uint16_t>(static_cast<uint8_t>(bytes[1]));
}

int32_t four_bytes_to_int_be(const std::array<std::byte, 4>& bytes) {
    return (static_cast<int32_t>(std::to_integer<uint8_t>(bytes[0])) << 24) |
           (static_cast<int32_t>(std::to_integer<uint8_t>(bytes[1])) << 16) |
           (static_cast<int32_t>(std::to_integer<uint8_t>(bytes[2])) << 8)  |
            static_cast<int32_t>(std::to_integer<uint8_t>(bytes[3]));
}

inline uint16_t two_bytes_to_int_be(std::initializer_list<std::byte> bytes) noexcept {
    auto it = bytes.begin();
    return (static_cast<uint16_t>(static_cast<uint8_t>(*it)) << 8) |
           static_cast<uint16_t>(static_cast<uint8_t>(*(it + 1)));
}

// =======================================================
// Int -> bytes (big-endian)
// =======================================================
inline std::byte int_to_byte(uint16_t value) noexcept {
    // return only the low 8 bits as a byte (common usage in your code)
    return static_cast<std::byte>(value & 0xFF);
}

inline std::vector<std::byte> int_to_bytes(uint16_t value) {
    return {
        static_cast<std::byte>((value >> 8) & 0xFF),
        static_cast<std::byte>(value & 0xFF)
    };
}

inline std::vector<std::byte> int_to_bytes(int32_t value) {
    return {
        static_cast<std::byte>((value >> 24) & 0xFF),
        static_cast<std::byte>((value >> 16) & 0xFF),
        static_cast<std::byte>((value >> 8) & 0xFF),
        static_cast<std::byte>(value & 0xFF)
    };
}

// =======================================================
// Int <-> string
// =======================================================
std::string int_to_string(int8_t value) {
    return std::to_string(value);
}

// =======================================================
// String <-> ASCII bytes
// =======================================================
std::vector<std::byte> string_to_ascii_bytes(const std::string& str) {
    std::vector<std::byte> bytes;
    bytes.reserve(str.size());
    for (unsigned char c : str) {
        bytes.push_back(std::byte{c});
    }
    return bytes;
}

std::string ascii_bytes_to_string(const std::vector<std::byte>& bytes) {
    std::string str;
    str.reserve(bytes.size());
    for (auto b : bytes) {
        str.push_back(static_cast<char>(std::to_integer<uint8_t>(b)));
    }
    return str;
}
