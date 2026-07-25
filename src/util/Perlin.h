//
// Created by Andrew Graser on 7/24/2026.
//


#pragma once

#include "Util.h"
#include "vec3.h"




class Perlin {
public:
    Perlin() {
        for (int i = 0; i < PointCount; i++) {
            RandVec[i] = UnitVector(vec3::Random(-1, 1));
        }

        PerlinGeneratePerm(perm_x);
        PerlinGeneratePerm(perm_y);
        PerlinGeneratePerm(perm_z);
    }

    double Noise(const Point3& p) const {
        double u = p.x() - std::floor(p.x());
        double v = p.y() - std::floor(p.y());
        double w = p.z() - std::floor(p.z());


        int i = (int)std::floor(p.x());
        int j = (int)std::floor(p.y());
        int k = (int)std::floor(p.z());
        vec3 c[2][2][2];

        for(int di = 0; di < 2; di++)
        for(int dj = 0; dj < 2; dj++)
        for(int dk = 0; dk < 2; dk++)
            c[di][dj][dk] = RandVec[
                        perm_x[(i+di) & 255] ^
                        perm_y[(j+dj) & 255] ^
                        perm_z[(k+dk) & 255]
                    ];

        return PerlinInterp(c, u, v, w);
    }

    double Turb(const Point3& p, int depth) const {
        auto accum = 0.0;
        auto tempP = p;
        auto weight = 1.0;

        for (int i = 0; i < depth; ++i) {
            accum += weight * Noise(tempP);
            weight *= 0.5;
            tempP *= 2;
        }

        return std::fabs(accum);
    }


private:
    static const int PointCount = 256;
    vec3 RandVec[PointCount];
    int perm_x[PointCount];
    int perm_y[PointCount];
    int perm_z[PointCount];

    static void PerlinGeneratePerm(int* p) {
        for (int i = 0; i < PointCount; i++)
            p[i] = i;

        Permute(p, PointCount);
    }

    static void Permute(int* p, int n) {
        for (int i = n-1; i > 0; i--) {
            int target = RandomInt(0, i);
            int tmp = p[i];
            p[i] = p[target];
            p[target] = tmp;
        }
    }

    static double PerlinInterp(const vec3 c[2][2][2], double u, double v, double w) {
        auto uu = u*u*(3-2*u);
        auto vv = v*v*(3-2*v);
        auto ww = w*w*(3-2*w);
        auto accum = 0.0;

        for (int i=0; i < 2; i++)
            for (int j=0; j < 2; j++)
                for (int k=0; k < 2; k++) {
                    vec3 weight_v(u-i, v-j, w-k);
                    accum += (i*uu + (1-i)*(1-uu))
                             * (j*vv + (1-j)*(1-vv))
                             * (k*ww + (1-k)*(1-ww))
                             * dot(c[i][j][k], weight_v);
                }

        return accum;
    }
};