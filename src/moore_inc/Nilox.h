#pragma once
#include <opencv2\opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <iostream>
#include <vector>
#include "pos.h"

class Nilox
{
public:
    void init();

    /**
     * Detects all the markers present in the nilox field of view
     * @param markerPos, the vector that will contain the position of the found markers
     * @param ids, the vector that will contain the ids of the found markers
     * 
     * @return int, the number of markers detected
     */
    int detect(std::vector<pos> &markerPos, std::vector<int> &ids, bool centered=false);

    pos getDisplacementOfIDzeroInFOV();

private:
    cv::VideoCapture inputVideo;
    cv::aruco::ArucoDetector detector;

    float correctionValue = 1.5f;
};