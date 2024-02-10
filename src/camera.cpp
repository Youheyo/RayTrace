#include "camera.h"
#include "RenderThread.h"

camera::camera()
{
}

camera::~camera()
{
}

void camera::render(hittable_list world)
{

    std::cout << "P3\n" << image_width << ' ' << image_height << "\nMax Threads: " << this->threads << "\n";

    RenderImage* image = new RenderImage(image_width, image_height);

    int row = 0;
    bool standby = true , isBusy = false;
    std::vector<RenderThread*> rThread;
    ShowProgress(row);

    while(standby){
        for(int i = 0; i < this->threads; i++){
            if(rThread.size() < this->threads){
                RenderThread* thread = new RenderThread(rThread.size(), image, this, &world);
                rThread.push_back(thread);
                thread->setRow(row);
                thread->Start();
                row++;
                ShowProgress(row);
            }
            if(!rThread[i]->isRunning()){                    // * Check if a thread is finished running

                if(rThread[i]->canApplyPixels){              // * Check if a thread can apply pixels

                    for(int z = 0; z < rThread.size(); z++){ // * Loop Through list of threads

                        if(rThread[z]->applyingPixels){      // * Check if any are applying pixels
                            // std::cout << rThread[i]->id << " cannot apply because " << rThread[z]->id << " using CS\n";
                            isBusy = true;
                            break;
                        }

                    }

                    if(!isBusy){
                        rThread[i]->applyPixels(image);      // * Apply pixels
                        if(row < image_height){              // * Check left over rows
                            rThread[i]->setRow(row);         // * Assign a new row then start
                            rThread[i]->Start();
                            row++;
                            ShowProgress(row);
                        }
                        else{                               // * No more rows can be added
                            standby = false;
                            break;
                        }
                    }


                }
            }
        }
    }
    image->saveImage(fileName);
    std::clog << "\rDone.                 \n";
}

void camera::initialize(int numThread)
{

    this->threads = numThread;

    image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    center = lookfrom;

    // Determine viewport dimensions.
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta/2);
    auto viewport_height = 2 * h * focus_dist;
    auto viewport_width = viewport_height * (static_cast<double>(image_width)/image_height);

    // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
    w = unit_vector(lookfrom - lookat);
    u = unit_vector(cross(vup, w));
    v = cross(w, u);

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
    vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = center - (focus_dist * w) - viewport_u/2 - viewport_v/2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Calculate the camera defocus disk basis vectors.
    auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;

}

ray camera::get_ray(int i, int j) const
{
    // Get a randomly-sampled camera ray for the pixel at location i,j, originating from
    // the camera defocus disk.

    auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
    auto pixel_sample = pixel_center + pixel_sample_square();

    auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
}

void camera::ShowProgress(int row)
{
    std::clog << "\rScanlines remaining: " << (image_height - row) << ' ' << std::flush;
}

point3 camera::defocus_disk_sample() const
{
    // Returns a random point in the camera defocus disk.
    auto p = random_in_unit_disk();
    return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
}

vec3 camera::pixel_sample_square() const
{
    // Returns a random point in the square surrounding a pixel at the origin.
    auto px = -0.5 + random_double();
    auto py = -0.5 + random_double();
    return (px * pixel_delta_u) + (py * pixel_delta_v);
}

color camera::ray_color(const ray &r, int depth, const hittable_list world) const
{
    hit_record rec;

    // If we've exceeded the ray bounce limit, no more light is gathered.
    if (depth <= 0)
        return color(0,0,0);

    if (world.hit(r, interval(0.001, infinity), rec)) {
        ray scattered;
        color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, depth-1, world);
        return color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}
