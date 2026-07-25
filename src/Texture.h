//
// Created by Andrew Graser on 7/24/2026.
//


#pragma once

#include <memory>

#include "Color.h"
#include "util/RTWImage.h"
#include "util/Perlin.h"

class Texture {
public:
    virtual ~Texture() = default;

    virtual Color Value(double u, double v, const Point3& p) const = 0;
};


class TextureSolid : public Texture {
public:
    TextureSolid(const Color& albedo) : mAlbedo(albedo) {}
    TextureSolid(double r, double g, double b) : mAlbedo({r,g,b}) {}

    Color Value(double u, double v, const Point3& p) const override { return mAlbedo; }

private:
    Color mAlbedo;
};



class TextureCheckered : public Texture {
public:
    TextureCheckered(double scale, const std::shared_ptr<Texture>& even, const std::shared_ptr<Texture>& odd) : mInvScale(1.0 / scale), mEven(even), mOdd(odd) {}
    TextureCheckered(double scale, const Color& c1, const Color& c2) : TextureCheckered(scale, std::make_shared<TextureSolid>(c1), std::make_shared<TextureSolid>(c2)) {}

    Color Value(double u, double v, const Point3& p) const override {
        int xi = std::floor(mInvScale * p.x());
        int yi = std::floor(mInvScale * p.y());
        int zi = std::floor(mInvScale * p.z());

        bool isEven = (xi + yi + zi) % 2 == 0;

        return isEven ? mEven->Value(u,v,p) : mOdd->Value(u,v,p);
    }

private:
    std::shared_ptr<Texture> mEven;
    std::shared_ptr<Texture> mOdd;
    double mInvScale;
};



class TextureImage : public Texture {
public:
    TextureImage(const char* filename) : mImage(filename) {}

    Color Value(double u, double v, const Point3& p) const override {
        if(mImage.height() <= 0) return {0, 0, 1};

        u = Interval(0, 1).Clamp(u);
        v = 1.0 - Interval(0, 1).Clamp(v);

        int i = u * mImage.width();
        int j = v * mImage.height();
        auto pixel = mImage.pixel_data(i, j);

        double scale = 1.0 / 255.0;
        return Color(scale * pixel[0], scale * pixel[1], scale * pixel[2]);
    }

private:
    rtw_image mImage;
};



class TextureNoise : public Texture {
public:
    TextureNoise(double scale) : mScale(scale) {}

    Color Value(double u, double v, const Point3& p) const override {
        return Color(.5) * ( 1 + std::sin(mScale * p.z() + 10 * mNoise.Turb(p, 7)));
    }


private:
    Perlin mNoise;

    double mScale;
};