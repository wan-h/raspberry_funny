#include "detector.h"
#include "camera.h"
#include <iostream>
#include <ctime>


int main(int argc, char* argv[]){
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s [rtsp address]\n", argv[0]);
        return -1;
    }

    const char* address = argv[1];
    std::cout << "RTSP address: " << address << std::endl;
    cam::Camera camera = cam::Camera(address);
    det::Detector detector = det::Detector(
        "/home/wan/workspace/code/raspberry_funny/detector/mobilenetv3_ssdlite_voc.param", 
        "/home/wan/workspace/code/raspberry_funny/detector/mobilenetv3_ssdlite_voc.bin"
        );

    clock_t start,end;
    while(1){
        cv::Mat frame = camera.get();
        std::cout << "Get a frame" << std::endl;
        std::vector<Object> objects;
        start=clock();
        detector.detect(frame, objects);
        end=clock();
        std::cout<<"Total time:"<<double(end-start)/CLOCKS_PER_SEC * 1000<<"ms"<<std::endl;
        detector.draw(frame, objects);
    }
    return 0;
}