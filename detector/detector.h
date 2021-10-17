#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <string>
#include "net.h"

struct Object
{
    cv::Rect_<float> rect;
    int label;
    float prob;
};

namespace det {
    class Detector
    {
    private:
        ncnn::Net detector;
    public:
        // Detector();s
        // ~Detector();
        Detector(const char* param_file, const char* bin_file);
        int detect(const cv::Mat& bgr, std::vector<Object>& objects);
        cv::Mat draw(const cv::Mat& bgr, const std::vector<Object>& objects);
    };
}

#endif