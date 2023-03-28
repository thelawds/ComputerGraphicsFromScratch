#pragma once

#include <cstdint>
#include <iostream>
#include <algorithm>

struct Color {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
};

inline Color operator+(const Color &color1, const Color &color2) {
    uint64_t newRed = static_cast<uint64_t>(color1.red) + color2.red;
    uint64_t newGreen = static_cast<uint64_t>(color1.green) + color2.green;
    uint64_t newBlue = static_cast<uint64_t>(color1.blue) + color2.blue;
    return {static_cast<uint8_t>(newRed), static_cast<uint8_t>(newGreen), static_cast<uint8_t>(newBlue)};
}

inline Color operator*(const Color &color, double scalar) {
    uint64_t newRed = color.red * scalar;
    uint64_t newGreen = color.green * scalar;
    uint64_t newBlue = color.blue * scalar;

    return {
            static_cast<uint8_t>(std::clamp(newRed, 0ul, 255ul)),
            static_cast<uint8_t>(std::clamp(newGreen, 0ul, 255ul)),
            static_cast<uint8_t>(std::clamp(newBlue, 0ul, 255ul))
    };
}

inline Color operator*(double scalar, const Color &color) {
    return color * scalar;
}