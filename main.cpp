#include <iostream>
#include "hazeremovehandler.h"

using namespace std;

int main(int argc, char *argv[])
{
    HazeRemoveHandler* hazeHandler = new HazeRemoveHandler();
    Mat testImg = imread("../test.jpg");
    hazeHandler->ReadOriImage(testImg);
    hazeHandler->DarkChannelPrior();
    hazeHandler->EstimateAtmoLight();
    hazeHandler->EstimateTransmission();
    return 0;
}
