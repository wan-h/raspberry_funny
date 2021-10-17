#ifndef CAMERA_H
#define CAMERA_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <vector>
#include <string>
#include <queue>

namespace sensor {
class Camera
{
private:
    std::queue<cv::Mat> Q;
public:
    Camera(const int deviceID);
    Camera(const char* address);
    cv::Mat get_frame();
};
}

#endif