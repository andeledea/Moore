#include "app.h"
#include "Moore.h"
#include "PosFrame.h"
#include "CHRMeasFrame.h"

// implement the application
wxIMPLEMENT_APP(MyApp);
 
bool MyApp::OnInit()
{
    MyApp::online = true;
    
    MoorePosFrame* posFrame = new MoorePosFrame(nullptr);
    MooreCHRMeasFrame* CHRmeasFrame = new MooreCHRMeasFrame(nullptr);
    
    posFrame->Show(true);
    CHRmeasFrame->Show(true);

#ifndef GUI
    Moore* moore = new Moore();
    moore->init();

    posFrame->setMoore(moore);  // starts the read thread
    CHRmeasFrame->setMoore(moore);
#endif // GUI

    return true;
}

int MyApp::OnExit()
{
    MyApp::online = false;
    return 0;
}

bool MyApp::online = false;
 