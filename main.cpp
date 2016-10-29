#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>

using namespace cv;
using namespace std;



Mat showHistogram(Mat& img)
{
    int bins = 256;             // number of bins
    int nc = img.channels();    // number of channels

    vector<Mat> hist(nc);       // histogram arrays

    // Initalize histogram arrays
    for (int i = 0; i < hist.size(); i++)
        hist[i] = Mat::zeros(1, bins, CV_32SC1);

    // Calculate the histogram of the image
    for (int i = 0; i < img.rows; i++)
    {
        for (int j = 0; j < img.cols; j++)
        {
            for (int k = 0; k < nc; k++)
            {
                uchar val = nc == 1 ? img.at<uchar>(i,j) : img.at<Vec3b>(i,j)[k];
                hist[k].at<int>(val) += 1;
            }
        }
    }

    // For each histogram arrays, obtain the maximum (peak) value
    // Needed to normalize the display later Mat v = canvas[0];
    int hmax[3] = {0,0,0};
    for (int i = 0; i < nc; i++)
    {
        for (int j = 0; j < bins-1; j++)
            hmax[i] = hist[i].at<int>(j) > hmax[i] ? hist[i].at<int>(j) : hmax[i];
    }

    const char* wname[3] = { "blue", "green", "red" };
    Scalar colors[3] = { Scalar(255,0,0), Scalar(0,255,0), Scalar(0,0,255) };

    vector<Mat> canvas(nc);

    // Display each histogram in a canvas
    for (int i = 0; i < nc; i++)
    {
        canvas[i] = Mat::ones(125, bins, CV_8UC3);

        for (int j = 0, rows = canvas[i].rows; j < bins-1; j++)
        {
            line(
                    canvas[i],
                    Point(j, rows),
                    Point(j, rows - (hist[i].at<int>(j) * rows/hmax[i])),
                    nc == 1 ? Scalar(200,200,200) : colors[i],
                    1, 8, 0
            );
        }


        //imshow(nc == 1 ? "value" : wname[i], canvas[i]);
    }

    Mat v = canvas[0]; //Добавление 1

    return v; //Доб 2



}


int main( int, char** argv )
{
    Mat src, dst;

    const char* source_window = "Source image";
    const char* equalized_window = "Equalized Image";
    const char* sourdce_histogramm = "Source Image Histogramm";
    const char* equalized_image_histogramm = "Equalized Image Histogramm";

    /// Load image
   // src = imread( argv[1], IMREAD_COLOR );
     src = imread( "/home/kama/lenna.jpg", IMREAD_COLOR );

    if( src.empty() )
    { cout<<"Нет такого изображения по данному пути, путь:"<<argv[1]<<endl;
        return -1;
    }

    /// Convert to grayscale
    cvtColor( src, src, COLOR_BGR2GRAY );

    /// Apply Histogram Equalization
    equalizeHist( src, dst );

    /// Display results
    namedWindow( source_window, WINDOW_AUTOSIZE );
    namedWindow( equalized_window, WINDOW_AUTOSIZE );
    namedWindow( sourdce_histogramm, WINDOW_AUTOSIZE );
    namedWindow( equalized_image_histogramm, WINDOW_AUTOSIZE );

    imshow( source_window, src );

    imshow( sourdce_histogramm, showHistogram(src) );



    imshow( equalized_window, dst );


    imshow( equalized_image_histogramm,showHistogram(dst)  );
    /// Wait until user exits the program
    waitKey(0);

    return 0;

}


