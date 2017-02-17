#ifndef HAZEREMOVEHANDLER_H
#define HAZEREMOVEHANDLER_H

#include <iostream>
#include <opencv.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class HazeRemoveHandler
{
public:
    HazeRemoveHandler();
    void ReadOriImage(Mat img);
    int DarkChannelPrior();
    void ShowImage(int flag);
private:
    int kernel_size_;
    Mat ori_image_;
    Mat dark_channel_image_;

};

#endif // HAZEREMOVEHANDLER_H
