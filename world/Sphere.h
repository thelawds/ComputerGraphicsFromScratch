#pragma once

#include "../common/Vector3D.h"
#include "../common/Canvas.h"
#include "../common/Color.h"

class Sphere {
    Vector3D centre_;
    double radius_;
    Color color_;
    int32_t specular_;
    double reflectiveness_;

public:

    Sphere(const Vector3D &centre, double radius, Color color, int32_t specular, double reflectiveness)
            : centre_(centre),
              radius_(radius),
              color_(color),
              specular_(specular),
              reflectiveness_(reflectiveness) {

    }

    inline const Vector3D &centre() const {
        return centre_;
    }

    inline double radius() const {
        return radius_;
    }

    inline Color color() const {
        return color_;
    }

    inline int32_t specular() const {
        return specular_;
    }

    inline double reflectiveness() const {
        return reflectiveness_;
    }

};
