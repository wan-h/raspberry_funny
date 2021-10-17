// #include "UltraFace.h"
// #include <iostream>
// #include <opencv2/opencv.hpp>

// using namespace det;

// int main(int argc, char **argv) {
//     if (argc <= 3) {
//         fprintf(stderr, "Usage: %s <ncnn bin> <ncnn param> [image files...]\n", argv[0]);
//         return 1;
//     }

//     std::string bin_path = argv[1];
//     std::string param_path = argv[2];
//     Detector detector(param_path.data(), bin_path.data()); // config model input

//     for (int i = 3; i < argc; i++) {
//         std::string image_file = argv[i];
//         std::cout << "Processing " << image_file << std::endl;

//         cv::Mat frame = cv::imread(image_file);

//         std::vector<Object> face_info;
//         detector.detect(frame, face_info);

//         for (int i = 0; i < face_info.size(); i++) {
//             auto face = face_info[i];
//             cv::Point pt1(face.x1, face.y1);
//             cv::Point pt2(face.x2, face.y2);
//             cv::rectangle(frame, pt1, pt2, cv::Scalar(0, 255, 0), 2);
//         }

//         cv::imwrite("result.jpg", frame);
//     }
//     return 0;
// }


#include "button.h"
#include "buzzer.h"
#include "camera.h"
#include "motor.h"
#include "pid.h"
#include "config.h"
// #include "detector.h"
#include "UltraFace.h"
#include <iostream>
#include <ctime>
#include <string>
#include <assert.h>

using namespace std;
using namespace det;
using namespace sensor;
using namespace controller;

Object findPerson(cv::Mat frame, Detector& detector){
    float personArea = 0; 
    // Object personObject = {{0, 0, 0, 0}, 0, 0};
    Object personObject = {0, 0, 0, 0, 0, NULL};
    double start=clock();
    vector<Object> objects;
    detector.detect(frame, objects);
    cv::Mat image = detector.draw(frame, objects);
    cv::imwrite("test.jpg", image);
    double end=clock();
    cout<<"Inference time:"<<double(end-start)/CLOCKS_PER_SEC * 1000<<"ms"<<endl;
    // 找到person对象
    // for (size_t i = 0; i < objects.size(); i++){
    //     const Object& object = objects[i];
    //     float x = object.rect.x;
    //     float y = object.rect.y;
    //     float width = object.rect.width;
    //     float height = object.rect.height;
    //     // 寻找最近目标
    //     float area = width * height;
    //     if (area > personArea){
    //         personArea = area;
    //         personObject = object;
    //     }
    // }
    for (size_t i = 0; i < objects.size(); i++){
        const Object& object = objects[i];
        float x1 = object.x1;
        float y1 = object.y1;
        float x2 = object.x2;
        float y2 = object.y2;
        float width = x2 - x1;
        float height = y2 - y1;
        // 寻找最大目标
        float area = width * height;
        if (area > personArea){
            personArea = area;
            personObject = object;
        }
    }
    return personObject;
}


int modeChoose(const Object& object, int widthTarget, int heightTarget){
    float midWidth = (object.x1 + object.x2) / 2;
    float midHeight = (object.y1 + object.y2) / 2;
    // 优先矫正转向过大偏移
    if(abs(midHeight - heightTarget) < 10){
        return 1;
    }
    return 2;
}

int main(int argc, char* argv[]){
    if (argc != 2)
    {   
        cout << "Usage: [config path]" << endl;
        return -1;
    }

    const char* config_path = argv[1];
    cout << "config path: " << config_path << endl;
    INIReader reader(config_path);
    assert(reader.ParseError() >= 0);
    
    // 读取配置文件信息
    string modelBinPath = reader.GetString("model", "bin", "");
    string modelParamPath = reader.GetString("model", "param", "");
    double widthKp = reader.GetReal("pid", "widthKp", 0.0);
    double widthKi = reader.GetReal("pid", "widthKi", 0.0);
    double widthKd = reader.GetReal("pid", "widthKd", 0.0);
    double heightKp = reader.GetReal("pid", "heightKp", 0.0);
    double heightKi = reader.GetReal("pid", "heightKi", 0.0);
    double heightKd = reader.GetReal("pid", "heightKd", 0.0);
    double widthTarget = reader.GetReal("pid", "widthTarget", 0.0);
    double heightTarget = reader.GetReal("pid", "heightTarget", 0.0);
    int width = reader.GetInteger("image", "width", 0);
    int height = reader.GetInteger("image", "height", 0);
    int widthMax_speed = reader.GetInteger("car", "widthMax_speed", 0);
    int heightMax_speed = reader.GetInteger("car", "heightMax_speed", 0);

    // 找到最近目标
    Detector detector(modelParamPath.data(), modelBinPath.data());
    Camera camera(0);
    // C++ 编译器把不带参数的构造函数优先认为是一个函数声明
    Buzzer buzzer{};
    PID pid_width(widthKp, widthKi, widthKd, widthTarget);
    PID pid_height(heightKp, heightKi, heightKd, heightTarget);
    Motor motor{};

    while(1){
        double start=clock();
        cv::Mat frame = camera.get_frame();
        cv::Mat resize_frame;
        cv::resize(frame, resize_frame, cv::Size(width, height));
        Object personObject = findPerson(resize_frame, detector);
        // 若找到目标
        double out = 0;
        // if(personObject.prob != 0){
        int mode = modeChoose(personObject, widthTarget, heightTarget);
        int max_speed = 0;
        if(personObject.score != 0){
            buzzer.doubleClock();
            // pid计算电机输出
            // out = pid.cal(personObject.rect.x + personObject.rect.width / 2);
            if(mode == 1){
                out = pid_width.cal((personObject.x1 + personObject.x2) / 2);
                max_speed = widthMax_speed;
            } else {
                out = pid_height.cal((personObject.y1 + personObject.y2) / 2);
                max_speed = heightMax_speed;
            }
        }
        int speed = int(out);
        // 最大速度限制
        if(speed > max_speed) {
            speed = max_speed;
        }
        if(speed < -max_speed) {
            speed = -max_speed;
        }

        if(mode == 1){
            motor.go(-speed, speed);
        } else {
            motor.go(-speed, -speed);
        }

        if(personObject.score != 0){
            cout << "x1: " << personObject.x1 << ";y1: " << personObject.y1 << ";x2: " << personObject.x2 << ";y2: " << personObject.y2 << endl;
            cout << "Speed: " << speed << endl;
        }
        double end=clock();
        cout<<"Total time:"<<double(end-start)/CLOCKS_PER_SEC * 1000<<"ms"<<endl;
    }

    return 0;
}