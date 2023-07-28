///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version 3.10.1-0-g8feb16b3)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include "moore_lay.h"

///////////////////////////////////////////////////////////////////////////

PosFrame::PosFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxSize( 500,300 ), wxSize( 500,300 ) );

	wxBoxSizer* posBoxSizer;
	posBoxSizer = new wxBoxSizer( wxVERTICAL );

	wxGridBagSizer* absSizer;
	absSizer = new wxGridBagSizer( 0, 0 );
	absSizer->SetFlexibleDirection( wxBOTH );
	absSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	absP_lab = new wxStaticText( this, wxID_ANY, wxT("ABS POSITION"), wxDefaultPosition, wxDefaultSize, 0 );
	absP_lab->Wrap( -1 );
	absP_lab->SetFont( wxFont( 11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );

	absSizer->Add( absP_lab, wxGBPosition( 0, 6 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	xabs_lab = new wxStaticText( this, wxID_ANY, wxT("0000.000"), wxDefaultPosition, wxDefaultSize, 0 );
	xabs_lab->Wrap( -1 );
	xabs_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	absSizer->Add( xabs_lab, wxGBPosition( 0, 3 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	yabs_lab = new wxStaticText( this, wxID_ANY, wxT("0000.000"), wxDefaultPosition, wxDefaultSize, 0 );
	yabs_lab->Wrap( -1 );
	yabs_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	absSizer->Add( yabs_lab, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	zabs_lab = new wxStaticText( this, wxID_ANY, wxT("0000.000"), wxDefaultPosition, wxDefaultSize, 0 );
	zabs_lab->Wrap( -1 );
	zabs_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	absSizer->Add( zabs_lab, wxGBPosition( 0, 5 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	posBoxSizer->Add( absSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxGridBagSizer* relSizer;
	relSizer = new wxGridBagSizer( 0, 0 );
	relSizer->SetFlexibleDirection( wxBOTH );
	relSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	relP_lab = new wxStaticText( this, wxID_ANY, wxT("REL POSITION"), wxDefaultPosition, wxDefaultSize, 0 );
	relP_lab->Wrap( -1 );
	relP_lab->SetFont( wxFont( 11, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_NORMAL, false, wxT("Arial") ) );

	relSizer->Add( relP_lab, wxGBPosition( 0, 7 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	xrel_lab = new wxStaticText( this, wxID_ANY, wxT("0000.000"), wxDefaultPosition, wxDefaultSize, 0 );
	xrel_lab->Wrap( -1 );
	xrel_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	relSizer->Add( xrel_lab, wxGBPosition( 0, 4 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	yrel_lab = new wxStaticText( this, wxID_ANY, wxT("0000.000"), wxDefaultPosition, wxDefaultSize, 0 );
	yrel_lab->Wrap( -1 );
	yrel_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	relSizer->Add( yrel_lab, wxGBPosition( 0, 5 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	zrel_lab = new wxStaticText( this, wxID_ANY, wxT("0000.000"), wxDefaultPosition, wxDefaultSize, 0 );
	zrel_lab->Wrap( -1 );
	zrel_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	relSizer->Add( zrel_lab, wxGBPosition( 0, 6 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	posBoxSizer->Add( relSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxGridBagSizer* inputPosSizer;
	inputPosSizer = new wxGridBagSizer( 0, 0 );
	inputPosSizer->SetFlexibleDirection( wxBOTH );
	inputPosSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	xpos_in = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	inputPosSizer->Add( xpos_in, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_HORIZONTAL|wxALIGN_CENTER_VERTICAL, 5 );

	ypos_in = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	inputPosSizer->Add( ypos_in, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );

	zpos_in = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	inputPosSizer->Add( zpos_in, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_HORIZONTAL, 5 );


	posBoxSizer->Add( inputPosSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxBoxSizer* CmdSizer;
	CmdSizer = new wxBoxSizer( wxVERTICAL );

	wxGridBagSizer* posSizer;
	posSizer = new wxGridBagSizer( 0, 0 );
	posSizer->SetFlexibleDirection( wxBOTH );
	posSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	setAbs_but = new wxButton( this, wxID_ANY, wxT("Set absolute"), wxDefaultPosition, wxDefaultSize, 0 );
	posSizer->Add( setAbs_but, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );


	posSizer->Add( 20, 0, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), 0, 5 );

	setRel_but = new wxButton( this, wxID_ANY, wxT("Set relative"), wxDefaultPosition, wxDefaultSize, 0 );
	posSizer->Add( setRel_but, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );


	CmdSizer->Add( posSizer, 1, wxALIGN_CENTER, 5 );

	wxGridBagSizer* zeroSizer;
	zeroSizer = new wxGridBagSizer( 0, 0 );
	zeroSizer->SetFlexibleDirection( wxBOTH );
	zeroSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	setZero_but = new wxButton( this, wxID_ANY, wxT("Set Zero"), wxDefaultPosition, wxDefaultSize, 0 );
	zeroSizer->Add( setZero_but, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );


	zeroSizer->Add( 20, 0, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxEXPAND, 5 );

	gotoZero_but = new wxButton( this, wxID_ANY, wxT("Go to Zero"), wxDefaultPosition, wxDefaultSize, 0 );
	zeroSizer->Add( gotoZero_but, wxGBPosition( 0, 2 ), wxGBSpan( 1, 1 ), wxALL, 5 );


	CmdSizer->Add( zeroSizer, 1, wxALIGN_CENTER_HORIZONTAL, 5 );


	posBoxSizer->Add( CmdSizer, 1, wxEXPAND, 5 );


	this->SetSizer( posBoxSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	setAbs_but->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetAbs_butClick ), NULL, this );
	setRel_but->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetRel_butClick ), NULL, this );
	setZero_but->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetZero_butClick ), NULL, this );
	gotoZero_but->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnGotoZero_butClick ), NULL, this );
}

PosFrame::~PosFrame()
{
	// Disconnect Events
	setAbs_but->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetAbs_butClick ), NULL, this );
	setRel_but->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetRel_butClick ), NULL, this );
	setZero_but->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetZero_butClick ), NULL, this );
	gotoZero_but->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnGotoZero_butClick ), NULL, this );

}
