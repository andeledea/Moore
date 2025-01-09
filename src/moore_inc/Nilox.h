#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/objdetect/aruco_detector.hpp>
#include <iostream>
#include <vector>
#include "pos.h"

class Nilox
{
public:
    /**
     * Initializes the Nilox camera
     * Tries to open file camera_params.yml
     */
    void init();

    /**
     * Using a chessBoard allows the calibration of the
     * camera and stores the resuts in the camera_params.yml file
     */
    void calibrate();

    /**
     * Shows the camera feed indefinetly
     */
    void render();

    /**
     * Shows the undistorted camera feed and the
     * original one
     */
    void renderUndistorted();

    /**
     * Detects all the markers present in the nilox field of view relative to
     * the center of the FOV itself
     * 
     * @param markerPos, the vector that will contain the position of the found markers
     * @param ids, the vector that will contain the ids of the found markers
     * 
     * @return float, the scaling factor calculated with the markerSize
     */
    float detect(std::vector<cv::Point2f> &markerPos, std::vector<int> &ids, bool estimatePose);

    /**
     * Given two IDs (one defined as origin) calculates the distance and the 
     * displacement vector
     * 
     * @param id, the id to be compared with the origin
     * @param originId, default=0, the id of the origin marker
     */
    cv::Point2f getDisplacementFromOriginOfID(int id, int origId=0);
    ~Nilox();

private:
    cv::VideoCapture inputVideo;
    cv::aruco::ArucoDetector detector;

    cv::Mat cameraMatrix, distCoeffs;

    float correctionValue = 1.5f;
};

void saveCameraParameters(const std::string &filename, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs);
void loadCameraParameters(const std::string& filename, cv::Mat& cameraMatrix, cv::Mat& distCoeffs);
