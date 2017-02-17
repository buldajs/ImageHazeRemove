#include "hazeremovehandler.h"

HazeRemoveHandler::HazeRemoveHandler()
{
    kernel_size_ = 15;
}


// This Method is to read original image and convert it to float data type and been normilized to 0~1.
// Input : input Mat type image.
void HazeRemoveHandler::ReadOriImage(Mat img)
{
    ori_image_ = Mat(img.rows,img.cols,CV_32FC3);
    img.convertTo(ori_image_,CV_32FC3);

    // Normilize to 0~1
    ori_image_ = ori_image_/255;
}

void HazeRemoveHandler::ShowImage(int type)
{
    switch (type) {
    case 0:
        //
        // imshow function notice
        // for different data types
        // 1. 8-bit unsigned char : show directly
        // 2. 16-bit unsigned or 32-bit integer : pixel value/256, [0,255*256] -> [0,255]
        // 3. 32-bit floating-point : pixel value*255, [0,1]->[0,255]
        //
        imshow("Original Image",ori_image_);
        break;
    case 1:
        imshow("Darkchannel Image",dark_channel_image_);
        break;
    default:
        break;
    }
}

// This Method is to get the dark channel of the original image
// //J^{dark}(x)=min( min( J^c(y) ) )
// Output :
//          -1: means it has not load original image, should do ReadOriImage before.
//          0: means computing dark channel succeed.
int HazeRemoveHandler::DarkChannelPrior()
{
    if( (ori_image_.cols==0) || (ori_image_.rows==0))
    {
        return -1;
    }

    // Compute min value for every pixel during different three channels(R,G,B)
    Mat dark_tmp = Mat::zeros(ori_image_.rows,ori_image_.cols,CV_32FC1);

    int rows = dark_tmp.rows;
    int cols = dark_tmp.cols;

    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
        {
            dark_tmp.at<float>(i,j) = min(
                        min(ori_image_.at<Vec<float,3>>(i,j)[0],ori_image_.at<Vec<float,3>>(i,j)[1]),
                        min(ori_image_.at<Vec<float,3>>(i,j)[0],ori_image_.at<Vec<float,3>>(i,j)[2])
                        );
        }

    // Minfilter implemented by erode function
    erode(dark_tmp,dark_channel_image_,Mat::ones(kernel_size_,kernel_size_,CV_32FC1));

    return 0;

}
