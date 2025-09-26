//
// Created by Anchit Mishra on 2023-06-11.
//

#ifndef LUMINA_MATERIAL_H
#define LUMINA_MATERIAL_H

#include <lumina.h>
#include <hittable.h>
#include <texture.h>

struct hit_record;

// Abstract class definition for materials
class material  {
public:
    virtual bool scatter(const ray& ray_in, const hit_record& hit_rec, color3& attenuation, ray& scattered_light) const = 0;
};

// Class definition for lambertian/diffuse/matte style materials
class lambertian : public material  {
public:
    lambertian(const color3& albedo) : tex(make_shared<solid_color>(albedo)) {}
    lambertian(shared_ptr<texture> tex) : tex(tex) {}
//    color3 albedo;
    virtual bool scatter(const ray& ray_in, const hit_record& hit_rec, color3& attenuation, ray& scattered_light) const override    {
        const auto timestamp = ray_in.timestamp;
        auto scatter_direction = hit_rec.normal + random_unit_vector();
        if (scatter_direction.near_zero())  scatter_direction = hit_rec.normal;
        point3 origin = hit_rec.point;
        scattered_light = ray(origin, scatter_direction, timestamp);
        attenuation = tex->value(hit_rec.u, hit_rec.v, hit_rec.point);
        return true;
    }
private:
    shared_ptr<texture> tex;
};

// Class definition for metallic/perfectly reflective materials
class metal : public material   {
public:
    color3 albedo; // for base color of metal
    double fuzz; // for blurriness of reflection from metal
    metal(const color3& color, const double f): albedo(color), fuzz(f < 1 ? f : 1) {}
    virtual bool scatter(const ray& ray_in, const hit_record& hit_rec, color3& attenuation, ray& scattered_light) const override    {
        vec3 reflected = reflect(unit(ray_in.direction), hit_rec.normal);
        point3 origin = hit_rec.point;
        vec3 direction = reflected + fuzz * random_in_unit_sphere();
        scattered_light = ray(origin, direction, ray_in.timestamp);
        attenuation = albedo;
        return (dot(scattered_light.direction, hit_rec.normal) > 0);
    }
};

//Class definition for dielectric (clear/translucent) materials (e.g. glass, diamond etc.)
class dielectric: public material   {
public:
    double eta; // refractive index
    dielectric(const double n): eta(n) {}
    virtual bool scatter(const ray& ray_in, const hit_record& hit_rec, color3& attenuation, ray& scattered_light) const override  {
        attenuation = color3(1.0, 1.0, 1.0);
        double refraction_ratio = hit_rec.front_face ? (1.0 / eta) : eta;
        double cos_theta = fmin(dot(-ray_in.direction, hit_rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        vec3 direction;
        vec3 unit_direction = unit(ray_in.direction);
        bool no_refraction = refraction_ratio * sin_theta > 1.0;
        if (no_refraction || reflectance(cos_theta, refraction_ratio) > random_double())
            // reflection must occur
            direction = reflect(unit_direction, hit_rec.normal);
        else
            // can refract light here
            direction = refract(unit_direction, hit_rec.normal, refraction_ratio);
        point3 point = hit_rec.point;
        scattered_light = ray(point, direction, ray_in.timestamp);
        return true;
    }
private:
    static double reflectance(double cos, double eta)   {
        // Christophe Schlick's polynomial approximation for when and how much a dielectric reflects
        // Source: https://en.wikipedia.org/wiki/Schlick%27s_approximation
        auto r0 = (1 - eta) / (1 + eta);
        r0 *= r0;
        return r0 + (1 - r0) * pow(1 - cos, 5);
    }
};

#endif //LUMINA_MATERIAL_H
