#include "rtweekend.h"

#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>

int main()
{

    // World

    hittable_list world;

    world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    Camera camera;
    camera.aspect_ratio = 16.0 / 9.0;
    camera.image_width = 400;
    camera.sample_cnt = 100;

    camera.render(world);
}