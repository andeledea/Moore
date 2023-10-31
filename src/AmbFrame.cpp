#include <thread>
#include <chrono>

#include "app.h"
#include "AmbFrame.h"

MooreLayAmbFrame::MooreLayAmbFrame( wxWindow* parent )
:
AmbFrame( parent )
{
	
}

void MooreLayAmbFrame::setAmb(Paramb* a)
{
	amb = a;
#ifndef GUI
	std::thread ambThread{ &MooreLayAmbFrame::UpdateAmb, this };
	ambThread.detach();
#endif // !GUI
}

void MooreLayAmbFrame::UpdateAmb()
{
	ambVals v;
	while (true)
	{
		v = this->amb->scanParams();

		wxGetApp().CallAfter([this, v]()
		{
			this->tvalA_lab->SetLabelText(wxString::Format("%7.3f", v.t[0]));
			this->tvalB_lab->SetLabelText(wxString::Format("%7.3f", v.t[1]));
			this->tvalC_lab->SetLabelText(wxString::Format("%7.3f", v.t[2]));
			this->tvalD_lab->SetLabelText(wxString::Format("%7.3f", v.t[3]));

			this->pval_lab->SetLabelText(wxString::Format("%7.3f", v.p));
			this->uval_lab->SetLabelText(wxString::Format("%7.3f", v.u));
		});

		std::this_thread::sleep_for(std::chrono::seconds(60));
	}
}
