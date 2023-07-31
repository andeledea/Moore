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
#include <wx/frame.h>
#include <wx/checkbox.h>

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

		// Virtual event handlers, override them in your derived class
		virtual void OnSetAbs_butClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSetRel_butClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnSetZero_butClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGotoZero_butClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		PosFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 500,300 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("POSITION CONTROL") );

		~PosFrame();

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
		wxTextCtrl* path_in;
		wxButton* start_but;

		// Virtual event handlers, override them in your derived class
		virtual void OnSpeed_Text( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnStartCHR_butClick( wxCommandEvent& event ) { event.Skip(); }


	public:

		CHRMeasFrame( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 436,227 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL, const wxString& name = wxT("CHR MEASURE CONTROL") );

		~CHRMeasFrame();

};

