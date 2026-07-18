//
// Created by Andrew Graser on 7/17/2026.
//


#pragma once

#include <fstream>


#include "Hittable.h"
#include "Color.h"
#include "Util.h"


class Camera {
public:
    void Render(const Hittable& world) {
        Init();

        std::ofstream ppmImage("../ppmImage.ppm");

        ppmImage << "P3\n" << ImageWidth << ' ' << ImageHeight << "\n255\n";

        for (int j = 0; j < ImageHeight; ++j) {
            std::clog << "\rScanlines remaining: " << (ImageHeight - j) << ' ' << std::flush;
            for (int i = 0; i < ImageWidth; ++i) {

                Color pixelColor(0.0);
                for (int sample = 0; sample < SamplesPerPixel; ++sample) {
                    Ray r = GetRay(i,j);
                    pixelColor += RayColor(r, world);
                }
                write_color(ppmImage, pixelColor * mPixelSamplesScale);
            }
        }

        ppmImage.close();
    }

private:
    void Init() {
        ImageHeight = int(ImageWidth / AspectRatio);
        ImageHeight = (ImageHeight < 1) ? 1 : ImageHeight;

        mPixelSamplesScale = 1.0 / SamplesPerPixel;


        double focalLength = 1.0;
        double viewportHeight = 2.0;
        double viewportWidth = viewportHeight * ((double)ImageWidth / ImageHeight);

        mViewportU = vec3(viewportWidth, 0, 0);
        mViewportV = vec3(0, -viewportHeight, 0);

        mPixelDeltaU = mViewportU / ImageWidth;
        mPixelDeltaV = mViewportV / ImageHeight;

        Point3 viewportUpperLeft = mCenter -
                                   vec3(0, 0, focalLength) - mViewportU / 2.0 - mViewportV / 2.0;

        mPixel00Loc = viewportUpperLeft + 0.5 * (mPixelDeltaU + mPixelDeltaV);
    }

    Color RayColor(const Ray& r, const Hittable& world) const {
        HitRecord rec;
        if(world.Hit(r, Interval(0, INFINITY), rec)) {
            return 0.5 * (rec.Normal + Color(1));
        }

        vec3 unitDirection = UnitVector(r.Direction());
        auto a = 0.5*(unitDirection.y() + 1.0);
        return (1.0 - a) * Color(1.0) + a * Color(0.5, 0.7, 1.0);
    }

    Ray GetRay(int x, int y) {
        vec3 offset = SampleSquare();
        vec3 pixelSample = mPixel00Loc +
                ((x + offset.x()) * mPixelDeltaU) +
                ((y + offset.y()) * mPixelDeltaV);

        Point3 rayOrigin = mCenter;
        vec3 rayDirection = pixelSample - rayOrigin;

        return Ray(rayOrigin, rayDirection);
    }

    vec3 SampleSquare() {
        return {RandomDouble01() - 0.5, RandomDouble01() - 0.5, 0.0};
    }


public:
    double AspectRatio = 16.0 / 9.0;
    int ImageWidth = 400;
    int SamplesPerPixel = 10;


private:
    int ImageHeight;
    double mPixelSamplesScale;

    Point3 mCenter {0.0}, mPixel00Loc;
    vec3 mPixelDeltaU, mPixelDeltaV;
    vec3 mViewportU, mViewportV;

};