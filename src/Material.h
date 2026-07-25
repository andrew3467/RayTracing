//
// Created by Andrew Graser on 7/17/2026.
//


#pragma once

#include "Hittable.h"
#include "Color.h"
#include "Texture.h"


struct Material {
    virtual ~Material() = default;

    virtual bool Scatter(
            const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered
            ) const {
        return false;
    }


    virtual Color Emitted(double u, double v, const Point3& p) const {
        return {0, 0, 0};
    }
};

struct Lambertian : public Material {
    Lambertian(const Color& albedo) : Texture(std::make_shared<TextureSolid>(albedo)) {}
    Lambertian(const std::shared_ptr<Texture>& tex) : Texture(tex) {}

    bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        vec3 scatterDirection = rec.Normal + RandomUnitVector();

        if(scatterDirection.NearZero()) scatterDirection = rec.Normal;

        scattered = Ray(rec.Point, scatterDirection, rayIn.Time());
        attenuation = Texture->Value(rec.U, rec.V, rec.Point);
        return true;
    }


    std::shared_ptr<Texture> Texture;
};

struct Metal : public Material {
    Metal(const Color& albedo, double fuzz) : Albedo(albedo), Fuzz(fuzz < 1 ? fuzz : 1) {}

    bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        vec3 reflected = Reflect(rayIn.Direction(), rec.Normal);
        reflected = UnitVector(reflected) + (Fuzz * RandomUnitVector());
        scattered = Ray(rec.Point, reflected, rayIn.Time());
        attenuation = Albedo;
        return dot(scattered.Direction(), rec.Normal) > 0;
    }

    Color Albedo;
    double Fuzz;
};

struct Dielectric : public Material {
    Dielectric(double refractionIndex) : RefractionIndex(refractionIndex) {}

    bool Scatter(const Ray& rayIn, const HitRecord& rec, Color& attenuation, Ray& scattered) const override {
        attenuation = Color(1.0);
        double ri = rec.FrontFace ? (1.0 / RefractionIndex) : RefractionIndex;

        vec3 unitDirection = UnitVector(rayIn.Direction());
        double cosTheta = std::fmin(dot(-unitDirection, rec.Normal), 1.0);
        double sinTheta = std::sqrt(1.0 - cosTheta * cosTheta);

        bool cannotRefract = ri * sinTheta > 1.0;
        vec3 direction;

        if(cannotRefract || Reflectance(cosTheta, ri) > RandomDouble01())
            direction = Reflect(unitDirection, rec.Normal);
        else
            direction = Refract(unitDirection, rec.Normal, ri);

        scattered = Ray(rec.Point, direction, rayIn.Time());
        return true;
    }

    double RefractionIndex;


    static double Reflectance(double cosine, double refractionIndex) {
        auto r0 = (1 - refractionIndex) / (1 + refractionIndex);
        r0 = r0 * r0;

        return r0 + (1 - r0) * std::pow((1 - cosine), 5);
    }
};



class DiffuseLight : public Material {
public:
    DiffuseLight(std::shared_ptr<Texture>& tex) : mTexture(tex) {}
    DiffuseLight(const Color& emit) : mTexture(std::make_shared<TextureSolid>(emit)) {}

    Color Emitted(double u, double v, const Point3& p) const override {
        return mTexture->Value(u, v, p);
    }


private:
    std::shared_ptr<Texture> mTexture;
};