//
// Created by Andrew Graser on 7/17/2026.
//


#pragma once


#include <cmath>
#include <cstdlib>
#include <random>


const double PI = 3.14159265359;

inline double ToRadians(double deg) {return deg * PI / 180.0;}

inline double RandomDouble01() {
    static thread_local std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static thread_local std::mt19937 generator;
    return distribution(generator);
}

inline double RandomDouble(double min, double max) {
    return min + (max - min) * RandomDouble01();
}

inline int RandomInt(int min, int max) {
    thread_local std::mt19937 generator;
    thread_local std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}