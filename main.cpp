#include <iostream>
#include "hazeremovehandler.h"

using namespace std;

int main(int argc, char *argv[])
{
    HazeRemoveHandler* hazeHandler = new HazeRemoveHandler();
    Mat testImg = imread("../test2.jpg");
    hazeHandler->ReadOriImage(testImg);
    hazeHandler->DarkChannelPrior();
    hazeHandler->ShowImage(0);
    hazeHandler->ShowImage(1);
    return 0;
}
