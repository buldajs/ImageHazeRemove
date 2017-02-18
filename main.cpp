#include <iostream>
#include "hazeremovehandler.h"

using namespace std;

int main(int argc, char *argv[])
{
    HazeRemoveHandler* hazeHandler = new HazeRemoveHandler();
    Mat testImg = imread("../demo5.jpg");
    hazeHandler->ReadOriImage(testImg);
    hazeHandler->DarkChannelPrior();
    hazeHandler->EstimateAtmoLight();
    hazeHandler->EstimateTransmission();
    hazeHandler->SmoothTransmissionMap();
    hazeHandler->HazeFree();
    hazeHandler->ShowImage(0);
//    hazeHandler->ShowImage(1);
//    hazeHandler->ShowImage(2);
//    hazeHandler->ShowImage(3);
    hazeHandler->ShowImage(4);
    cvWaitKey(0);
    return 0;
}
