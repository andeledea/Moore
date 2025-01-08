#include <exception>
#include <algorithm>
#include "Nilox.h"

void Nilox::init()
{
    // Open the default camera (usually the first camera)
    inputVideo.open(0);

    // Check if the camera opened successfully
    if (!inputVideo.isOpened())
    {
        throw std::runtime_error("Could not open nilox camera!");
    }

    cv::aruco::DetectorParameters detectorParams;
    detectorParams.cornerRefinementMethod = cv::aruco::CORNER_REFINE_CONTOUR;
    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

    detector.setDetectorParameters(detectorParams);
    detector.setDictionary(dictionary);
}

int Nilox::detect(std::vector<pos> &markerPos, std::vector<int> &ids, bool centered)
{
    double totalTime = 0;
    int totalIterations = 0;
    bool showRejected = false;
    int waitTime = 10;
    float markerLength = 0.081f; // 8,1 cm

    while (inputVideo.grab())
    {
        cv::Mat image, imageCopy;
        inputVideo.retrieve(image);

        double tick = (double)cv::getTickCount();

        std::vector<std::vector<cv::Point2f>> corners, rejected;

        // detect markers and estimate pose
        detector.detectMarkers(image, corners, ids, rejected);

        image.copyTo(imageCopy);
        if (!ids.empty())
        {
            cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);
        }

        if (showRejected && !rejected.empty())
            cv::aruco::drawDetectedMarkers(imageCopy, rejected, cv::noArray(), cv::Scalar(100, 0, 255));

        // cv::Point2f centerSize(1, 1);
        // for (auto corner : corners)
        // {
        //     cv::Point2f center = (corner[0] + corner[1] + corner[2] + corner[3]) / 4;
        //     cv::rectangle(imageCopy, center - centerSize, center + centerSize, cv::Scalar(255, 0, 0));
        // }
        cv::imshow("Press ESC to confirm", imageCopy);
        char key = (char)cv::waitKey(waitTime);
        if (key == 27)
        {
            cv::Point2f center;
            for (auto corner : corners)
            {
                center = (corner[0] + corner[1] + corner[2] + corner[3]) / 4 / correctionValue;
                if (centered) center -= cv::Point2f(image.size()) / 2;
                markerPos.emplace_back((pos){
                    .x = center.x,
                    .y = center.y,
                    .z = 0});

                float d = cv::norm(corner[0] - corner[1]);
            }
            return markerPos.size();
        }
    }
    return 0;
}

pos Nilox::getDisplacementOfIDzeroInFOV()
{
    std::vector<pos> markerPos;
    std::vector<int> ids;
    this->detect(markerPos, ids, true);

    bool found = false;
    pos retVal = (pos){0, 0, 0};

    for (int i = 0; i < ids.size(); i++)
    {
        if (ids[i] == 0)
        {
            if (found == false) {
                retVal = markerPos[i];
            }
            else std::cout << "Multiple 0 Ids" << std::endl;
        }
    }
    if(!found) std::cout << "Could not find ID 0 in FOV" << std::endl;

    return retVal;
}
