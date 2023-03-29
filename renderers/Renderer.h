#pragma once

#include "../common/Canvas.h"
#include "../world/Camera.h"

class Renderer {

public:
    virtual void render(const Camera &camera, Canvas &outCanvas) = 0;

    virtual ~Renderer() = default;
};