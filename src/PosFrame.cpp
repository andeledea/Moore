#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <thread>
#include "PosFrame.h"

MoorePosFrame::MoorePosFrame( wxWindow* parent )
:
PosFrame( parent )
{
	
}

void MoorePosFrame::UpdatePosition()
{
	pos abs = moore->getAbsPosition();
	pos rel = moore->getRelPosition();

	auto _c = [](float f)
	{
		std::stringstream s;
		s << std::fixed << std::setw(8) << std::setfill('0') << std::setprecision(3) << f;
		return s.str();
	};

	this->xabs_lab->SetLabel(_c(abs.x));
	this->yabs_lab->SetLabel(_c(abs.y));
	this->zabs_lab->SetLabel(_c(abs.z));

	this->xrel_lab->SetLabel(_c(rel.x));
	this->yrel_lab->SetLabel(_c(rel.y));
	this->zrel_lab->SetLabel(_c(rel.z));
}

void MoorePosFrame::setMoore(Moore* m)
{
	moore = m;
	std::thread posThread (&Moore::updatePosition, moore);
}

void MoorePosFrame::OnSetAbs_butClick( wxCommandEvent& event )
{
	std::cout << "Set abs" << std::endl;

	pos togo, rel;
	togo.x = atof(this->xpos_in->GetLineText(0));
	togo.y = atof(this->ypos_in->GetLineText(0));
	togo.z = atof(this->zpos_in->GetLineText(0));

	moore->setAbsPosition(togo);
}

void MoorePosFrame::OnSetRel_butClick( wxCommandEvent& event )
{
	std::cout << "Set rel" << std::endl;

	pos abs, togo;
	togo.x = atof(this->xpos_in->GetLineText(0));
	togo.y = atof(this->ypos_in->GetLineText(0));
	togo.z = atof(this->zpos_in->GetLineText(0));

	moore->setRelPosition(togo);
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
	moore->setRelPosition(zero);
}
