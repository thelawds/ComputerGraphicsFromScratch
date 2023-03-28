
#include "SdlWindow.h"

#include <utility>

SDLWindow::SDLWindow(uint32_t screenWidth, uint32_t screenHeight, std::string name)
        : screenWidth_(screenWidth), screenHeight_(screenHeight), name_(std::move(name)) {}

SDLWindow::~SDLWindow() {
    if (isInitialized) {
        SDL_DestroyWindow(window);
        SDL_Quit();
    }
}

bool SDLWindow::initialize() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return false;
    }

    window = SDL_CreateWindow(
            name_.c_str(),
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            screenWidth_, screenHeight_,
            SDL_WINDOW_SHOWN & SDL_WINDOW_BORDERLESS
    );

    isInitialized = static_cast<bool>(window);

    if (isInitialized) {
        surface = SDL_GetWindowSurface(window);
        updateWindow();
    }

    return isInitialized;
}

bool SDLWindow::updateWindow() {
    if (isInitialized) {
        SDL_UpdateWindowSurface(window);
        return true;
    }

    return false;
}

bool SDLWindow::updateWindow(const Canvas &canvas) {
    if (!isInitialized) {
        return false;
    }

    for (int x = 0; x < screenWidth_; ++x) {
        for (int y = 0; y < screenHeight_; ++y) {

            Color color = canvas.getScreenPixel(x, y);
            SDL_Rect fillingRect{x, y, 1, 1};
            SDL_FillRect(
                    surface,
                    &fillingRect,
                    SDL_MapRGB(surface->format, color.red, color.green, color.blue)
            );

        }
    }

    return updateWindow();
}

void SDLWindow::delay(uint32_t timeToDelayInSeconds) {
    SDL_Delay(timeToDelayInSeconds * 1000);
}

Canvas SDLWindow::getEmptyCanvas() const {
    return {screenWidth_, screenHeight_};
}
