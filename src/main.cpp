#include <iostream>
#include "rtweekend.h"

#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <opencv2/opencv.hpp>

#define OUTPUT 1
#define MODE 1

int main() {


	hittable_list world;
    camera cam;

#if MODE == 0 // * Validation
    cam.image_width       = 360;
    cam.samples_per_pixel = 10;
#else // * Real Render
    cam.image_width       = 1080;
    cam.samples_per_pixel = 50;
#endif

    cam.aspect_ratio      = 1;
    cam.lookfrom          = point3(13,2,3);
    cam.lookat            = point3(0,0,0);
    cam.vup               = vec3(0,1,0);

    cam.defocus_angle     = 0.6;
    cam.focus_dist        = 10.0;

    auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<sphere>(point3(0,-1000,0), 1000, ground_material));

#if OUTPUT == 0


    for (int a = -8; a < 8; a++) {
        for (int b = -8; b < 8; b++) {
            auto choose_mat = random_double();
            point3 center(a + 1.9*random_double(), 0.2, b + 1.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

    auto material4 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(-4, 1, 3), 1.0, material4));


    cam.vfov              = 40;
    cam.fileName          = "Output1";


#elif OUTPUT == 1

    for (int a = -6; a < 6; a++) {
        for (int b = -6; b < 6; b++) {
            auto choose_mat = random_double();
            point3 center(a + 1.9*random_double(), 0.2, b + 1.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));
    
    cam.defocus_angle     = 0.2;
    cam.vfov              = 20;
    cam.focus_dist        = 0.1f;
    cam.fileName          = "Output2";


#elif OUTPUT == 2

    int max = random_float(5, 10);

    for (int a = 0; a < max; a++) {
            auto choose_mat = random_double();
            point3 center(0, 0.2 + a*.4f, 0);
            // point3 center(a + 1.9*random_double(), 0.2, b + 1.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            
        }
    }
    cam.vfov              = 30;
    cam.lookat            = point3(0,0.90f,0);
    cam.fileName          = "Output3";


#endif

    std::cout << "Spheres Created " << world.objects.size() << "\n";

    // cam.aspect_ratio      = 16.0f / 9.0f;
    // cam.fileName          = "MainOutput";


    cam.initialize(10);
    cam.render(world);

 
} 