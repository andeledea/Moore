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
        throw std::runtime_error("[ERROR] Could not open nilox camera!");
    }

    try
    {
        loadCameraParameters("camera_params.yml", this->cameraMatrix, this->distCoeffs);
        std::cout << "[INFO] Nilox camera parameters set" << std::endl;
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
        std::cout << "[ACTION] Nilox camera parameters calibration started" << std::endl;
        this->calibrate();
    }

    cv::aruco::DetectorParameters detectorParams;
    detectorParams.cornerRefinementMethod = cv::aruco::CORNER_REFINE_CONTOUR;
    cv::aruco::Dictionary dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);

    detector.setDetectorParameters(detectorParams);
    detector.setDictionary(dictionary);
}

void Nilox::calibrate()
{
    // Define the dimensions of the chessboard
    cv::Size boardSize(9, 6); // Number of inner corners per chessboard row and column
    float squareSize = 25.0f; // Size of a square in your defined unit (e.g., meters)

    // Prepare object points (3D points in real world space)
    std::vector<cv::Point3f> objectPoints;
    for (int i = 0; i < boardSize.height; i++)
    {
        for (int j = 0; j < boardSize.width; j++)
        {
            objectPoints.emplace_back(j * squareSize, i * squareSize, 0);
        }
    }

    std::vector<std::vector<cv::Point3f>> objectPointsList;
    std::vector<std::vector<cv::Point2f>> imagePoints;

    if (!inputVideo.isOpened())
    {
        std::cerr << "[ERROR] Could not open the camera." << std::endl;
        return;
    }

    cv::Mat frame, gray;
    char key = 0;

    std::cout << "Press 'a' to capture an image for calibration." << std::endl;
    std::cout << "Press 's' to start calibration and save parameters." << std::endl;

    while (key != 'q')
    { // Press 'q' to quit
        inputVideo >> frame;
        if (frame.empty())
        {
            std::cerr << "[ERROR] Could not grab a frame." << std::endl;
            break;
        }

        key = cv::waitKey(30); // Wait for a key press

        cvtColor(frame, gray, cv::COLOR_BGR2GRAY);
        std::vector<cv::Point2f> corners;

        // Find the chessboard corners
        bool found = findChessboardCorners(gray, boardSize, corners);
        if (found)
        {
            // Refine corner locations
            cornerSubPix(gray, corners, cv::Size(11, 11), cv::Size(-1, -1),
                         cv::TermCriteria(cv::TermCriteria::EPS + cv::TermCriteria::COUNT, 30, 0.1));

            // Draw and display the corners
            drawChessboardCorners(frame, boardSize, cv::Mat(corners), found);
            imshow("Camera Feed", frame);

            // Store the object points and image points
        }
        else
        {
            imshow("Camera Feed", frame);
            std::cout << "[WARN] Chessboard corners not found. \r";
        }

        if (key == 'a')
        { // If a key is pressed
            objectPointsList.push_back(objectPoints);
            imagePoints.push_back(corners);
            std::cout << "[INFO] Captured image for calibration." << std::endl;
        }

        if (key == 's')
        { // Start calibration when 's' is pressed
            if (objectPointsList.size() < 1)
            {
                std::cout << "[WARN] Not enough images for calibration." << std::endl;
                continue;
            }
            std::cout << "[INFO] Images for calibration: " << objectPointsList.size() << std::endl;

            // Calibrate the camera
            std::vector<cv::Mat> rvecs, tvecs;
            calibrateCamera(objectPointsList, imagePoints, gray.size(), cameraMatrix, distCoeffs, rvecs, tvecs);

            // Save the camera parameters
            saveCameraParameters("camera_params.yml", cameraMatrix, distCoeffs);
            std::cout << "[INFO] Camera parameters saved to camera_params.yml" << std::endl;

            // Clear the lists for the next calibration session
            objectPointsList.clear();
            imagePoints.clear();
            break;
        }
    }

    cv::destroyAllWindows();
}

void Nilox::render()
{
    std::cout << "[INFO] Starting Nilox render" << std::endl;

    cv::Mat frame;
    char key = 0;
    while (key != 'q')
    { // Press 'q' to quit
        inputVideo >> frame;
        if (frame.empty())
        {
            std::cerr << "[ERROR] Could not grab a frame." << std::endl;
            break;
        }

        key = cv::waitKey(30); // Wait for a key press
        imshow("Nilox render", frame);
    }

    cv::destroyAllWindows();
}

void Nilox::renderUndistorted()
{
    std::cout << "[INFO] Starting Nilox render and undistort" << std::endl;

    cv::Mat frame, undistort_frame;
    char key = 0;
    while (key != 'q')
    { // Press 'q' to quit
        inputVideo >> frame;
        if (frame.empty())
        {
            std::cerr << "[ERROR] Could not grab a frame." << std::endl;
            break;
        }

        key = cv::waitKey(30); // Wait for a key press

        // Correct fisheye distortion
        undistort(frame, undistort_frame, cameraMatrix, distCoeffs);

        // Show the original and corrected frames
        imshow("Original Camera Feed", frame);
        imshow("Corrected Camera Feed", undistort_frame);
    }

    cv::destroyAllWindows();
}

float Nilox::detect(std::vector<cv::Point2f> &markerPos, std::vector<int> &ids, bool estimatePose)
{
    bool showRejected = false;
    int waitTime = 10;
    float markerLength = 52.8f; // mm

    cv::Mat objPoints(4, 1, CV_32FC3);
    objPoints.ptr<cv::Vec3f>(0)[0] = cv::Vec3f(-markerLength / 2.f, markerLength / 2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[1] = cv::Vec3f(markerLength / 2.f, markerLength / 2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[2] = cv::Vec3f(markerLength / 2.f, -markerLength / 2.f, 0);
    objPoints.ptr<cv::Vec3f>(0)[3] = cv::Vec3f(-markerLength / 2.f, -markerLength / 2.f, 0);

    cv::Mat centPoint(1, 1, CV_32FC3);
    centPoint.ptr<cv::Point2f>(0)[0] = cv::Point2f(0, 0);

    while (inputVideo.grab())
    {
        cv::Mat image, undistorted, imageCopy;
        inputVideo.retrieve(image);
        cv::undistort(image, undistorted, this->cameraMatrix, this->distCoeffs);

        double tick = (double)cv::getTickCount();

        std::vector<std::vector<cv::Point2f>> corners, rejected;

        // detect markers and estimate pose
        detector.detectMarkers(undistorted, corners, ids, rejected);

        std::vector<cv::Point2f> centers;
        for (auto corner : corners) centers.emplace_back((corner[0] + corner[1] + corner[2] + corner[3]) / 4);

        size_t nMarkers = corners.size();
        std::vector<cv::Vec3d> rvecs(nMarkers), tvecs(nMarkers);

        if (estimatePose && !ids.empty())
        {
            // Calculate pose for each marker
            for (size_t i = 0; i < nMarkers; i++)
            {
                solvePnP(objPoints, corners.at(i), this->cameraMatrix, this->distCoeffs, rvecs.at(i), tvecs.at(i));
            }
        }

        undistorted.copyTo(imageCopy);
        // draw section
        if (!ids.empty())
        {
            cv::aruco::drawDetectedMarkers(imageCopy, corners, ids);

            if (estimatePose)
            {
                for (unsigned int i = 0; i < ids.size(); i++) {
                    cv::drawFrameAxes(imageCopy, this->cameraMatrix, this->distCoeffs, rvecs[i], tvecs[i], markerLength * 1.5f, 2);
                }
            }
        }

        if (showRejected && !rejected.empty())
            cv::aruco::drawDetectedMarkers(imageCopy, rejected, cv::noArray(), cv::Scalar(100, 0, 255));

        cv::imshow("Press q to confirm", imageCopy);
        char key = (char)cv::waitKey(waitTime);
        if (key == 'q')
        {
            if(!ids.empty()) {
                float sum = 0;
                int n = 0;
                for (auto corner: corners) {
                    n += 4;
                    sum += cv::norm(corner[0] - corner[1]);
                    sum += cv::norm(corner[1] - corner[2]);
                    sum += cv::norm(corner[2] - corner[3]);
                    sum += cv::norm(corner[3] - corner[0]);
                }
                float l = sum / n;
                float scale = markerLength / l;


                for (auto center: centers) {
                    markerPos.emplace_back(center - cv::Point2f(image.size()) / 2);
                }
                std::cout << "[INFO] Scale: " << scale << " L: " << l << "N: " << n << std::endl;
                std::cout << "[INFO] Centers: " << std::endl;
                for (int i = 0; i < ids.size(); i++)
                    std::cout << "ID: " << ids[i] << " -> " << markerPos[i] * scale << "\t";
                std::cout << std::endl;
                return scale;
            }
        }
    }
    return 0;
}

cv::Point2f Nilox::getDisplacementFromOriginOfID(int id, int origId)
{
    std::vector<cv::Point2f> markerPos;
    std::vector<int> ids;
    float scale = this->detect(markerPos, ids, false);

    bool originfound = false;
    bool idfound = false;

    cv::Point2f displacement(0, 0);
    cv::Point2f origin, idpos;

    for (int i = 0; i < ids.size(); i++)
    {
        if (ids[i] == origId) // origin found
        {
            if (originfound == false)
            {
                origin = markerPos[i];
                originfound = true;
            }
            else
            {
                std::cout << "[WARN] Multiple origins" << std::endl;
                return displacement;
            }
        }
        if (ids[i] == id) // origin found
        {
            if (idfound == false)
            {
                idpos = markerPos[i];
                idfound = true;
            }
            else
            {
                std::cout << "[WARN] Multiple Ids" << std::endl;
                return displacement;
            }
        }
    }
    if (originfound && idfound)
    {
        displacement = (idpos - origin) * scale;
    }
    else
    {
        std::cout << "[WARN] Could not find origin or ID " << id << " in FOV... ids found: ";
        for (int id : ids)
            std::cout << id << " ";
        std::cout << std::endl;
    }

    return displacement;
}

Nilox::~Nilox()
{
    inputVideo.release();
}

// Function to save camera parameters to a file
void saveCameraParameters(const std::string &filename, const cv::Mat &cameraMatrix, const cv::Mat &distCoeffs)
{
    cv::FileStorage fs(filename, cv::FileStorage::WRITE);
    fs << "cameraMatrix" << cameraMatrix;
    fs << "distCoeffs" << distCoeffs;
    fs.release();
}

// Function to load camera parameters from a file
void loadCameraParameters(const std::string &filename, cv::Mat &cameraMatrix, cv::Mat &distCoeffs)
{
    cv::FileStorage fs(filename, cv::FileStorage::READ);
    if (!fs.isOpened())
    {
        throw std::runtime_error("[ERROR] Could not open camera param yml file");
    }
    fs["cameraMatrix"] >> cameraMatrix;
    fs["distCoeffs"] >> distCoeffs;
    fs.release();
}