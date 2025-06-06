#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtweekend.h"

class material
{
    public:
        virtual ~material() = default;
        
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material
{
    public:
        lambertian(const color& a) : albedo(a) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
        {
            auto scatter_direction = rec.normal + random_unit_vector();

            if(scatter_direction.near_zero())
                scatter_direction = rec.normal;

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }
    private:
        color albedo;
};

class metal : public material
{
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
        {
            auto reflect_dir = reflect(unit_vector(r_in.direction()), rec.normal);
            reflect_dir = reflect_dir + fuzz * random_in_unit_sphere();
            scattered = ray(rec.p, reflect_dir);
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    private:
        color albedo;
        double fuzz;
};

class dielectric : public material
{
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}

        bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
        {
            // 折射暂时没有衰减
            attenuation = color(1.0, 1.0, 1.0);
            // judge if the normal is inside the sphere or not.
            double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

            vec3 unit_direction = unit_vector(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = refraction_ratio * sin_theta > 1.0;
            vec3 direction;

            // 这样不是随机反射折射了么？
            if(cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double())
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, direction);
            return true;
        }

    private:
        double ir; // Index of refraction;

        static double reflectance(double cosine, double ref_idx)
        {
            auto r0 = (1 - ref_idx) / (1 + ref_idx);
            r0 = r0 * r0;
            return r0 + (1 - r0) * pow((1 - cosine), 5);
        }
};

#endif