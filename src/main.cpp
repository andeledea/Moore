#include "app.h"
#include "Moore.h"
#include "paramb.h"
#include "PosFrame.h"
#include "CHRMeasFrame.h"
#include "AmbFrame.h"

// implement the application
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    MyApp::online = true;

    MoorePosFrame* posFrame = new MoorePosFrame(nullptr);
    MooreCHRMeasFrame* CHRmeasFrame = new MooreCHRMeasFrame(nullptr);
    MooreLayAmbFrame* AmbFrame = new MooreLayAmbFrame(nullptr);

	Paramb* amb = new Paramb();
    amb->init();
	Moore* moore = new Moore();
    moore->init();

    posFrame->setMoore(moore);  // starts the read thread
    CHRmeasFrame->setMoore(moore);
    AmbFrame->setAmb(amb);  // starts the read amb thread
	
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