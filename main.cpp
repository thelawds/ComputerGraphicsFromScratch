#include "common/SdlWindow.h"
#include "common/Vector3D.h"
#include "world/World.h"
#include "world/Camera.h"
#include "renderers/RayTracingRenderer.h"

const World world = {
        {
                new Sphere({0, -1, 3}, 1, {255, 0, 0}, 500, .2),
                new Sphere({2, 0, 5}, 1, {0, 0, 255}, 500, .3),
                new Sphere({-2, 0, 5}, 1, {0, 255, 0}, 10, .05),
                new Sphere({0, -5001, 0}, 5000, {255, 255, 0}, 1000, .5)
        },
        {
                new AmbientLight(0.2),
                new PointLight(0.6, Vector3D(2, 1, 0)),
                new DirectionalLight(0.2, Vector3D(1, 4, 4))
        },
        Color{50, 50, 50}
};

int main() {
    SDLWindow window{1280, 1280, "RayTracing"};
    window.initialize();
    Canvas canvas = window.getEmptyCanvas();
    Camera camera{{1, 1, 1},
                  {0, 0, 0}};

    Renderer *renderer = new RayTracingRenderer(world);
    renderer->render(camera, canvas);

    window.updateWindow(canvas);
    SDLWindow::delay(10);

    delete renderer;
    return 0;
}
