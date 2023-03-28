#include "Canvas.h"
#include <iostream>

Canvas::Canvas(uint32_t width, uint32_t height) : width_(width), height_(height), canvasColors_(width) {
    for (auto &col: canvasColors_) {
        col.resize(height);
    }
}

void Canvas::putPixel(int32_t x, int32_t y, Color color) {
    uint32_t screenX = x + width_ / 2;
    uint32_t screenY = -(y - height_ / 2);

    if (screenX >= width_ || screenY >= height_) {
        std::fprintf(
                stderr,
                "Error coloring canvas pixel (%d, %d): screen (%d, %d) is out of range for canvas of %d x %d\n",
                x, y, screenX, screenY, width_, height_
        );
        return;
    }

    canvasColors_[screenX][screenY] = color;
}

Color Canvas::getScreenPixel(uint32_t x, uint32_t y) const {
    if (x >= width_ || y >= height_) {
        std::fprintf(
                stderr,
                "Error getting screen pixel (%d, %d) is out of range for canvas of %d x %d",
                x, y, width_, height_
        );
        return {0, 0, 0};
    }

    return canvasColors_[x][y];
}

uint32_t Canvas::width() const {
    return width_;
}

uint32_t Canvas::height() const {
    return height_;
}

Range<int32_t> Canvas::getXRange() const {
    return {-static_cast<int32_t>(width_ / 2), static_cast<int32_t>(width_ / 2)};
}

Range<int32_t> Canvas::getYRange() const {
    return {-static_cast<int32_t>(height_ / 2) + 1, static_cast<int32_t>(height_ / 2) + 1};
}
