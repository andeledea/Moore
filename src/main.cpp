#include "app.h"
#include "Moore.h"
#include "paramb.h"
#include "PosFrame.h"
#include "CHRMeasFrame.h"
#include "layAmbFrame.h"

// implement the application
wxIMPLEMENT_APP(MyApp);
 
bool MyApp::OnInit()
{
    MyApp::online = true;

    MoorePosFrame* posFrame = new MoorePosFrame(nullptr);
    MooreCHRMeasFrame* CHRmeasFrame = new MooreCHRMeasFrame(nullptr);
    Moore_layAmbFrame* AmbFrame = new Moore_layAmbFrame(nullptr);

	Moore* moore = new Moore(); 
    moore->init();

    Paramb* amb = new Paramb();
    amb->init();

    posFrame->setMoore(moore);  // starts the read thread
    CHRmeasFrame->setMoore(moore);
    AmbFrame->setAmb(amb);  // starts the read thread
	
	posFrame->Show(true);
    CHRmeasFrame->Show(true);
    AmbFrame->Show(true);
	
    return true;
}

int MyApp::OnExit()
{
    MyApp::online = false;
    return 0;
}

bool MyApp::online = false;