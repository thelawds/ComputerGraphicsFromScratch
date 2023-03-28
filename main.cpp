#include <iostream>
#include <limits>
#include "common/SdlWindow.h"
#include "common/Vector3D.h"
#include "world/Sphere.h"
#include "world/Viewport.h"
#include "world/Lights.h"

constexpr double EPSILON = 10e-3;

const Color backgroundColor = {50, 50, 50};

const Sphere spheres[]{
        Sphere({0, -1, 3}, 1, {255, 0, 0}, 500, .2),
        Sphere({2, 0, 5}, 1, {0, 0, 255}, 500, .3),
        Sphere({-2, 0, 5}, 1, {0, 255, 0}, 10, .05),
        Sphere({0, -5001, 0}, 5000, {255, 255, 0}, 1000, .5)
};

const Light *lights[]{
        new AmbientLight(0.2),
        new PointLight(0.6, Vector3D(2, 1, 0)),
        new DirectionalLight(0.2, Vector3D(1, 4, 4))
};

Vector3D reflectRay(const Vector3D& surfaceNormal, const Vector3D& direction) {
    return 2 * surfaceNormal * (surfaceNormal * direction) - direction;
}

// todo: Move to individual objects
std::pair<double, double> intersectRaySphere(const Vector3D &origin, const Vector3D &direction, const Sphere &sphere) {
    Vector3D CO = origin - sphere.centre();
    double a = direction * direction;
    double b = 2 * (CO * direction);
    double c = CO * CO - sphere.radius() * sphere.radius();

    double discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        return {std::numeric_limits<double>::max(), std::numeric_limits<double>::max()};
    }

    double t1 = (-b + std::sqrt(discriminant)) / (2 * a);
    double t2 = (-b - std::sqrt(discriminant)) / (2 * a);
    return {t1, t2};
}

std::pair<double, const Sphere *>
closestIntersection(const Vector3D &origin, const Vector3D &direction, double t_min, double t_max) {
    double closestT = std::numeric_limits<double>::max();
    const Sphere *closestSphere = nullptr;

    for (const Sphere &sphere: spheres) {
        auto [t1, t2] = intersectRaySphere(origin, direction, sphere);

        if (t1 > t_min && t1 < t_max && t1 < closestT) {
            closestT = t1;
            closestSphere = &sphere;
        }

        if (t2 > t_min && t2 < t_max && t2 < closestT) {
            closestT = t2;
            closestSphere = &sphere;
        }

    }

    return {closestT, closestSphere};
}

double computeLightning(
        const Vector3D &surfacePoint,
        const Vector3D &surfaceNormal,
        const Vector3D &surfaceToCamera,
        int32_t surfaceSpecular
) {
    double resultIntensity = 0;
    for (const auto &light: lights) {
        if (dynamic_cast<const AmbientLight *>(light)) {
            resultIntensity += light->intensity;
        } else {
            Vector3D lightDirection{0, 0, 0};
            if (dynamic_cast<const PointLight *>(light)) {
                const auto &pointLight = dynamic_cast<const PointLight *>(light);
                lightDirection = pointLight->position - surfacePoint;
            } else {
                const auto &directedLight = dynamic_cast<const DirectionalLight *>(light);
                lightDirection = directedLight->direction;
            }

            // Check for objects casting shadows on current point
            auto [shadowT, shadowSphere] = closestIntersection(
                    surfacePoint, lightDirection, EPSILON, std::numeric_limits<double>::max());

            if (shadowSphere) {
                continue;
            }

            // Diffuse light
            double intensity = surfaceNormal * lightDirection / (surfaceNormal.length() * lightDirection.length());

            if (intensity > 0) {
                resultIntensity += light->intensity * intensity;
            }

            // Specular light
            if (surfaceSpecular != -1) {
                Vector3D reflectionDirection = reflectRay(surfaceNormal, lightDirection);
                double specular = reflectionDirection * surfaceToCamera
                                  / (reflectionDirection.length() * surfaceToCamera.length());

                if (specular > 0) {
                    resultIntensity += light->intensity * std::pow(specular, surfaceSpecular);
                }
            }
        }
    }

    return resultIntensity;
}

Color traceRay(Vector3D origin, Vector3D direction, double t_min, double t_max, uint32_t recursionDepth) {
    auto [closestT, closestSphere] = closestIntersection(origin, direction, t_min, t_max);

    if (!closestSphere) {
        return backgroundColor;
    }

    // Compute local color:
    Vector3D spherePoint = origin + closestT * direction;
    Vector3D sphereNormal = spherePoint - closestSphere->centre();
    sphereNormal /= sphereNormal.length();

    Color localColor = closestSphere->color()
                       * computeLightning(spherePoint, sphereNormal, -direction, closestSphere->specular());

    double r = closestSphere->reflectiveness();
    if (recursionDepth == 0 || r <= 0) {
        return localColor;
    }

    Vector3D reflectedRay = reflectRay(sphereNormal, -direction);
    Color reflectedColor = traceRay(
            spherePoint, reflectedRay, EPSILON, std::numeric_limits<double>::max(), recursionDepth - 1
    );

    Color resultColor = localColor * (1.0 - r) + reflectedColor * r;
    return resultColor;
}

int main() {
    SDLWindow window{1280, 1280, "RayTracing"};
    window.initialize();
    Canvas canvas = window.getEmptyCanvas();
    Viewport viewport{1, 1, 1};
    Vector3D origin{0, 0, 0};

    auto XRange = canvas.getXRange();
    auto YRange = canvas.getYRange();

    printf("Going for x in %s and y in %s.\n", XRange.toString().c_str(), YRange.toString().c_str());

    for (int32_t canvasX = XRange.fromInclusive; canvasX < XRange.toExclusive; ++canvasX) {
        for (int32_t canvasY = YRange.fromInclusive; canvasY < YRange.toExclusive; ++canvasY) {
            Vector3D direction{
                    canvasX * static_cast<double>(viewport.width) / canvas.width(),
                    canvasY * static_cast<double>(viewport.height) / canvas.height(),
                    static_cast<double>(viewport.distanceFromOrigin)
            };
            Color color = traceRay(origin, direction, 1, std::numeric_limits<double>::max(), 3);
            canvas.putPixel(canvasX, canvasY, color);
        }
    }

    window.updateWindow(canvas);
    SDLWindow::delay(10);

    for (auto &light: lights) {
        delete light;
    }

    return 0;
}
