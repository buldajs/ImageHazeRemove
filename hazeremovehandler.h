#ifndef HAZEREMOVEHANDLER_H
#define HAZEREMOVEHANDLER_H

#include <iostream>
#include <opencv.hpp>
#include <opencv2/opencv.hpp>
#include "guidedfilter.h"

using namespace cv;
using namespace std;

class HazeRemoveHandler
{
public:
    HazeRemoveHandler();
    void ReadOriImage(Mat img);
    int DarkChannelPrior();
    int EstimateAtmoLight();
    int EstimateTransmission();
    int SmoothTransmissionMap();
    int HazeFree();
    void ShowImage(int flag);
private:
    int kernel_size_;
    double bright_percent_;
    double w_;
    float A_[3];
    float A_average;
    float t0_;
    int r_;
    double eps_;
    Mat ori_image_;
    Mat dark_channel_image_;
    Mat transmission_image_;
    Mat transmission_smoothed_image_;
    Mat haze_free_image_;

};

#endif // HAZEREMOVEHANDLER_H
