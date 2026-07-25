//
// Created by Andrew Graser on 7/17/2026.
//


#pragma once

#include <fstream>
#include <thread>


#include "Hittable.h"
#include "Material.h"
#include "util/Util.h"


class Camera {
public:
    void Render(const Hittable& world) {
        Init();



        std::vector<Color> output(ImageHeight *ImageWidth);

#if 0
        for (int i = 0; i < ImageHeight; ++i) {
            RenderRow(i, world, output);
        }

#else
        std::atomic<int> nextRow{0};

        unsigned int threadCount = std::thread::hardware_concurrency();
        std::vector<std::thread> threads;
        threads.reserve(threadCount);
        auto thread = [&]() {
            while (true) {
                int y = nextRow.fetch_add(1, std::memory_order_relaxed);

                if (y >= ImageHeight)
                    return;

                RenderRow(y, std::ref(world), std::ref(output));
            }
        };


        for(int i = 0; i < threadCount; i++)
            threads.emplace_back(thread);

        for(auto& t : threads)
            t.join();
#endif

        std::ofstream ppmImage("../ppmImage.ppm");

        ppmImage << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n255\n";
        for (int y = 0; y < ImageHeight; ++y) {
            for (int x = 0; x < ImageWidth; ++x) {
                write_color(ppmImage, output[y * ImageWidth + x] * mPixelSamplesScale);
            }
        }

        ppmImage.close();
    }

private:
    void Init() {
        ImageHeight = int(ImageWidth / AspectRatio);
        ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;

        mPixelSamplesScale = 1.0 / SamplesPerPixel;

        mCenter = LookFrom;


        double theta = ToRadians(VertFOV);
        double h = std::tan(theta / 2);
        double viewportHeight = 2.0 * h * FocusDist;
        double viewportWidth = viewportHeight * ((double)ImageWidth / ImageHeight);

        w = UnitVector(LookFrom - LookAt);
        u = UnitVector(cross(VUp, w));
        v = cross(w, u);

        mViewportU = viewportWidth * u;
        mViewportV = viewportHeight * -v;

        mPixelDeltaU = mViewportU / ImageWidth;
        mPixelDeltaV = mViewportV / ImageHeight;

        Point3 viewportUpperLeft = mCenter - (FocusDist * w) - mViewportU / 2.0 - mViewportV / 2.0;

        mPixel00Loc = viewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);


        auto defocusRadius = FocusDist * std::tan(ToRadians(DefocusAngle / 2.0));
        mDefocusDiskU = u * defocusRadius;
        mDefocusDiskV = v * defocusRadius;
    }

    void RenderRow(int y, const Hittable& world, std::vector<Color>& output) {
        for (int x = 0; x < ImageWidth; ++x) {
            Color pixelColor(0.0);
            for (int sample = 0; sample < SamplesPerPixel; ++sample) {
                Ray r = GetRay(x, y);
                pixelColor += RayColor(r, MaxRayDepth, world);
            }

            output[y * ImageWidth + x] = pixelColor;
        }

        std::clog << "Rendered Row: " << y << std::endl << std::flush;
    }

    Color RayColor(const Ray& r, int depth, const Hittable& world) const {
        if(depth <= 0) return Color(0.0);

        HitRecord rec;
        if(!world.Hit(r, Interval(0.001, INFINITY), rec)) return Background;


        Ray scattered;
        Color attenuation;
        Color emissionColor = rec.Material->Emitted(rec.U, rec.V, rec.Point);


        if(!rec.Material->Scatter(r, rec, attenuation, scattered))  return emissionColor;

        Color scatterColor = attenuation * RayColor(scattered, depth - 1, world);
        //Color scatterColor = attenuation * bounced;
        return emissionColor + scatterColor;
    }

    Ray GetRay(int x, int y) {
        vec3 offset = SampleSquare();
        vec3 pixelSample = mPixel00Loc +
                ((x + offset.x()) * mPixelDeltaU) +
                ((y + offset.y()) * mPixelDeltaV);

        Point3 rayOrigin = (DefocusAngle <= 0) ? mCenter : DefocusDiskSample();
        vec3 rayDirection = pixelSample - rayOrigin;
        double rayTime = RandomDouble01();

        return Ray(rayOrigin, rayDirection, rayTime);
    }

    vec3 SampleSquare() {
        return {RandomDouble01() - 0.5, RandomDouble01() - 0.5, 0.0};
    }

    vec3 DefocusDiskSample() const {
        auto p = RandomInUnitDisk();
        return mCenter + (p[0] * mDefocusDiskU) + (p[1] * mDefocusDiskV);
    }


public:
    double AspectRatio = 16.0 / 9.0;
    int ImageWidth = 400;
    int SamplesPerPixel = 10;
    int MaxRayDepth = 10;
    Color Background;

    double VertFOV = 90;
    Point3 LookFrom = {0,0,0};
    Point3 LookAt = {0,0,-1};
    vec3 VUp = {0,1,0};

    double DefocusAngle = 0;
    double FocusDist = 10;


private:
    int ImageHeight;
    double mPixelSamplesScale;

    Point3 mCenter {0.0}, mPixel00Loc;
    vec3 mPixelDeltaU, mPixelDeltaV;
    vec3 mViewportU, mViewportV;

    vec3 u, v, w;

    vec3 mDefocusDiskU, mDefocusDiskV;

};