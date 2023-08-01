#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>

#include "app.h"
#include "PosFrame.h"

MoorePosFrame::MoorePosFrame( wxWindow* parent )
:
PosFrame( parent )
{
	
}

void MoorePosFrame::UpdatePosition()
{
	/*auto _c = [](double f)
	{
		std::stringstream s;
		s << std::fixed << std::setw(8) << std::setfill('0') << std::setprecision(3) << f;
		return s.str();
	};*/
	while (true)
	{
		this->moore->updatePosition();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		
		pos abs = this->moore->getAbsPosition();
		pos rel = this->moore->getRelPosition();
		
		wxGetApp().CallAfter([this, abs, rel] ()
		{
			this->xabs_lab->SetLabelText(wxString::Format("%8.3f", abs.x));
			this->yabs_lab->SetLabelText(wxString::Format("%8.3f", abs.y));
			this->zabs_lab->SetLabelText(wxString::Format("%8.3f", abs.z));

			this->xrel_lab->SetLabelText(wxString::Format("%8.3f", rel.x));
			this->yrel_lab->SetLabelText(wxString::Format("%8.3f", rel.y));
			this->zrel_lab->SetLabelText(wxString::Format("%8.3f", rel.z));
			
			this->Layout();
		});
	}
}

void MoorePosFrame::setMoore(Moore* m)
{
	this->moore = m;
	std::thread posThread {&MoorePosFrame::UpdatePosition, this};
	posThread.detach();
}

void MoorePosFrame::OnSetAbs_butClick( wxCommandEvent& event )
{
	pos togo, rel;
	togo.x = atof(this->xpos_in->GetLineText(0));
	togo.y = atof(this->ypos_in->GetLineText(0));
	togo.z = atof(this->zpos_in->GetLineText(0));
	
	std::thread mt(&Moore::setAbsPosition, moore, togo);
	mt.detach();
}

void MoorePosFrame::OnSetRel_butClick( wxCommandEvent& event )
{
	pos abs, togo;
	togo.x = atof(this->xpos_in->GetLineText(0));
	togo.y = atof(this->ypos_in->GetLineText(0));
	togo.z = atof(this->zpos_in->GetLineText(0));

	std::thread mt(&Moore::setRelPosition, moore, togo);
	mt.detach();
}

void MoorePosFrame::OnSetZero_butClick( wxCommandEvent& event )
{
	moore->defineZero();
}

void MoorePosFrame::OnGotoZero_butClick( wxCommandEvent& event )
{
	pos zero;
	std::thread mt(&Moore::setRelPosition, moore, zero);
	mt.detach();
}
