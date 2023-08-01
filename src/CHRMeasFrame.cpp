#include <string>
#include <thread>
#include "CHRMeasFrame.h"

MooreCHRMeasFrame::MooreCHRMeasFrame( wxWindow* parent )
:
CHRMeasFrame( parent )
{

}

void MooreCHRMeasFrame::OnSpeed_Text( wxCommandEvent& event )
{
	int speed = atoi(this->speed_in->GetLineText(0));
	if (speed < 0) this->speed_in->SetLabelText("0");
	if (speed > 63) this->speed_in->SetLabelText("63");
}

void MooreCHRMeasFrame::OnStartCHR_butClick( wxCommandEvent& event )
{
	std::thread mt(
		&Moore::measCHR, moore,

		std::string(this->measname_in->GetLineText(0)),
		std::string(this->path_in->GetLineText(0)),
		atoi(this->speed_in->GetLineText(0)),
		this->track_check->GetValue(),
		atof(this->start_in->GetLineText(0)),
		atof(this->stop_in->GetLineText(0))
	);
	mt.detach();
}

void MooreCHRMeasFrame::setMoore(Moore* m)
{
	moore = m;
}