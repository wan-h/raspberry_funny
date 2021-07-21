#include "detector.h"
#include "net.h"

template<class T>
const T& clamp(const T& v, const T& lo, const T& hi)
{
    assert(!(hi < lo));
    return v < lo ? lo : hi < v ? hi : v;
}

namespace det{
Detector::Detector(const char* param_file, const char* bin_file){
    paramFile = param_file;
    binFile = bin_file;
}
int Detector::detect(const cv::Mat& bgr, std::vector<Object>& objects){
    ncnn::Net net;

#if NCNN_VULKAN
    net.opt.use_vulkan_compute = true;
#endif // NCNN_VULKAN

    // converted ncnn model from https://github.com/ujsyehao/mobilenetv3-ssd
    net.load_param(paramFile);
    net.load_model(binFile);

    const int target_size = 300;

    int img_w = bgr.cols;
    int img_h = bgr.rows;

    ncnn::Mat in = ncnn::Mat::from_pixels_resize(bgr.data, ncnn::Mat::PIXEL_BGR2RGB, bgr.cols, bgr.rows, target_size, target_size);

    const float mean_vals[3] = {123.675f, 116.28f, 103.53f};
    const float norm_vals[3] = {1.0f, 1.0f, 1.0f};
    in.substract_mean_normalize(mean_vals, norm_vals);

    ncnn::Extractor ex = net.create_extractor();

    ex.input("input", in);

    ncnn::Mat out;
    ex.extract("detection_out", out);

    //     printf("%d %d %d\n", out.w, out.h, out.c);
    objects.clear();
    for (int i = 0; i < out.h; i++)
    {
        const float* values = out.row(i);

        Object object;
        object.label = values[0];
        object.prob = values[1];

        // filter out cross-boundary
        float x1 = clamp(values[2] * target_size, 0.f, float(target_size - 1)) / target_size * img_w;
        float y1 = clamp(values[3] * target_size, 0.f, float(target_size - 1)) / target_size * img_h;
        float x2 = clamp(values[4] * target_size, 0.f, float(target_size - 1)) / target_size * img_w;
        float y2 = clamp(values[5] * target_size, 0.f, float(target_size - 1)) / target_size * img_h;

        object.rect.x = x1;
        object.rect.y = y1;
        object.rect.width = x2 - x1;
        object.rect.height = y2 - y1;

        objects.push_back(object);
    }

    return 0;
}

cv::Mat Detector::draw(const cv::Mat& bgr, const std::vector<Object>& objects)
{
    static const char* class_names[] = {"background",
                                        "aeroplane", "bicycle", "bird", "boat",
                                        "bottle", "bus", "car", "cat", "chair",
                                        "cow", "diningtable", "dog", "horse",
                                        "motorbike", "person", "pottedplant",
                                        "sheep", "sofa", "train", "tvmonitor"
                                       };

    cv::Mat image = bgr.clone();

    for (size_t i = 0; i < objects.size(); i++)
    {
        if (objects[i].prob > 0.6)
        {
            const Object& obj = objects[i];

            fprintf(stderr, "%d = %.5f at %.2f %.2f %.2f x %.2f\n", obj.label, obj.prob,
                    obj.rect.x, obj.rect.y, obj.rect.width, obj.rect.height);

            cv::rectangle(image, obj.rect, cv::Scalar(255, 0, 0));

            char text[256];
            sprintf(text, "%s %.1f%%", class_names[obj.label], obj.prob * 100);

            int baseLine = 0;
            cv::Size label_size = cv::getTextSize(text, cv::FONT_HERSHEY_SIMPLEX, 0.5, 1, &baseLine);

            int x = obj.rect.x;
            int y = obj.rect.y - label_size.height - baseLine;
            if (y < 0)
                y = 0;
            if (x + label_size.width > image.cols)
                x = image.cols - label_size.width;

            cv::rectangle(image, cv::Rect(cv::Point(x, y), cv::Size(label_size.width, label_size.height + baseLine)),
                          cv::Scalar(255, 255, 255), -1);

            cv::putText(image, text, cv::Point(x, y + label_size.height),
                        cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 0, 0));
        }
    }

    cv::imshow("image", image);
    cv::waitKey(0);
    return image;
}
}