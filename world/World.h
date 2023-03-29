#pragma once

#include <vector>
#include "Sphere.h"
#include "Lights.h"

struct World {
    const std::vector<Sphere *> objects;
    const std::vector<Light *> lights;
    const Color backgroundColor;

    World(const std::vector<Sphere *> &objects, const std::vector<Light *> &lights, const Color &backgroundColor)
            : objects(objects), lights(lights), backgroundColor(backgroundColor) {}

    virtual ~World() {
        for (auto *obj: objects) {
            delete obj;
        }

        for (auto *light: lights) {
            delete light;
        }
    }
};