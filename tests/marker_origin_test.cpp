#include "Moore.h"
#include "Nilox.h"

pos niloxPointToMoore(cv::Point2f p) {
    std::cout << "[INFO] converting: " << p << std::endl;
    pos retp = (pos) {
        .x = p.x,
        .y = -p.y,
        .z = 0
    };

    std::cout << "[INFO] pospos: " << retp << std::endl;
    return retp;
}

int main(void)
{
    Nilox nil;
    nil.init();

    Moore m;
    m.init();
    m.defineZero();

    std::cout << "[INFO] Starting marker origin test" << std::endl;

    auto readAllInstr = [](Moore *m) {
        while (true)
        {
            m->updatePosition();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };
    std::thread readT(readAllInstr, &m);
    readT.detach();

    std::vector<cv::Point2f> markerPos;
    std::vector<int> ids;
    float scale = nil.detect(markerPos, ids);
    std::cout << "[INFO] Found " << ids.size() << " markers" << std::endl;

    for (int i = 0; i < ids.size(); i++) {
        std::cout << "[INFO] Moving to ID: " << ids[i] << " -> " << markerPos[i] * scale << std::endl;
        m.setRelPosition(niloxPointToMoore(-markerPos[i] * scale));
        cv::waitKey(2000);
    }
    std::cout << std::endl;
    return 0;
}

/**
cd tests
g++ nilox_test.cpp ..\src\nilox.cpp -I../src/moore_inc -IC:\Users\X\libs\opencv\build\include -LC:\Users\X\libs\opencv\mybuild\bin -lstdc++ -lopencv_calib3d4100 -lopencv_core4100 -lopencv_dnn4100 -lopencv_features2d4100 -lopencv_flann4100 -lopencv_gapi4100 -lopencv_highgui4100 -lopencv_imgcodecs4100 -lopencv_imgproc4100 -lopencv_ml4100 -lopencv_objdetect4100 -lopencv_photo4100 -lopencv_stitching4100 -lopencv_video4100 -lopencv_videoio4100 -o bin\nilox_test
.\bin\nilox_test.exe
*/