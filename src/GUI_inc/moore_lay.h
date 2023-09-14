///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/gbsizer.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/sizer.h>
#include <wx/toolbar.h>
#include <wx/frame.h>
#include <wx/slider.h>
#include <wx/checkbox.h>
#include <wx/filepicker.h>

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class PosFrame
///////////////////////////////////////////////////////////////////////////////
class PosFrame : public wxFrame
{
	private:

	protected:
		wxStaticText* absP_lab;
		wxStaticText* xabs_lab;
		wxStaticText* yabs_lab;
		wxStaticText* zabs_lab;
		wxStaticText* relP_lab;
		wxStaticText* xrel_lab;
		wxStaticText* yrel_lab;
		wxStaticText* zrel_lab;
		wxTextCtrl* xpos_in;
		wxTextCtrl* ypos_in;
		wxTextCtrl* zpos_in;
		wxButton* setAbs_but;
		wxButton* setRel_but;
		wxButton* setZero_but;
		wxButton* gotoZero_but;
		wxToolBar* pos_toolbar;
		wxToolBarToolBase* Pos_set_tool;

		// Virtual event handlers, override them in your derived class
		virtual void OnSetAbs_butClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSetRel_butClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSetZero_butClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGotoZero_butClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnPosSettings_clicked( wxCommandEvent& event ) { event.Skip(); }


	public:

		PosFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("POSITION CONTROL"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("POSITION CONTROL") );

		~PosFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AxisFrame
///////////////////////////////////////////////////////////////////////////////
class AxisFrame : public wxFrame
{
	private:

	protected:
		wxStaticText* x_acc_lab;
		wxSlider* x_acc_slider;
		wxStaticText* x_max_lab;
		wxSlider* x_max_slider;
		wxStaticText* x_start_lab;
		wxSlider* x_start_slider;
		wxStaticText* x_stop_lab;
		wxSlider* x_stop_slider;
		wxStaticText* x_dir_lab;
		wxCheckBox* x_mov_check;
		wxStaticText* y_acc_lab;
		wxSlider* y_acc_slider;
		wxStaticText* y_max_lab;
		wxSlider* y_max_slider;
		wxStaticText* y_start_lab;
		wxSlider* y_start_slider;
		wxStaticText* y_stop_lab;
		wxSlider* y_stop_slider;
		wxStaticText* y_dir_lab;
		wxCheckBox* y_mov_check;
		wxStaticText* z_acc_lab;
		wxSlider* z_acc_slider;
		wxStaticText* z_max_lab2;
		wxSlider* z_max_slider;
		wxStaticText* z_start_lab;
		wxSlider* z_start_slider;
		wxStaticText* z_stop_lab;
		wxSlider* z_stop_slider;
		wxStaticText* z_dir_lab;
		wxCheckBox* z_mov_check;

		// Virtual event handlers, override them in your derived class
		virtual void OnAxisSetting_changed( wxScrollEvent& event ) { event.Skip(); }
		virtual void OnAxisSetting_changed( wxCommandEvent& event ) { event.Skip(); }


	public:

		AxisFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("AXIS SETTINGS"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 715,285 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );

		~AxisFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class CHRMeasFrame
///////////////////////////////////////////////////////////////////////////////
class CHRMeasFrame : public wxFrame
{
	private:

	protected:
		wxStaticText* measname_lab;
		wxTextCtrl* measname_in;
		wxStaticText* speed_lab;
		wxTextCtrl* speed_in;
		wxStaticText* track_lab;
		wxCheckBox* track_check;
		wxStaticText* start_lab;
		wxTextCtrl* start_in;
		wxStaticText* stop_lab;
		wxTextCtrl* stop_in;
		wxDirPickerCtrl* path_in;
		wxButton* start_but;

		// Virtual event handlers, override them in your derived class
		virtual void OnSpeed_Text( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStartCHR_butClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		CHRMeasFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxT("CHR"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 436,227 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("CHR MEASURE CONTROL") );

		~CHRMeasFrame();

};

///////////////////////////////////////////////////////////////////////////////
/// Class AmbFrame
///////////////////////////////////////////////////////////////////////////////
class AmbFrame : public wxFrame
{
	private:

	protected:
		wxStaticText* tempA_lab;
		wxStaticText* tvalA_lab;
		wxStaticText* tempB_lab;
		wxStaticText* tvalB_lab;
		wxStaticText* tempC_lab;
		wxStaticText* tvalC_lab;
		wxStaticText* tempD_lab;
		wxStaticText* tvalD_lab;
		wxStaticText* press_lab;
		wxStaticText* pval_lab;
		wxStaticText* umidity_lab;
		wxStaticText* uval_lab;

	public:

		AmbFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 755,149 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("Ambiental Parameters") );

		~AmbFrame();

};

