
#include <utility>
#include <limits>
#include <complex>
#include "RayTracingRenderer.h"

RayTracingRenderer::RayTracingRenderer(const World &world) : world(world) {}

RayTracingRenderer::~RayTracingRenderer() = default;

void RayTracingRenderer::render(const Camera &camera, Canvas &outCanvas) {
    const auto &viewport = camera.viewport;
    const auto &origin = camera.origin;

    auto XRange = outCanvas.getXRange();
    auto YRange = outCanvas.getYRange();

    for (int32_t canvasX = XRange.fromInclusive; canvasX < XRange.toExclusive; ++canvasX) {
        for (int32_t canvasY = YRange.fromInclusive; canvasY < YRange.toExclusive; ++canvasY) {
            Vector3D direction{
                    canvasX * static_cast<double>(viewport.width) / outCanvas.width(),
                    canvasY * static_cast<double>(viewport.height) / outCanvas.height(),
                    static_cast<double>(viewport.distanceFromOrigin)
            };
            Color color = traceRay(origin, direction, 1, std::numeric_limits<double>::max(), 3);
            outCanvas.putPixel(canvasX, canvasY, color);
        }
    }
}

Vector3D RayTracingRenderer::reflectRay(const Vector3D &surfaceNormal, const Vector3D &direction) {
    return 2 * surfaceNormal * (surfaceNormal * direction) - direction;
}

// todo: Move to individual objects
std::pair<double, double>
RayTracingRenderer::intersectRaySphere(const Vector3D &origin, const Vector3D &direction, const Sphere &sphere) {
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
RayTracingRenderer::closestIntersection(const Vector3D &origin, const Vector3D &direction, double t_min, double t_max) {
    double closestT = std::numeric_limits<double>::max();
    const Sphere *closestSphere = nullptr;

    for (const auto &obj: world.objects) {
        auto [t1, t2] = intersectRaySphere(origin, direction, *obj);

        if (t1 > t_min && t1 < t_max && t1 < closestT) {
            closestT = t1;
            closestSphere = obj;
        }

        if (t2 > t_min && t2 < t_max && t2 < closestT) {
            closestT = t2;
            closestSphere = obj;
        }

    }

    return {closestT, closestSphere};
}

double RayTracingRenderer::computeLightning(
        const Vector3D &surfacePoint,
        const Vector3D &surfaceNormal,
        const Vector3D &surfaceToCamera,
        int32_t surfaceSpecular
) {
    double resultIntensity = 0;
    for (const auto &light: world.lights) {
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
                    surfacePoint, lightDirection, std::numeric_limits<float>::epsilon(),
                    std::numeric_limits<double>::max());

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

Color
RayTracingRenderer::traceRay(Vector3D origin, Vector3D direction, double t_min, double t_max, uint32_t recursionDepth) {
    auto [closestT, closestSphere] = closestIntersection(origin, direction, t_min, t_max);

    if (!closestSphere) {
        return world.backgroundColor;
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
            spherePoint, reflectedRay, std::numeric_limits<float>::epsilon(), std::numeric_limits<double>::max(),
            recursionDepth - 1
    );

    Color resultColor = localColor * (1.0 - r) + reflectedColor * r;
    return resultColor;
}
