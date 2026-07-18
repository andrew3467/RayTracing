//
// Created by Andrew Graser on 7/16/2026.
//


#pragma once


#include "vec3.h"


using Color = vec3;

void write_color(std::ostream& out, const Color& pixelColor) {
    double r = pixelColor.x();
    double g = pixelColor.y();
    double b = pixelColor.z();

    static const Interval intensity(0.000, 0.999);
    int rbyte = int(256 * intensity.Clamp(r));
    int gbyte = int(256 * intensity.Clamp(g));
    int bbyte = int(256 * intensity.Clamp(b));

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}