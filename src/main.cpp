#include <wx/wx.h>
#include "Moore.h"
#include "PosFrame.h"
#include "CHRMeasFrame.h"
 
class MyApp : public wxApp
{
public:
    bool OnInit() override;
    int OnExit() override;

    static bool online;
};

// implement the application
wxIMPLEMENT_APP(MyApp);
 
bool MyApp::OnInit()
{
    MyApp::online = true;

    Moore moore;
    // moore.init();

    MoorePosFrame* posFrame = new MoorePosFrame(nullptr);
    MooreCHRMeasFrame* CHRmeasFrame = new MooreCHRMeasFrame(nullptr);
    
    posFrame->Show(true);
    CHRmeasFrame->Show(true);

    // posFrame->setMoore(&moore);  // starts the read thread
    // CHRmeasFrame->setMoore(&moore);

    return true;
}

int MyApp::OnExit()
{
    MyApp::online = false;
    return 0;
}

bool MyApp::online = false;
 