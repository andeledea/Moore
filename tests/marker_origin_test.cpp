#include "Moore.h"
#include "Nilox.h"

int main(void)
{
    Nilox nil;
    nil.init();

    Moore m;
    m.init();
    m.defineZero();

    std::cout << "Starting marker origin test" << std::endl;

    auto readAllInstr = [](Moore *m) {
        while (true)
        {
            m->updatePosition();
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };
    std::thread readT(readAllInstr, &m);
    readT.detach();

    std::vector<pos> markerPos;
    std::vector<int> ids;
    
    pos p = nil.getDisplacementOfIDzeroInFOV();
    p.x = -p.x;
    p.y = -p.y;
    std::cout << "Origin displacement: " << p.x << '\t' << p.y << std::endl;

    m.setRelPosition(p);

    return 0;
}

/**
cd tests
g++ nilox_test.cpp ..\src\nilox.cpp -I../src/moore_inc -IC:\Users\X\libs\opencv\build\include -LC:\Users\X\libs\opencv\mybuild\bin -lstdc++ -lopencv_calib3d4100 -lopencv_core4100 -lopencv_dnn4100 -lopencv_features2d4100 -lopencv_flann4100 -lopencv_gapi4100 -lopencv_highgui4100 -lopencv_imgcodecs4100 -lopencv_imgproc4100 -lopencv_ml4100 -lopencv_objdetect4100 -lopencv_photo4100 -lopencv_stitching4100 -lopencv_video4100 -lopencv_videoio4100 -o bin\nilox_test
.\bin\nilox_test.exe
*/