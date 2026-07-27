#include <iostream>

#include "Camera.h"
#include "Sphere.h"
#include "BvhNode.h"
#include "Quad.h"
#include "Mediums.h"


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

    std::shared_ptr<Hittable> box1 = Box(Point3(0), Point3(165, 330, 165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, vec3(265, 0, 295));
    world.Add(box1);

    std::shared_ptr<Hittable> box2 = Box(Point3(0), Point3(165, 165, 165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, vec3(130, 0, 65));
    world.Add(box2);

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

void CornellSmoke() {
    HittableList world;

    auto red   = std::make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = std::make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));

    world.Add(std::make_shared<Quad>(Point3(555,0,0), vec3(0,555,0), vec3(0,0,555), green));
    world.Add(std::make_shared<Quad>(Point3(0,0,0), vec3(0,555,0), vec3(0,0,555), red));
    world.Add(std::make_shared<Quad>(Point3(113,554,127), vec3(330,0,0), vec3(0,0,305), light));
    world.Add(std::make_shared<Quad>(Point3(0,555,0), vec3(555,0,0), vec3(0,0,555), white));
    world.Add(std::make_shared<Quad>(Point3(0,0,0), vec3(555,0,0), vec3(0,0,555), white));
    world.Add(std::make_shared<Quad>(Point3(0,0,555), vec3(555,0,0), vec3(0,555,0), white));

    std::shared_ptr<Hittable> box1 = Box(Point3(0,0,0), Point3(165,330,165), white);
    box1 = std::make_shared<RotateY>(box1, 15);
    box1 = std::make_shared<Translate>(box1, vec3(265,0,295));

    std::shared_ptr<Hittable> box2 = Box(Point3(0,0,0), Point3(165,165,165), white);
    box2 = std::make_shared<RotateY>(box2, -18);
    box2 = std::make_shared<Translate>(box2, vec3(130,0,65));

    world.Add(std::make_shared<ConstantMedium>(box1, 0.01, Color(0,0,0)));
    world.Add(std::make_shared<ConstantMedium>(box2, 0.01, Color(1,1,1)));

    Camera cam;

    cam.AspectRatio      = 1.0;
    cam.ImageWidth       = 600;
    cam.SamplesPerPixel = 200;
    cam.MaxRayDepth         = 50;
    cam.Background        = Color(0,0,0);

    cam.VertFOV     = 40;
    cam.LookFrom = Point3(278, 278, -800);
    cam.LookAt   = Point3(278, 278, 0);
    cam.VUp      = vec3(0,1,0);

    cam.DefocusAngle = 0;

    cam.Render(world);
}

void MultipleTextured(int image_width, int samples_per_pixel, int max_depth) {
    HittableList boxes1;
    auto ground = std::make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i*w;
            auto z0 = -1000.0 + j*w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = RandomDouble(1,101);
            auto z1 = z0 + w;

            boxes1.Add(Box(Point3(x0,y0,z0), Point3(x1,y1,z1), ground));
        }
    }

    HittableList world;

    world.Add(std::make_shared<BvHNode>(boxes1));

    auto light = std::make_shared<DiffuseLight>(Color(7, 7, 7));
    world.Add(std::make_shared<Quad>(Point3(123,554,147), vec3(300,0,0), vec3(0,0,265), light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + vec3(30,0,0);
    auto sphere_material = std::make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    world.Add(std::make_shared<Sphere>(center1, center2, 50, sphere_material));

    world.Add(std::make_shared<Sphere>(Point3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));
    world.Add(std::make_shared<Sphere>(
            Point3(0, 150, 145), 50, std::make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
    ));

    auto boundary = std::make_shared<Sphere>(Point3(360,150,145), 70, std::make_shared<Dielectric>(1.5));
    world.Add(boundary);
    world.Add(std::make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = std::make_shared<Sphere>(Point3(0,0,0), 5000, std::make_shared<Dielectric>(1.5));
    world.Add(std::make_shared<ConstantMedium>(boundary, .0001, Color(1,1,1)));

    auto emat = std::make_shared<Lambertian>(std::make_shared<TextureImage>("earthmap.jpg"));
    world.Add(std::make_shared<Sphere>(Point3(400,200,400), 100, emat));
    auto pertext = std::make_shared<TextureNoise>(0.2);
    world.Add(std::make_shared<Sphere>(Point3(220,280,300), 80, std::make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white = std::make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.Add(std::make_shared<Sphere>(Point3::Random(0,165), 10, white));
    }

    world.Add(std::make_shared<Translate>(
                      std::make_shared<RotateY>(
                              std::make_shared<BvHNode>(boxes2), 15),
                      vec3(-100,270,395)
              )
    );

    Camera cam;

    cam.AspectRatio      = 1.0;
    cam.ImageWidth       = image_width;
    cam.SamplesPerPixel = samples_per_pixel;
    cam.MaxRayDepth         = max_depth;
    cam.Background        = Color(0,0,0);

    cam.VertFOV     = 40;
    cam.LookFrom = Point3(478, 278, -600);
    cam.LookAt   = Point3(278, 278, 0);
    cam.VUp      = vec3(0,1,0);

    cam.DefocusAngle = 0;

    cam.Render(world);
}

int main() {
    int renderTarget = 91;


    switch(renderTarget) {
        case 1: BouncingSpheres(); break;
        case 2: CheckeredSpheres(); break;
        case 3: Earth(); break;
        case 4: NoiseMapping(); break;
        case 5: Quads(); break;
        case 6: SimpleLight(); break;
        case 7: CornellBox(); break;
        case 8: CornellSmoke(); break;
        case 9: MultipleTextured(800, 10000, 40); break;


        default: MultipleTextured(400, 250, 4); break;
    }
}
