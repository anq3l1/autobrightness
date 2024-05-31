#include <iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include <string>
#include <thread>
#include <chrono>

using namespace cv;

void set_brightness(int level)
{
    std::string brightnessCommand = "(Get-WmiObject -Namespace root/WMI -Class WmiMonitorBrightnessMethods).WmiSetBrightness(1, " + std::to_string(level) + ")";
    std::string command = "powershell.exe -command \"" + brightnessCommand + "\"";
    system(command.c_str());
}

void get_light_room(Mat img, std::vector<int> thresholds = { 30, 60, 120, 180 })
{
    setlocale(LC_ALL, "ru");

    Mat gray;
    cvtColor(img, gray, COLOR_BGR2GRAY);
    double mean_brightness = mean(gray)[0];

    if (mean_brightness < thresholds[0])
    {
        std::cout << "Очень темно" << std::endl;
        set_brightness(30);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    else if (mean_brightness < thresholds[1])
    {
        std::cout << "Темно" << std::endl;
        set_brightness(50);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    else if (mean_brightness < thresholds[2])
    {
        std::cout << "Ярко" << std::endl;
        set_brightness(70);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    else if (mean_brightness < thresholds[3])
    {
        std::cout << "Очень ярко" << std::endl;
        set_brightness(100);
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}

int main()
{
    setlocale(LC_ALL, "ru");

    VideoCapture capture(0);
    if (!capture.isOpened())
    {
        std::cerr << "Ошибка открытия камеры" << std::endl;
        return -1;
    }

    Mat img;
    while (true) {
        capture >> img;
        if (img.empty()) {
            std::cerr << "Ошибка захвата кадра" << std::endl;
            break;
        }

        flip(img, img, 1);

        get_light_room(img);

        //cv::imshow("From camera", frame);

        int k = cv::waitKey(30) & 0xFF;
        if (k == 27)
        {
            set_brightness(60);
            std::cout << "Яркость поставленна на 60%" << std::endl;
            break;
        }
    }

    return 0;
}
