#pragma once

#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

//For Test Purpose , it extract only the "red color" ,it may detect orange also :) (no escape from that) 
// return: a binary image where the extracted color represent '1' and other are 0
void ExtractColoredObjects(Mat & _src,Mat & _outputMat)
{
	//Opencv see BGR code via Scalar Function 
	//TO Change it to another color , use the numbers range from http://www.workwithcolor.com/red-color-hue-range-01.htm
	//But remember i use the HSV plane instead of BGR so you need to convert HSV to BGR.
	//HSV plane is more robust than RGB.
	//for more about HSV:  https://goo.gl/LCFZyF
	//the code is a snippest from stackoverflow after many tries of me that was work sometimes and sometimes not. :D
	//http://stackoverflow.com/questions/32522989/opencv-better-detection-of-red-color
	Mat hsv_src;
    cvtColor(_src, hsv_src, COLOR_BGR2HSV);

    Mat mask1, mask2;
    inRange(hsv_src, Scalar(0, 70, 50), Scalar(10, 255, 255), mask1);
    inRange(hsv_src, Scalar(170, 70, 50), Scalar(180, 255, 255), mask2);

    _outputMat = mask1 | mask2;

}

// _src : binary image
//_originalImage : original colored image
// return the _originalImage(colored) + rectangle bounary around the non zero pixels
// return the avg x,y of this rectangle
void drawBoundaryAroundObjects(Mat & _src,Mat & _originalImage,Mat & _outputMat,Point & _avgPos)
{
	//This is just a cheap way and not efficient for multiple objects
    cv::Size size = _src.size();

	// The bounding rectangle is defined by its top-left (TL) and bottom-right (BR)
    // coordinates.
    cv::Point tl( size.width, size.height );
    cv::Point br(          0,           0 );
    bool hasPoints = false;

    for ( int y = 0; y < size.height; ++y )
    {
        for ( int x = 0; x < size.width; ++x )
        {
            if ( _src.at<unsigned char>( y, x ) > 0 )
            {
                hasPoints = true;

                // Update the top-left corner.
                if ( x < tl.x ) tl.x = x;
                if ( y < tl.y ) tl.y = y;

                // Update the bottom-right corner.
                if ( x > br.x ) br.x = x;
                if ( y > br.y ) br.y = y;
            }
        }
    }

    _outputMat = _originalImage;
    // If the image contains any non-zero pixels, then draw green rect box with thickness of 5
    if ( hasPoints )
    {
        cv::rectangle( _outputMat, tl, br, Scalar( 0, 255, 0), 5 );
        _avgPos.x = ( tl.x + br.x ) / 2;
        _avgPos.y = ( tl.y + br.y) / 2;
    }
    else
    	putText(_outputMat,"No Red Color ?",Point(0,60),2,2,Scalar(0,0,255));

}


