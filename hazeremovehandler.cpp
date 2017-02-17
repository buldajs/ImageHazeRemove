#include "hazeremovehandler.h"

HazeRemoveHandler::HazeRemoveHandler()
{
    kernel_size_ = 15;
    bright_percent_ = 0.1;
    A_[0] = A_[1] = A_[2] = 0;
    w_ = 0.95;
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
        cvWaitKey(0);
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
                        min(ori_image_.at<Vec3f>(i,j)[0],ori_image_.at<Vec3f>(i,j)[1]),
                        min(ori_image_.at<Vec3f>(i,j)[0],ori_image_.at<Vec3f>(i,j)[2])
                        );
        }

    // Minfilter implemented by erode function
    erode(dark_tmp,dark_channel_image_,Mat::ones(kernel_size_,kernel_size_,CV_32FC1));

    return 0;

}
// This Method is to estimate the transmission map of the original image
// t(x) = 1- min( min( I^c(y)/A^c ) )
// Output :
//          -1: means it has not load original image or estimate the Atmospheric Light
//              , should do that before.
//          0: means estimate the transmission map succeed.

int HazeRemoveHandler::EstimateTransmission()
{
    float A_average = (A_[0]+A_[1]+A_[2])/3;
    if( (ori_image_.cols==0) || (ori_image_.rows==0) || (A_average ==0))
    {
        return -1;
    }
    Mat dark_tmp = Mat::zeros(ori_image_.rows,ori_image_.cols,CV_32FC1);

    int rows = dark_tmp.rows;
    int cols = dark_tmp.cols;

    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
        {
            dark_tmp.at<float>(i,j) = min(
                        min(ori_image_.at<Vec3f>(i,j)[0]/A_average,ori_image_.at<Vec3f>(i,j)[1]/A_average),
                        min(ori_image_.at<Vec3f>(i,j)[0]/A_average,ori_image_.at<Vec3f>(i,j)[2]/A_average)
                        );
        }
    Mat dark_hazeimage;

    // Minfilter implemented by erode function
    erode(dark_tmp,dark_hazeimage,Mat::ones(kernel_size_,kernel_size_,CV_32FC1));

    transmission_image_ = Mat(ori_image_.rows,ori_image_.cols,CV_32FC1);
    for(int i = 0; i < rows; i++)
        for(int j = 0; j < cols; j++)
        {
            transmission_image_.at<float>(i,j) = 1-w_*dark_hazeimage.at<float>(i,j);
        }
    return 0;
}

// This Method is to estimating the Atmospheric Light of the scene
// Output :
//          -1: means it has not compute dark channel image, should do DarkChannelPrior before.
//          0: means computing estimate succeed.

int HazeRemoveHandler::EstimateAtmoLight()
{
    if((dark_channel_image_.cols ==0 )||(dark_channel_image_.rows==0))
    {
        return -1;
    }

    int bripix_num;
    bripix_num = (int)dark_channel_image_.cols*dark_channel_image_.rows*bright_percent_/100;

    // Change rows*cols data -> 1*(rows*cols) data
    Mat dark_reshape = dark_channel_image_.reshape(0,1).clone();
    Mat sorted;
    sortIdx(dark_reshape,sorted,CV_SORT_EVERY_ROW+CV_SORT_DESCENDING);
    vector<int> bripixel_idx;
    for(int i = 0 ; i < bripix_num; i++)
    {
        int pixel_rows;
        int pixel_cols;
        if(sorted.at<int>(0,i)%dark_channel_image_.cols==0)
        {
            pixel_rows = (sorted.at<int>(0,i)+1)/dark_channel_image_.cols-1;
            pixel_cols = (sorted.at<int>(0,i)+1)%dark_channel_image_.cols+dark_channel_image_.cols;
        }
        else
        {
            pixel_rows = (sorted.at<int>(0,i)+1)/dark_channel_image_.cols;
            pixel_cols = (sorted.at<int>(0,i)+1)%dark_channel_image_.cols-1;
        }
        A_[0] += ori_image_.at<Vec3f>(pixel_rows,pixel_cols)[0];
        A_[1] += ori_image_.at<Vec3f>(pixel_rows,pixel_cols)[1];
        A_[2] += ori_image_.at<Vec3f>(pixel_rows,pixel_cols)[2];
    }
    A_[0] /= bripix_num;
    A_[1] /= bripix_num;
    A_[2] /= bripix_num;
    return 0;
}
