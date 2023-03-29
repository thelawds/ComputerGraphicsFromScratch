#pragma once

#include "Viewport.h"
#include "../common/Vector3D.h"

struct Camera {

    const Viewport viewport;
    const Vector3D origin;

    Camera(const Viewport &viewport, const Vector3D &origin) : viewport(viewport), origin(origin) {}

};