#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include "Color.h"

template<typename T>
struct Range {
    T fromInclusive;
    T toExclusive;

    std::string toString() {
        return "[" + std::to_string(fromInclusive) + ", " + std::to_string(toExclusive) + ")";
    }
};

class Canvas {
public:

    Canvas(uint32_t width, uint32_t height);

    void putPixel(int32_t x, int32_t y, Color color);

    Color getScreenPixel(uint32_t x, uint32_t y) const;

    uint32_t width() const;

    uint32_t height() const;

    Range<int32_t> getXRange() const;

    Range<int32_t> getYRange() const;

private:
    uint32_t width_;
    uint32_t height_;
    std::vector<std::vector<Color>> canvasColors_;

};


