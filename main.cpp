#include "detector.h"

int main(int argc, char* argv[]){
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [imagepath]\n", argv[0]);
        return -1;
    }

    const char* imagepath = argv[1];

    cv::Mat m = cv::imread(imagepath, 1);
    if (m.empty())
    {
        fprintf(stderr, "cv::imread %s failed\n", imagepath);
        return -1;
    }

    det::Detector detector = det::Detector(
        "/home/wan/workspace/code/raspberry_funny/detector/mobilenetv3_ssdlite_voc.param", 
        "/home/wan/workspace/code/raspberry_funny/detector/mobilenetv3_ssdlite_voc.bin"
        );

    std::vector<Object> objects;
    detector.detect(m, objects);
    detector.draw(m, objects);

    return 0;
}