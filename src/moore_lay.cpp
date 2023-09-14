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
	pos_toolbar = this->CreateToolBar( wxTB_HORIZONTAL, wxID_ANY );
	Pos_set_tool = pos_toolbar->AddTool( wxID_ANY, wxT("Settings"), wxBitmap( wxT("icons/set.bmp"), wxBITMAP_TYPE_ANY ), wxNullBitmap, wxITEM_NORMAL, wxEmptyString, wxEmptyString, NULL );

	pos_toolbar->Realize();


	this->Centre( wxBOTH );

	// Connect Events
	setAbs_but->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetAbs_butClick ), NULL, this );
	setRel_but->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetRel_butClick ), NULL, this );
	setZero_but->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetZero_butClick ), NULL, this );
	gotoZero_but->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnGotoZero_butClick ), NULL, this );
	this->Connect( Pos_set_tool->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( PosFrame::OnPosSettings_clicked ) );
}

PosFrame::~PosFrame()
{
	// Disconnect Events
	setAbs_but->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetAbs_butClick ), NULL, this );
	setRel_but->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetRel_butClick ), NULL, this );
	setZero_but->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnSetZero_butClick ), NULL, this );
	gotoZero_but->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PosFrame::OnGotoZero_butClick ), NULL, this );
	this->Disconnect( Pos_set_tool->GetId(), wxEVT_COMMAND_TOOL_CLICKED, wxCommandEventHandler( PosFrame::OnPosSettings_clicked ) );

}

AxisFrame::AxisFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxGridSizer* gSizer6;
	gSizer6 = new wxGridSizer( 0, 3, 0, 0 );

	wxGridSizer* gSizer7;
	gSizer7 = new wxGridSizer( 0, 2, 0, 0 );

	x_acc_lab = new wxStaticText( this, wxID_ANY, wxT("X ACCELERATION"), wxDefaultPosition, wxDefaultSize, 0 );
	x_acc_lab->Wrap( -1 );
	gSizer7->Add( x_acc_lab, 0, wxALL, 5 );

	x_acc_slider = new wxSlider( this, wxID_ANY, 100, 0, 300, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer7->Add( x_acc_slider, 0, wxALL, 5 );

	x_max_lab = new wxStaticText( this, wxID_ANY, wxT("X MAX SPEED"), wxDefaultPosition, wxDefaultSize, 0 );
	x_max_lab->Wrap( -1 );
	gSizer7->Add( x_max_lab, 0, wxALL, 5 );

	x_max_slider = new wxSlider( this, wxID_ANY, 63, 0, 63, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer7->Add( x_max_slider, 0, wxALL, 5 );

	x_start_lab = new wxStaticText( this, wxID_ANY, wxT("X START SPEED"), wxDefaultPosition, wxDefaultSize, 0 );
	x_start_lab->Wrap( -1 );
	gSizer7->Add( x_start_lab, 0, wxALL, 5 );

	x_start_slider = new wxSlider( this, wxID_ANY, 20, 0, 63, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer7->Add( x_start_slider, 0, wxALL, 5 );

	x_stop_lab = new wxStaticText( this, wxID_ANY, wxT("X STOP SPEED"), wxDefaultPosition, wxDefaultSize, 0 );
	x_stop_lab->Wrap( -1 );
	gSizer7->Add( x_stop_lab, 0, wxALL, 5 );

	x_stop_slider = new wxSlider( this, wxID_ANY, 15, 0, 63, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer7->Add( x_stop_slider, 0, wxALL, 5 );

	x_dir_lab = new wxStaticText( this, wxID_ANY, wxT("X DIRECTION"), wxDefaultPosition, wxDefaultSize, 0 );
	x_dir_lab->Wrap( -1 );
	gSizer7->Add( x_dir_lab, 0, wxALL, 5 );

	x_mov_check = new wxCheckBox( this, wxID_ANY, wxT("INVERTED"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
	gSizer7->Add( x_mov_check, 0, wxALL, 5 );


	gSizer6->Add( gSizer7, 1, wxEXPAND, 5 );

	wxGridSizer* gSizer10;
	gSizer10 = new wxGridSizer( 0, 2, 0, 0 );

	y_acc_lab = new wxStaticText( this, wxID_ANY, wxT("Y ACCELERATION"), wxDefaultPosition, wxDefaultSize, 0 );
	y_acc_lab->Wrap( -1 );
	gSizer10->Add( y_acc_lab, 0, wxALL, 5 );

	y_acc_slider = new wxSlider( this, wxID_ANY, 100, 0, 300, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer10->Add( y_acc_slider, 0, wxALL, 5 );

	y_max_lab = new wxStaticText( this, wxID_ANY, wxT("Y MAX SPEED"), wxDefaultPosition, wxDefaultSize, 0 );
	y_max_lab->Wrap( -1 );
	gSizer10->Add( y_max_lab, 0, wxALL, 5 );

	y_max_slider = new wxSlider( this, wxID_ANY, 63, 0, 63, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer10->Add( y_max_slider, 0, wxALL, 5 );

	y_start_lab = new wxStaticText( this, wxID_ANY, wxT("Y START SPEED"), wxDefaultPosition, wxDefaultSize, 0 );
	y_start_lab->Wrap( -1 );
	gSizer10->Add( y_start_lab, 0, wxALL, 5 );

	y_start_slider = new wxSlider( this, wxID_ANY, 20, 0, 63, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer10->Add( y_start_slider, 0, wxALL, 5 );

	y_stop_lab = new wxStaticText( this, wxID_ANY, wxT("Y STOP SPEED"), wxDefaultPosition, wxDefaultSize, 0 );
	y_stop_lab->Wrap( -1 );
	gSizer10->Add( y_stop_lab, 0, wxALL, 5 );

	y_stop_slider = new wxSlider( this, wxID_ANY, 15, 0, 63, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer10->Add( y_stop_slider, 0, wxALL, 5 );

	y_dir_lab = new wxStaticText( this, wxID_ANY, wxT("Y DIRECTION"), wxDefaultPosition, wxDefaultSize, 0 );
	y_dir_lab->Wrap( -1 );
	gSizer10->Add( y_dir_lab, 0, wxALL, 5 );

	y_mov_check = new wxCheckBox( this, wxID_ANY, wxT("INVERTED"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
	gSizer10->Add( y_mov_check, 0, wxALL, 5 );


	gSizer6->Add( gSizer10, 1, wxEXPAND, 5 );

	wxGridSizer* gSizer11;
	gSizer11 = new wxGridSizer( 0, 2, 0, 0 );

	z_acc_lab = new wxStaticText( this, wxID_ANY, wxT("Z ACCELERATION"), wxDefaultPosition, wxDefaultSize, 0 );
	z_acc_lab->Wrap( -1 );
	gSizer11->Add( z_acc_lab, 0, wxALL, 5 );

	z_acc_slider = new wxSlider( this, wxID_ANY, 100, 0, 300, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer11->Add( z_acc_slider, 0, wxALL, 5 );

	z_max_lab2 = new wxStaticText( this, wxID_ANY, wxT("Z MAX SPEED"), wxDefaultPosition, wxDefaultSize, 0 );
	z_max_lab2->Wrap( -1 );
	gSizer11->Add( z_max_lab2, 0, wxALL, 5 );

	z_max_slider = new wxSlider( this, wxID_ANY, 63, 0, 63, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer11->Add( z_max_slider, 0, wxALL, 5 );

	z_start_lab = new wxStaticText( this, wxID_ANY, wxT("Z START SPEED"), wxDefaultPosition, wxDefaultSize, 0 );
	z_start_lab->Wrap( -1 );
	gSizer11->Add( z_start_lab, 0, wxALL, 5 );

	z_start_slider = new wxSlider( this, wxID_ANY, 20, 0, 63, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer11->Add( z_start_slider, 0, wxALL, 5 );

	z_stop_lab = new wxStaticText( this, wxID_ANY, wxT("Z STOP SPEED"), wxDefaultPosition, wxDefaultSize, 0 );
	z_stop_lab->Wrap( -1 );
	gSizer11->Add( z_stop_lab, 0, wxALL, 5 );

	z_stop_slider = new wxSlider( this, wxID_ANY, 15, 0, 63, wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL|wxSL_VALUE_LABEL );
	gSizer11->Add( z_stop_slider, 0, wxALL, 5 );

	z_dir_lab = new wxStaticText( this, wxID_ANY, wxT("Z DIRECTION"), wxDefaultPosition, wxDefaultSize, 0 );
	z_dir_lab->Wrap( -1 );
	gSizer11->Add( z_dir_lab, 0, wxALL, 5 );

	z_mov_check = new wxCheckBox( this, wxID_ANY, wxT("INVERTED"), wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
	z_mov_check->SetValue(true);
	gSizer11->Add( z_mov_check, 0, wxALL, 5 );


	gSizer6->Add( gSizer11, 1, wxEXPAND, 5 );


	this->SetSizer( gSizer6 );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	x_acc_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_mov_check->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_mov_check->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Connect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Connect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Connect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Connect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Connect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Connect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Connect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Connect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Connect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_mov_check->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
}

AxisFrame::~AxisFrame()
{
	// Disconnect Events
	x_acc_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_acc_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_max_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_start_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_stop_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	x_mov_check->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_acc_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_max_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_start_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_stop_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	y_mov_check->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_acc_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_max_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_start_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Disconnect( wxEVT_SCROLL_TOP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Disconnect( wxEVT_SCROLL_BOTTOM, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Disconnect( wxEVT_SCROLL_LINEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Disconnect( wxEVT_SCROLL_LINEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Disconnect( wxEVT_SCROLL_PAGEUP, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Disconnect( wxEVT_SCROLL_PAGEDOWN, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Disconnect( wxEVT_SCROLL_THUMBTRACK, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Disconnect( wxEVT_SCROLL_THUMBRELEASE, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_stop_slider->Disconnect( wxEVT_SCROLL_CHANGED, wxScrollEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );
	z_mov_check->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( AxisFrame::OnAxisSetting_changed ), NULL, this );

}

CHRMeasFrame::CHRMeasFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* chrBoxSizer;
	chrBoxSizer = new wxBoxSizer( wxVERTICAL );

	wxGridBagSizer* nameSizer;
	nameSizer = new wxGridBagSizer( 0, 0 );
	nameSizer->SetFlexibleDirection( wxBOTH );
	nameSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	measname_lab = new wxStaticText( this, wxID_ANY, wxT("Measure Name"), wxDefaultPosition, wxDefaultSize, 0 );
	measname_lab->Wrap( -1 );
	nameSizer->Add( measname_lab, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );

	measname_in = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !measname_in->HasFlag( wxTE_MULTILINE ) )
	{
	measname_in->SetMaxLength( 30 );
	}
	#else
	measname_in->SetMaxLength( 30 );
	#endif
	nameSizer->Add( measname_in, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL, 5 );


	chrBoxSizer->Add( nameSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxGridBagSizer* ParamSizer;
	ParamSizer = new wxGridBagSizer( 0, 0 );
	ParamSizer->SetFlexibleDirection( wxBOTH );
	ParamSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	speed_lab = new wxStaticText( this, wxID_ANY, wxT("Speed"), wxDefaultPosition, wxDefaultSize, 0 );
	speed_lab->Wrap( -1 );
	fgSizer1->Add( speed_lab, 0, wxALL, 5 );

	speed_in = new wxTextCtrl( this, wxID_ANY, wxT("10"), wxDefaultPosition, wxDefaultSize, 0 );
	#ifdef __WXGTK__
	if ( !speed_in->HasFlag( wxTE_MULTILINE ) )
	{
	speed_in->SetMaxLength( 2 );
	}
	#else
	speed_in->SetMaxLength( 2 );
	#endif
	fgSizer1->Add( speed_in, 0, wxALL, 5 );

	track_lab = new wxStaticText( this, wxID_ANY, wxT("Tracking"), wxDefaultPosition, wxDefaultSize, 0 );
	track_lab->Wrap( -1 );
	fgSizer1->Add( track_lab, 0, wxALL, 5 );

	track_check = new wxCheckBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxCHK_2STATE );
	track_check->SetValue(true);
	fgSizer1->Add( track_check, 0, wxALL, 5 );


	ParamSizer->Add( fgSizer1, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), 0, 5 );

	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 0, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	start_lab = new wxStaticText( this, wxID_ANY, wxT("Start relative X position"), wxDefaultPosition, wxDefaultSize, 0 );
	start_lab->Wrap( -1 );
	fgSizer2->Add( start_lab, 0, wxALL, 5 );

	start_in = new wxTextCtrl( this, wxID_ANY, wxT("+5"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( start_in, 0, wxALL, 5 );

	stop_lab = new wxStaticText( this, wxID_ANY, wxT("Stop relative X position"), wxDefaultPosition, wxDefaultSize, 0 );
	stop_lab->Wrap( -1 );
	fgSizer2->Add( stop_lab, 0, wxALL, 5 );

	stop_in = new wxTextCtrl( this, wxID_ANY, wxT("-5"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( stop_in, 0, wxALL, 5 );


	ParamSizer->Add( fgSizer2, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), 0, 5 );


	chrBoxSizer->Add( ParamSizer, 0, wxEXPAND, 5 );

	wxBoxSizer* pathSizer;
	pathSizer = new wxBoxSizer( wxVERTICAL );

	path_in = new wxDirPickerCtrl( this, wxID_ANY, wxT("C:\\CHRmeasures"), wxT("Select a folder"), wxDefaultPosition, wxDefaultSize, wxDIRP_DEFAULT_STYLE );
	pathSizer->Add( path_in, 0, wxALL|wxEXPAND, 5 );


	chrBoxSizer->Add( pathSizer, 0, wxEXPAND, 5 );

	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );

	start_but = new wxButton( this, wxID_ANY, wxT("Start CHR measure"), wxDefaultPosition, wxDefaultSize, 0 );
	start_but->SetFont( wxFont( 14, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	bSizer4->Add( start_but, 1, wxALL|wxEXPAND, 5 );


	chrBoxSizer->Add( bSizer4, 1, wxEXPAND, 5 );


	this->SetSizer( chrBoxSizer );
	this->Layout();

	this->Centre( wxBOTH );

	// Connect Events
	speed_in->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CHRMeasFrame::OnSpeed_Text ), NULL, this );
	start_but->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CHRMeasFrame::OnStartCHR_butClick ), NULL, this );
}

CHRMeasFrame::~CHRMeasFrame()
{
	// Disconnect Events
	speed_in->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CHRMeasFrame::OnSpeed_Text ), NULL, this );
	start_but->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CHRMeasFrame::OnStartCHR_butClick ), NULL, this );

}

AmbFrame::AmbFrame( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style, const wxString& name ) : wxFrame( parent, id, title, pos, size, style, name )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );

	wxBoxSizer* amb_sizer;
	amb_sizer = new wxBoxSizer( wxVERTICAL );

	wxGridSizer* tempSizer;
	tempSizer = new wxGridSizer( 0, 4, 0, 0 );

	wxGridBagSizer* tempASizer;
	tempASizer = new wxGridBagSizer( 0, 0 );
	tempASizer->SetFlexibleDirection( wxBOTH );
	tempASizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	tempA_lab = new wxStaticText( this, wxID_ANY, wxT("T_A"), wxDefaultPosition, wxDefaultSize, 0 );
	tempA_lab->Wrap( -1 );
	tempA_lab->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	tempASizer->Add( tempA_lab, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	tvalA_lab = new wxStaticText( this, wxID_ANY, wxT("00.000"), wxDefaultPosition, wxDefaultSize, 0 );
	tvalA_lab->Wrap( -1 );
	tvalA_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	tempASizer->Add( tvalA_lab, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	tempSizer->Add( tempASizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );

	wxGridBagSizer* tempBSizer;
	tempBSizer = new wxGridBagSizer( 0, 0 );
	tempBSizer->SetFlexibleDirection( wxBOTH );
	tempBSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	tempB_lab = new wxStaticText( this, wxID_ANY, wxT("T_B"), wxDefaultPosition, wxDefaultSize, 0 );
	tempB_lab->Wrap( -1 );
	tempB_lab->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	tempBSizer->Add( tempB_lab, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	tvalB_lab = new wxStaticText( this, wxID_ANY, wxT("00.000"), wxDefaultPosition, wxDefaultSize, 0 );
	tvalB_lab->Wrap( -1 );
	tvalB_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	tempBSizer->Add( tvalB_lab, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	tempSizer->Add( tempBSizer, 1, wxEXPAND, 5 );

	wxGridBagSizer* tempCSizer;
	tempCSizer = new wxGridBagSizer( 0, 0 );
	tempCSizer->SetFlexibleDirection( wxBOTH );
	tempCSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	tempC_lab = new wxStaticText( this, wxID_ANY, wxT("T_C"), wxDefaultPosition, wxDefaultSize, 0 );
	tempC_lab->Wrap( -1 );
	tempC_lab->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	tempCSizer->Add( tempC_lab, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	tvalC_lab = new wxStaticText( this, wxID_ANY, wxT("00.000"), wxDefaultPosition, wxDefaultSize, 0 );
	tvalC_lab->Wrap( -1 );
	tvalC_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	tempCSizer->Add( tvalC_lab, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	tempSizer->Add( tempCSizer, 1, wxEXPAND, 5 );

	wxGridBagSizer* tempDSizer;
	tempDSizer = new wxGridBagSizer( 0, 0 );
	tempDSizer->SetFlexibleDirection( wxBOTH );
	tempDSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	tempD_lab = new wxStaticText( this, wxID_ANY, wxT("T_D"), wxDefaultPosition, wxDefaultSize, 0 );
	tempD_lab->Wrap( -1 );
	tempD_lab->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	tempDSizer->Add( tempD_lab, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	tvalD_lab = new wxStaticText( this, wxID_ANY, wxT("00.000"), wxDefaultPosition, wxDefaultSize, 0 );
	tvalD_lab->Wrap( -1 );
	tvalD_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	tempDSizer->Add( tvalD_lab, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	tempSizer->Add( tempDSizer, 1, wxEXPAND, 5 );


	amb_sizer->Add( tempSizer, 1, wxEXPAND, 5 );

	wxGridSizer* puSizer;
	puSizer = new wxGridSizer( 0, 2, 0, 0 );

	wxGridBagSizer* pSizer;
	pSizer = new wxGridBagSizer( 0, 0 );
	pSizer->SetFlexibleDirection( wxBOTH );
	pSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	press_lab = new wxStaticText( this, wxID_ANY, wxT("P"), wxDefaultPosition, wxDefaultSize, 0 );
	press_lab->Wrap( -1 );
	press_lab->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	pSizer->Add( press_lab, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	pval_lab = new wxStaticText( this, wxID_ANY, wxT("00.000"), wxDefaultPosition, wxDefaultSize, 0 );
	pval_lab->Wrap( -1 );
	pval_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	pSizer->Add( pval_lab, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	puSizer->Add( pSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );

	wxGridBagSizer* uSizer;
	uSizer = new wxGridBagSizer( 0, 0 );
	uSizer->SetFlexibleDirection( wxBOTH );
	uSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );

	umidity_lab = new wxStaticText( this, wxID_ANY, wxT("U"), wxDefaultPosition, wxDefaultSize, 0 );
	umidity_lab->Wrap( -1 );
	umidity_lab->SetFont( wxFont( 20, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	uSizer->Add( umidity_lab, wxGBPosition( 0, 0 ), wxGBSpan( 1, 1 ), wxALL, 5 );

	uval_lab = new wxStaticText( this, wxID_ANY, wxT("00.000"), wxDefaultPosition, wxDefaultSize, 0 );
	uval_lab->Wrap( -1 );
	uval_lab->SetFont( wxFont( 18, wxFONTFAMILY_SWISS, wxFONTSTYLE_NORMAL, wxFONTWEIGHT_BOLD, false, wxT("Arial") ) );

	uSizer->Add( uval_lab, wxGBPosition( 0, 1 ), wxGBSpan( 1, 1 ), wxALL|wxALIGN_CENTER_VERTICAL, 5 );


	puSizer->Add( uSizer, 1, wxEXPAND|wxALIGN_CENTER_HORIZONTAL, 5 );


	amb_sizer->Add( puSizer, 1, wxEXPAND, 5 );


	this->SetSizer( amb_sizer );
	this->Layout();

	this->Centre( wxBOTH );
}

AmbFrame::~AmbFrame()
{
}
