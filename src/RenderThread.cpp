#include "RenderThread.h"
#include "rtweekend.h"

RenderThread::RenderThread(int id, RenderImage *ri, camera *cam, hittable_list* world)
{
    this->id = id;
    this->cam = cam;
    this->world = world;
    this->render = ri;
}

RenderThread::~RenderThread()
{
}

void RenderThread::Start() {


    std::thread(&RenderThread::run, this).detach(); //detach thread for independent execution. without this, join() function must be called.
}

bool RenderThread::isRunning()
{
    return running;
}

void RenderThread::setRow(int row)
{
    this->row = row;
}

void RenderThread::run() {

    this->running = true;

    for (int i = 0; i < cam->image_width; ++i) {
        color pixel_color(0,0,0);
        for (int sample = 0; sample < cam->samples_per_pixel; ++sample) {
            ray r = cam->get_ray(i, row);
            pixel_color += cam->ray_color(r, cam->max_depth, *world);
        }
        // std::cout << "ID:" << this->id << " placing " << i << " out of " <<  cam->image_width << "\n";
        
        this->PixelData.push_back(pixel_color);
        // render->setPixel(i, row, pixel_color.x(), pixel_color.y(), pixel_color.z(), cam->samples_per_pixel);

    }
    
    // std::cout << "Thread " << id << " is ready to apply\n"; 
    this->canApplyPixels = true;

    this->running = false;
}

void RenderThread::sleep(int ms) {
	std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

void RenderThread::applyPixels(RenderImage *ri)
{

    this->applyingPixels = true;                // * Mark CS as busy

    for(int i = 0; i < PixelData.size(); i++){ // * Loop through all the pixel data

        ri->setPixel(i, row, PixelData[i].x(), PixelData[i].y(),PixelData[i].z(), cam->samples_per_pixel);
    }

    PixelData.clear();                         // * Clear the data

    // std::cout << "Thread " << id << " applied data!\n";

    this->canApplyPixels = false;           // * Set tags to false
    this->applyingPixels = false;
}
