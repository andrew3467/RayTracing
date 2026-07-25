#include <iostream>

#include "Camera.h"
#include "Sphere.h"
#include "BvhNode.h"
#include "quad.h"


void BouncingSpheres() {
    HittableList world;

    auto checkeredTex = std::make_shared<TextureCheckered>(0.32, Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    world.Add(make_shared<Sphere>(Point3(0,-1000,0), 1000, std::make_shared<Lambertian>(checkeredTex)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = RandomDouble01();
            Point3 center(a + 0.9 * RandomDouble01(), 0.2, b + 0.9 * RandomDouble01());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::Random() * Color::Random();
                    sphere_material = std::make_shared<Lambertian>(albedo);
                    auto center2 = center + vec3(0, RandomDouble(0, 0.5), 0);
                    world.Add(std::make_shared<Sphere>(center, center2, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::Random(0.5, 1);
                    auto fuzz = RandomDouble(0, 0.5);
                    sphere_material = std::make_shared<Metal>(albedo, fuzz);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<Dielectric>(1.5);
                    world.Add(std::make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = std::make_shared<Dielectric>(1.5);
    world.Add(std::make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.Add(std::make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.Add(std::make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    world = HittableList(std::make_shared<BvHNode>(world));

    Camera cam;

    cam.AspectRatio      = 16.0 / 9.0;
    cam.ImageWidth       = 600;
    cam.SamplesPerPixel = 800;
    cam.MaxRayDepth         = 50;

    cam.Background = {0.70, 0.80, 1.00};

    cam.VertFOV     = 20;
    cam.LookFrom = Point3(13,2,3);
    cam.LookAt   = Point3(0,0,0);
    cam.VUp = vec3(0,1,0);

    cam.DefocusAngle = 0.6;
    cam.FocusDist    = 10.0;

    cam.Render(world);
}

void CheckeredSpheres() {
    HittableList world;

    auto checker = std::make_shared<TextureCheckered>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));

    world.Add(make_shared<Sphere>(Point3(0,-10, 0), 10, make_shared<Lambertian>(checker)));
    world.Add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checker)));

    Camera cam;

    cam.AspectRatio      = 16.0 / 9.0;
    cam.ImageWidth       = 400;
    cam.SamplesPerPixel = 100;
    cam.MaxRayDepth         = 50;

    cam.Background = {0.70, 0.80, 1.00};

    cam.VertFOV     = 20;
    cam.LookFrom = Point3(13,2,3);
    cam.LookAt   = Point3(0,0,0);
    cam.VUp      = vec3(0,1,0);

    cam.DefocusAngle = 0;

    cam.Render(world);
}

void Earth() {
    auto  earthTex = std::make_shared<TextureImage>("earthmap.jpg");
    auto earthSurface = std::make_shared<Lambertian>(earthTex);
    auto globe = std::make_shared<Sphere>(Point3(0, 0, 0), 2, earthSurface);

    Camera cam;

    cam.AspectRatio      = 16.0 / 9.0;
    cam.ImageWidth= 800;
    cam.SamplesPerPixel = 100;
    cam.MaxRayDepth         = 50;

    cam.Background = {0.70, 0.80, 1.00};

    cam.VertFOV     = 20;
    cam.LookFrom = Point3(0,0,12);
    cam.LookAt   = Point3(0,0,0);
    cam.VUp      = vec3(0,1,0);

    cam.DefocusAngle = 0;

    cam.Render(HittableList(globe));
}

void NoiseMapping() {
    HittableList world;

    auto perText = std::make_shared<TextureNoise>(4);
    world.Add(std::make_shared<Sphere>(Point3(0, -1000, 0), 1000, std::make_shared<Lambertian>(perText)));
    world.Add(std::make_shared<Sphere>(Point3(0, 2, 0), 2, std::make_shared<Lambertian>(perText)));


    Camera cam;

    cam.AspectRatio      = 16.0 / 9.0;
    cam.ImageWidth= 800;
    cam.SamplesPerPixel = 100;
    cam.MaxRayDepth         = 50;

    cam.Background = {0.70, 0.80, 1.00};

    cam.VertFOV     = 20;
    cam.LookFrom = Point3(13, 2, 3);
    cam.LookAt   = Point3(0,0,0);
    cam.VUp      = vec3(0,1,0);

    cam.DefocusAngle = 0;

    cam.Render(world);
}

void Quads() {
    HittableList world;

    // Materials
    auto left_red     = std::make_shared<Lambertian>(Color(1.0, 0.2, 0.2));
    auto back_green   = std::make_shared<Lambertian>(Color(0.2, 1.0, 0.2));
    auto right_blue   = std::make_shared<Lambertian>(Color(0.2, 0.2, 1.0));
    auto upper_orange = std::make_shared<Lambertian>(Color(1.0, 0.5, 0.0));
    auto lower_teal   = std::make_shared<Lambertian>(Color(0.2, 0.8, 0.8));

    // Quads
    world.Add(std::make_shared<Quad>(Point3(-3,-2, 5), vec3(0, 0,-4), vec3(0, 4, 0), left_red));
    world.Add(std::make_shared<Quad>(Point3(-2,-2, 0), vec3(4, 0, 0), vec3(0, 4, 0), back_green));
    world.Add(std::make_shared<Quad>(Point3( 3,-2, 1), vec3(0, 0, 4), vec3(0, 4, 0), right_blue));
    world.Add(std::make_shared<Quad>(Point3(-2, 3, 1), vec3(4, 0, 0), vec3(0, 0, 4), upper_orange));
    world.Add(std::make_shared<Quad>(Point3(-2,-3, 5), vec3(4, 0, 0), vec3(0, 0,-4), lower_teal));


    Camera cam;

    cam.AspectRatio      = 1;
    cam.ImageWidth= 100;
    cam.SamplesPerPixel = 100;
    cam.MaxRayDepth         = 50;

    cam.Background = {0.70, 0.80, 1.00};

    cam.VertFOV     = 80;
    cam.LookFrom = Point3(0, 0, 9);
    cam.LookAt   = Point3(0,0,0);
    cam.VUp      = vec3(0,1,0);

    cam.DefocusAngle = 0;

    cam.Render(world);
}

void SimpleLight() {
    HittableList world;

    auto pertext = std::make_shared<TextureNoise>(4);
    world.Add(std::make_shared<Sphere>(Point3(0,-1000,0), 1000, std::make_shared<Lambertian>(pertext)));
    world.Add(std::make_shared<Sphere>(Point3(0,2,0), 2, std::make_shared<Lambertian>(pertext)));

    auto difflight = std::make_shared<DiffuseLight>(Color(4,4,4));
    world.Add(std::make_shared<Quad>(Point3(3,1,-2), vec3(2,0,0), vec3(0,2,0), difflight));


    Camera cam;

    cam.AspectRatio     = 16.0 / 9;
    cam.ImageWidth      = 800;
    cam.SamplesPerPixel = 100;
    cam.MaxRayDepth     = 50;
    cam.Background      = {0.00, 0.00, 0.00};

    cam.VertFOV     = 20;
    cam.LookFrom = Point3(26, 3, 6);
    cam.LookAt   = Point3(0,2,0);
    cam.VUp      = vec3(0,1,0);

    cam.DefocusAngle = 0;

    cam.Render(world);
}

void CornellBox() {
    HittableList world;

    auto red   = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(Color(15, 15, 15));

    world.Add(std::make_shared<Quad>(Point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.Add(std::make_shared<Quad>(Point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.Add(std::make_shared<Quad>(Point3(343, 554, 332), vec3(-130,0,0), vec3(0,0,-105), light));
    world.Add(std::make_shared<Quad>(Point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.Add(std::make_shared<Quad>(Point3(555,555,555), vec3(-555,0,0), vec3(0,0,-555), white));
    world.Add(std::make_shared<Quad>(Point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    Camera cam;

    cam.AspectRatio      = 1.0;
    cam.ImageWidth       = 600;
    cam.SamplesPerPixel = 200;
    cam.MaxRayDepth         = 50;
    cam.Background        = Color(0,0,0);

    cam.VertFOV= 40;
    cam.LookFrom = Point3(278, 278, -800);
    cam.LookAt   = Point3(278, 278, 0);
    cam.VUp      = vec3(0,1,0);

    cam.DefocusAngle = 0;

    cam.Render(world);
}



int main() {
    int renderTarget = 6;


    switch(renderTarget) {
        case 1: BouncingSpheres(); break;
        case 2: CheckeredSpheres(); break;
        case 3: Earth(); break;
        case 4: NoiseMapping(); break;
        case 5: Quads(); break;
        case 6: SimpleLight(); break;
        case 7: CornellBox(); break;
    }
}
