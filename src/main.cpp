#include <wx/wx.h>
#include "Moore.h"
#include "PosFrame.h"
 
class MyApp : public wxApp
{
public:
    bool OnInit() override;
};

// implement the application
wxIMPLEMENT_APP(MyApp);
 
bool MyApp::OnInit()
{
    Moore moore;
    moore.init();

    MoorePosFrame* posFrame = new MoorePosFrame(nullptr);
    posFrame->Show(true);
    posFrame->setMoore(&moore);

    return true;
}
 