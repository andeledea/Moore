#include <thread>
#include <chrono>

#include "app.h"
#include "layAmbFrame.h"

Moore_layAmbFrame::Moore_layAmbFrame( wxWindow* parent )
:
AmbFrame( parent )
{

}

void Moore_layAmbFrame::setAmb(Paramb* a)
{
	amb = a;
#ifndef GUI
	std::thread ambThread{ &Moore_layAmbFrame::UpdateAmb, this };
	ambThread.detach();
#endif // !GUI
}

void Moore_layAmbFrame::UpdateAmb()
{
	ambVals v;
	while (true)
	{
		v = this->amb->scanParams();

		wxGetApp().CallAfter([this, v]()
		{
			this->tval_lab->SetLabelText(wxString::Format("%7.3f", v.t));
		});

		std::this_thread::sleep_for(std::chrono::seconds(20));
	}
}

void Moore_layAmbFrame::OnTval_butClick( wxCommandEvent& event )
{
	std::cout << "ciao" << std::endl;
}
