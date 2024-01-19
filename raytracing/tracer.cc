#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

#include <iostream>

int main()
{

    // World

    hittable_list world;

    // auto R = cos(pi / 4);

    // auto material_left = make_shared<lambertian>(color(0, 0, 1));
    // auto material_right = make_shared<lambertian>(color(1, 0, 0));

    // world.add(make_shared<sphere>(point3(-R, 0, -1), R, material_left));
    // world.add(make_shared<sphere>(point3(R, 0, -1), R, material_right));

    auto material_ground = make_shared<lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.0),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    Camera camera;
    camera.aspect_ratio = 16.0 / 9.0;
    camera.image_width = 400;
    camera.sample_cnt = 100;
    camera.max_rec_depth = 50;

    camera.vfov = 20;
    camera.lookfrom = point3(-2, 2, 1);
    camera.lookat = point3(0, 0, -1);
    camera.vup = vec3(0, 1, 0);

    camera.defocus_angle = 10.0;
    camera.focus_dist = 3.5;

    camera.render(world);
}