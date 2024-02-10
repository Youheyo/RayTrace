#include "RenderImage.h"
#include "interval.h"
#include <iostream>
#include <opencv2/imgcodecs.hpp>


RenderImage::RenderImage(const int imageWidth, const int imageHeight) {
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;

    this->pixels = std::make_unique<cv::Mat>(cv::Mat::zeros(this->imageHeight, this->imageWidth, CV_8UC3));
}

RenderImage::~RenderImage() {
}

void RenderImage::setPixel(int x, int y, float r, float g, float b, int samples_per_pixel) {
    // Gamma Correction
    float scale = 1.0f / samples_per_pixel;
    r = sqrt(scale * r);
    g = sqrt(scale * g);
    b = sqrt(scale * b);

    interval intensity(0.0f, 0.999f);

    int rInt = static_cast<uchar>(256 * intensity.clamp(r));
    int gInt = static_cast<uchar>(256 * intensity.clamp(g));
    int bInt = static_cast<uchar>(256 * intensity.clamp(b));

    cv::Mat imgChannels[3];
    cv::split(*this->pixels, imgChannels);
    
    // * OpenCV has BGR instead of RGB
    imgChannels[0].at<uchar>(this->imageHeight - 1 - y, x ) = bInt;
    imgChannels[1].at<uchar>(this->imageHeight - 1 - y, x ) = gInt;
    imgChannels[2].at<uchar>(this->imageHeight - 1 - y, x ) = rInt;


    cv::merge(imgChannels, 3, *this->pixels);
    
}

void RenderImage::saveImage(cv::String &fileName) const {

    cv::flip(*this->pixels, *this->pixels, 0);

    cv::imwrite(fileName + ".png", *this->pixels);

    std::cout << fileName << " has been generated!\n";
}