#ifndef DETECTOR_H
#define DETECTOR_H

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <stdio.h>
#include <vector>
#include <string>

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
        const char* paramFile;
        const char* binFile;
    public:
        // Detector();
        // ~Detector();
        Detector(const char* param_file, const char* bin_file);
        int detect(const cv::Mat& bgr, std::vector<Object>& objects);
        cv::Mat draw(const cv::Mat& bgr, const std::vector<Object>& objects);
    };
}

#endif