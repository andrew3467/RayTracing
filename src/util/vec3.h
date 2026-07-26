//
// Created by Andrew Graser on 7/16/2026.
//


#pragma once


#include <cmath>
#include <iostream>
#include <ostream>
#include "Util.h"


class vec3 {
public:
    vec3() : e{0,0,0} {}
    vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}
    vec3(double e) : e{e, e, e} {}

    double x() const {return e[0];}
    double y() const {return e[1];}
    double z() const {return e[2];}

    vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec3& operator+=(const vec3& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    vec3& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    vec3& operator/=(double t) {
        return *this *= 1/t;
    }

    double length() const {
        return std::sqrt(LengthSquared());
    }

    double LengthSquared() const {
        return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
    }

    bool NearZero() const {
        // Return true if the vector is close to zero in all dimensions.
        auto s = 1e-8;
        return (std::fabs(e[0]) < s) && (std::fabs(e[1]) < s) && (std::fabs(e[2]) < s);
    }

    static vec3 Random() {return {RandomDouble01(), RandomDouble01(), RandomDouble01()};}

    static vec3 Random(double min, double max) {return {RandomDouble(min, max), RandomDouble(min, max), RandomDouble(min, max)};}


    double e[3];



};
using Point3 = vec3;

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.e[0] * v.e[0]
           + u.e[1] * v.e[1]
           + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 UnitVector(const vec3& v) {
    return v / v.length();
}

inline vec3 RandomUnitVector() {
    while (true) {
        vec3 P = vec3::Random(-1.0, 1.0);
        double lensq = P.LengthSquared();
        //TODO: account for floating point imprecision at extreme lows
        if(lensq <= 1) {
            return P / sqrt(lensq);
        }
    }
}

inline vec3 RandomOnHemisphere(const vec3& normal) {
    vec3 onUnitSphere = RandomUnitVector();
    if(dot(onUnitSphere, normal) > 0.0) {
        return onUnitSphere;
    }else {
        return -onUnitSphere;
    }
}

inline vec3 RandomInUnitDisk() {
    while (true) {
        auto p = vec3(RandomDouble(-1, 1), RandomDouble(-1, 1), 0);
        if(p.LengthSquared() < 1) return p;
    }
}

inline vec3 Reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline vec3 Refract(const vec3& uv, const vec3& n, double etaiOveretat) {
    auto cosTheta = std::fmin(dot(-uv, n), 1.0);
    vec3 outPerp = etaiOveretat * (uv + cosTheta * n);
    vec3 outParallel = -std::sqrt(std::fabs(1.0 - outPerp.LengthSquared())) * n;
    return outPerp + outParallel;
}

inline Point3 Min(const Point3& a, const Point3& b) {
    return {
            std::fmin(a.x(), b.x()),
            std::fmin(a.y(), b.y()),
            std::fmin(a.z(), b.z())
    };
}

inline Point3 Max(const Point3& a, const Point3& b) {
    return {
            std::fmax(a.x(), b.x()),
            std::fmax(a.y(), b.y()),
            std::fmax(a.z(), b.z())
    };
}
