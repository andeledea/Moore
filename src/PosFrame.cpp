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
	while (true)
	{
		this->moore->updatePosition();
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		
		pos abs = this->moore->getAbsPosition();
		pos rel = this->moore->getRelPosition();
		
		wxGetApp().CallAfter([this, abs, rel] ()
		{
			this->xabs_lab->SetLabelText(wxString::Format("%f", abs.x));
			this->yabs_lab->SetLabelText(wxString::Format("%f", abs.y));
			this->zabs_lab->SetLabelText(wxString::Format("%f", abs.z));

			this->xrel_lab->SetLabelText(wxString::Format("%f", rel.x));
			this->yrel_lab->SetLabelText(wxString::Format("%f", rel.x));
			this->zrel_lab->SetLabelText(wxString::Format("%f", rel.x));
			
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
	std::cout << "Set abs" << std::endl;

	pos togo, rel;
	togo.x = atof(this->xpos_in->GetLineText(0));
	togo.y = atof(this->ypos_in->GetLineText(0));
	togo.z = atof(this->zpos_in->GetLineText(0));
	
	std::thread mt(&Moore::setAbsPosition, moore, togo);
	mt.detach();
}

void MoorePosFrame::OnSetRel_butClick( wxCommandEvent& event )
{
	std::cout << "Set rel" << std::endl;

	pos abs, togo;
	togo.x = atof(this->xpos_in->GetLineText(0));
	togo.y = atof(this->ypos_in->GetLineText(0));
	togo.z = atof(this->zpos_in->GetLineText(0));

	std::thread mt(&Moore::setRelPosition, moore, togo);
	mt.detach();
}

void MoorePosFrame::OnSetZero_butClick( wxCommandEvent& event )
{
	std::cout << "Set zero" << std::endl;
	moore->defineZero();
}

void MoorePosFrame::OnGotoZero_butClick( wxCommandEvent& event )
{
	std::cout << "Goto zero" << std::endl;
	pos zero;
	std::thread mt(&Moore::setRelPosition, moore, zero);
	mt.detach();
}
