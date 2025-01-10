#include "Nilox.h"

int main(void)
{
    Nilox nil;

    nil.init();

    nil.render();
    nil.renderUndistorted();

    while (true)
    {
        std::vector<cv::Point2f> markerPos;
        std::vector<int> ids;
        float scale = nil.detect(markerPos, ids);

        for (int i = 0; i < ids.size(); i++)
            std::cout << "ID: " << ids[i] << " -> " << markerPos[i] * scale << "\t";
        std::cout << std::endl;
    }

    // while (true) {
    //     cv::Point2f p = nil.getDisplacementFromOriginOfID(2);
    //     std::cout << "V -> " << p << "\tD -> " << cv::norm(p) << std::endl;
    // }
    return 0;
}

/**
cd tests
g++ nilox_test.cpp ..\src\nilox.cpp -I../src/moore_inc -IC:\Users\X\libs\opencv\build\include -LC:\Users\X\libs\opencv\mybuild\bin -lstdc++ -lopencv_calib3d4100 -lopencv_core4100 -lopencv_dnn4100 -lopencv_features2d4100 -lopencv_flann4100 -lopencv_gapi4100 -lopencv_highgui4100 -lopencv_imgcodecs4100 -lopencv_imgproc4100 -lopencv_ml4100 -lopencv_objdetect4100 -lopencv_photo4100 -lopencv_stitching4100 -lopencv_video4100 -lopencv_videoio4100 -o bin\nilox_test
.\bin\nilox_test.exe
*/