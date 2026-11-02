#include <algorithm>
#include "AxisFrame.h"

MooreAxisFrame::MooreAxisFrame( wxWindow* parent )
:
AxisFrame( parent )
{
	
}

void MooreAxisFrame::setMoore(Moore* m)
{
	this->moore = m;

	x_acc_slider	->	SetValue(moore->Xaxis.acceleration);
	x_start_slider	->	SetValue(moore->Xaxis.startV);
	x_max_slider	->	SetValue(moore->Xaxis.maxV);
	x_stop_slider	->	SetValue(moore->Xaxis.stopV);
	x_mov_check		->	SetValue(moore->Xaxis.invertedMovement);

	y_acc_slider	->	SetValue(moore->Yaxis.acceleration);
	y_start_slider	->	SetValue(moore->Yaxis.startV);
	y_max_slider	->	SetValue(moore->Yaxis.maxV);
	y_stop_slider	->	SetValue(moore->Yaxis.stopV);
	y_mov_check		->	SetValue(moore->Yaxis.invertedMovement);

	z_acc_slider	->	SetValue(moore->Zaxis.acceleration);
	z_start_slider	->	SetValue(moore->Zaxis.startV);
	z_max_slider	->	SetValue(moore->Zaxis.maxV);
	z_stop_slider	->	SetValue(moore->Zaxis.stopV);
	z_mov_check		->	SetValue(moore->Zaxis.invertedMovement);
}

void MooreAxisFrame::OnAxisSetting_changed( wxScrollEvent& event )
{
	this->OnAxisSetting_changed();
}


void MooreAxisFrame::OnAxisSetting_changed(wxCommandEvent& event)
{
	this->OnAxisSetting_changed();
}

void MooreAxisFrame::OnAxisSetting_changed()
{
	// acc, start, max, stop
	int maxm, start, stop;

	start = x_start_slider->GetValue();
	stop = x_stop_slider->GetValue();
	x_max_slider->SetMin(std::max(start, stop));
	moore->Xaxis.setRamp
	(
		x_acc_slider->GetValue(),
		start, x_max_slider->GetValue(), stop,
		x_mov_check->GetValue()
	);

	start = y_start_slider->GetValue();
	stop = y_stop_slider->GetValue();
	y_max_slider->SetMin(std::max(start, stop));
	moore->Yaxis.setRamp
	(
		y_acc_slider->GetValue(),
		start, y_max_slider->GetValue(), stop,
		y_mov_check->GetValue()
	);

	start = z_start_slider->GetValue();
	stop = z_stop_slider->GetValue();
	z_max_slider->SetMin(std::max(start, stop));
	moore->Zaxis.setRamp
	(
		z_acc_slider->GetValue(),
		start, z_max_slider->GetValue(), stop,
		z_mov_check->GetValue()
	);
}
