#pragma once

#include "../common/Vector3D.h"

struct Light {
    const double intensity;

    explicit Light(const double intensity) : intensity(intensity) {}

    virtual ~Light() = default;
};

struct AmbientLight : public Light {
    explicit AmbientLight(const double intensity) : Light(intensity) {}
};

struct PointLight : public Light {
    const Vector3D position;

    PointLight(const double intensity, const Vector3D &position) : Light(intensity), position(position) {}
};

struct DirectionalLight : public Light {
    const Vector3D direction;

    DirectionalLight(const double intensity, const Vector3D &direction) : Light(intensity), direction(direction) {}
};