#pragma once

#include "RenderImage.h"
#include <thread>
#include "camera.h"

class RenderThread{

public:

    int id;
    RenderThread(int id, RenderImage* ri, camera* cam, hittable_list* world);
    ~RenderThread();

    void Start();
    bool isRunning();
    void setRow(int row);
	static void sleep(int ms);

    void applyPixels(RenderImage* ri);

    bool canApplyPixels = false;
    bool applyingPixels = false;

private:
    void run();


    std::vector<color> PixelData;
    camera* cam;
    hittable_list* world;
    RenderImage* render;
    int row = 0;
    bool running = false;

};