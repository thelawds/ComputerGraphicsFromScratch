#pragma once

#include <string>
#include "Canvas.h"
#include "SDL2/SDL.h"

class SDLWindow {

public:

    SDLWindow(uint32_t screenWidth, uint32_t screenHeight, std::string name);

    ~SDLWindow();

    bool initialize();

    bool updateWindow();

    bool updateWindow(const Canvas &canvas);

    static void delay(uint32_t timeToDelayInSeconds);

    Canvas getEmptyCanvas() const;

    inline operator bool () const {
        return isInitialized;
    }

private:
    const uint32_t screenWidth_;
    const uint32_t screenHeight_;
    const std::string name_;
    bool isInitialized = false;

    SDL_Window* window;
    SDL_Surface* surface;

};

