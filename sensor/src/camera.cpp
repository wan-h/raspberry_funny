#include "camera.h"
#include <iostream>
#include <thread>
#include <string>
#include <assert.h>

void camRecive(const int param, std::queue<cv::Mat>& Q){
    // 获取视频流
    cv::VideoCapture capture(param);
    assert(capture.isOpened());
    // 启动获取流线程
    // 获取视频写入队列
    std::cout<<"camRecive thread start runnig."<<std::endl;
    cv::Mat frame;
    while(capture.read(frame)){
        // 限制队列长度
        if(Q.size() <= 3){
            Q.push(frame);
        }
    }
}

namespace sensor{
// Camera::Camera(const char* address){
//     std::thread camThread(camRecive, address, std::ref(Q));
//     camThread.detach();
// }
Camera::Camera(const int deviceID){
    std::thread camThread(camRecive, deviceID, std::ref(Q));
    camThread.detach();
}
cv::Mat Camera::get_frame(){
    while(1){
        if(Q.size() > 0){
            cv::Mat frame = Q.front();
            Q.pop();
            return frame;
        }
    }
}
}