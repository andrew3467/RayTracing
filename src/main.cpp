#include <iostream>

#include "Camera.h"
#include "Sphere.h"

int main() {
    HittableList world;

    world.Add(std::make_shared<Sphere>(Point3(0, 0, -1), 0.5));
    world.Add(std::make_shared<Sphere>(Point3(0, -100.5, -1), 100));

    Camera cam;

    cam.SamplesPerPixel = 100;

    cam.Render(world);
}
