//
// Created by Anchit Mishra on 2023-06-11.
//

#ifndef LUMINA_MATERIAL_H
#define LUMINA_MATERIAL_H

#include <lumina.h>

struct hit_record;

// Abstract class definition for materials
class material  {
public:
    virtual bool scatter(const ray& ray_in, const hit_record& hit_rec, color3& attenuation, ray& scattered_light) const = 0;
};

// Class definition for lambertian/diffuse/matte style materials
class lambertian : public material  {
public:
    color3 albedo;
    lambertian(const color3& color): albedo(color) {}
    virtual bool scatter(const ray& ray_in, const hit_record& hit_rec, color3& attenuation, ray& scattered_light) const override    {
        auto scatter_direction = hit_rec.normal + random_unit_vector();
        if (scatter_direction.near_zero())  scatter_direction = hit_rec.normal;
        point3 origin = hit_rec.point;
        scattered_light = ray(origin, scatter_direction);
        attenuation = albedo;
        return true;
    }
};

// Class definition for metallic/perfectly reflective materials
class metal : public material   {
public:
    color3 albedo;
    metal(const color3& color): albedo(color) {}
    virtual bool scatter(const ray& ray_in, const hit_record& hit_rec, color3& attenuation, ray& scattered_light) const override    {
        vec3 reflected = reflect(unit(ray_in.direction), hit_rec.normal);
        point3 origin = hit_rec.point;
        scattered_light = ray(origin, reflected);
        attenuation = albedo;
        return (dot(scattered_light.direction, hit_rec.normal) > 0);
    }
};

#endif //LUMINA_MATERIAL_H
