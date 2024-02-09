#pragma once

#include <opencv2/core.hpp>


class RenderImage{

public:
    RenderImage(const int imageWidth, const int imageHeight);
    ~RenderImage();

    void setPixel(int x, int y, float r, float g, float b, int samples_per_pixel);

    void saveImage(cv::String &fileName) const;

private:

    std::unique_ptr<cv::Mat> pixels = NULL;

    int imageWidth;
    int imageHeight;

};