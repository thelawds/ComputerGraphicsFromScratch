#pragma once

#include "../common/Vector3D.h"
#include "../world/Sphere.h"
#include "Renderer.h"
#include "../world/World.h"

class RayTracingRenderer : public Renderer {

    const World &world;

public:
    explicit RayTracingRenderer(const World &world);

    ~RayTracingRenderer() override;

    void render(const Camera &camera, Canvas &outCanvas) override;

private:

    static Vector3D reflectRay(const Vector3D &surfaceNormal, const Vector3D &direction);

    static std::pair<double, double>
    intersectRaySphere(const Vector3D &origin, const Vector3D &direction, const Sphere &sphere);

    std::pair<double, const Sphere *>
    closestIntersection(const Vector3D &origin, const Vector3D &direction, double t_min, double t_max);

    double computeLightning(
            const Vector3D &surfacePoint,
            const Vector3D &surfaceNormal,
            const Vector3D &surfaceToCamera,
            int32_t surfaceSpecular
    );

    Color traceRay(Vector3D origin, Vector3D direction, double t_min, double t_max, uint32_t recursionDepth);
};
