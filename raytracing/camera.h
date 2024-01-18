#ifndef CAMERA_H
#define CAMERA_H

#include "rtweekend.h"

#include "color.h"
#include "hittable.h"
#include "material.h"

#include "iostream"

class Camera
{
    public:
        double aspect_ratio = 1.0; // ratio of image width over image height
        int image_width = 100; // Render image width in pixel count
        int sample_cnt = 10;
        int max_rec_depth = 10;

        double vfov = 90; // vertical field of view
        point3 lookfrom = point3(0, 0, -1);
        point3 lookat = point3(0, 0, 0);
        point3 vup = vec3(0, 1, 0);

        double defocus_angle = 0; // Variation angle of rays through each pixel
        double focus_dist = 10; // Distance from camera lookfrom point to plane of perfect focus

        void render(const hittable& world)
        {
            initialize();

            std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

            for(int j = 0; j < image_height; ++j)
            {
                std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
                for(int i = 0; i < image_width; ++i)
                {
                    // auto pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
                    // auto ray_direction = pixel_center - center;
                    // ray r(center, ray_direction);
                    color pixel_color(0, 0, 0);
                    for(int s = 0; s < sample_cnt; ++s)
                    {
                        ray r = get_ray(i, j);
                        pixel_color += ray_color(r, max_rec_depth, world);
                    }
                    write_color(std::cout, pixel_color, sample_cnt);
                }
            }

            std::clog << "\rDone.                                          \n";
        }

    private:
        int image_height; // Rendered image height
        point3 center;    // Camera Center
        point3 pixel00_loc; // 0,0 pixel location
        vec3 pixel_delta_u;
        vec3 pixel_delta_v;
        vec3 u, v, w; // the right-handed camera axis.

        vec3 defocus_disk_u; // Defocus disk horizontal radius
        vec3 defocus_disk_v; // Defocus disk vertical radius

        void initialize()
        {
            image_height = static_cast<int>(image_width / aspect_ratio);
            image_height = (image_height > 1) ? image_height : 1;

            center = lookfrom;

            // Determine viewport dimensions
            
            auto theta = degrees_to_radians(vfov);
            auto h = tan(theta / 2) * focus_dist;
            auto viewport_height = 2 * h;
            auto viewport_width = viewport_height * (static_cast<double>(image_width) / image_height);

            // Calculate the u, v, w unit basis vectors for the camera coordinate frame.
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = unit_vector(cross(w, u));

            // Calculate the vectors across the horizontal and down the vertical viewport edges.
            auto viewport_u = viewport_width * u;
            auto viewport_v = viewport_height * -v;



            // Calculate the horizontal and vertical delta vectors from pixel to pixel
            // 方向向量除以图像的宽度和图像长度，就得到了每次变化需要添加的增加量。
            pixel_delta_u = viewport_u / image_width;   
            pixel_delta_v = viewport_v / image_height;

            // Calculate the location of the upper left pixel
            auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
            pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

            // Calculate the camera defocus disk basis vector
            auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;
        }

        ray get_ray(int i, int j) const
        {
            // Get a randomly-sampled camera ray for the pixel at location i,j, originating from 
            // the camera defocus disk.
            auto pixel_center = pixel00_loc + i * pixel_delta_u + j * pixel_delta_v;
            auto pixel_sample = pixel_center + pixel_sample_square();

            auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
            auto ray_direction = pixel_sample - ray_origin;

            return ray(ray_origin, ray_direction);
        }

        point3 defocus_disk_sample() const
        {
            // Returns a random point in the camera defocus disk
            auto p = random_in_unit_disk();
            return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
        }

        vec3 pixel_sample_square() const
        {
            // Returns a random point in the square surrounding a pixel at the origin.
            auto px = -0.5 + random_double();
            auto py = -0.5 + random_double();
            return (px * pixel_delta_u) + (py * pixel_delta_v);
        }

        color ray_color(const ray& r, int depth_cnt, const hittable& world) const 
        {
            if(depth_cnt <= 0)
                return color(0, 0, 0);

            hit_record rec;

            if(world.hit(r, interval(0.001, infinity), rec))
            {
                ray scattered;
                color attenuation;
                if(rec.mat->scatter(r, rec, attenuation, scattered))
                {
                    return attenuation * ray_color(scattered, depth_cnt - 1, world);
                }
                return color(0, 0, 0); // 此时看到的光线是原来入射那边的颜色，因此相当于我们从视线去推测入射光线，然后再使用入射光线去和世界做相交。
            }

            vec3 unit_direction = unit_vector(r.direction());
            auto a = 0.5 * (unit_direction.y() + 1);
            return (1.0 - a)*color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
        }
};

#endif