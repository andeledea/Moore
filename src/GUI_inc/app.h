#include <wx/wx.h>
 
class MyApp : public wxApp
{
public:
    bool OnInit() override;
    int OnExit() override;

    static bool online;
};

wxDECLARE_APP(MyApp);