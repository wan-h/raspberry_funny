#include "camera.h"
#include <iostream>
#include <thread>
#include <string>

void camRecive(const char* address, std::queue<cv::Mat>& Q){
    // 获取视频流
    cv::VideoCapture capture(address);
    if(!capture.isOpened())
    {
        std::cout<<"video not open."<<std::endl;
    }
    // 启动获取流线程
    // 获取视频写入队列
    std::cout<<"camRecive thread start runnig."<<std::endl;
    cv::Mat frame;
    while(capture.read(frame)){
        // 限制队列长度
        if(Q.size() < 10){
            Q.push(frame);
        }
    }
}

namespace cam{
Camera::Camera(const char* address){
    std::thread camThread(camRecive, address, std::ref(Q));
    camThread.detach();
}
cv::Mat Camera::get(){
    while(1){
        if(Q.size() > 0){
            cv::Mat frame = Q.front();
            Q.pop();
            return frame;
        }
    }
}

}