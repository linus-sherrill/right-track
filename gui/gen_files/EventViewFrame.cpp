// -*- C++ -*- generated by wxGlade 0.6.3 on Wed May  4 18:54:19 2011

#include "EventViewFrame.h"

// begin wxGlade: ::extracode

// end wxGlade


EventViewFrame::EventViewFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos, const wxSize& size, long style):
    wxFrame(parent, id, title, pos, size, wxDEFAULT_FRAME_STYLE)
{
    // begin wxGlade: EventViewFrame::EventViewFrame
    EventWindow = new wxScrolledWindow(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);

    set_properties();
    do_layout();
    // end wxGlade
}


void EventViewFrame::set_properties()
{
    // begin wxGlade: EventViewFrame::set_properties
    SetTitle(wxT("frame_2"));
    EventWindow->SetScrollRate(10, 10);
    // end wxGlade
}


void EventViewFrame::do_layout()
{
    // begin wxGlade: EventViewFrame::do_layout
    wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer* sizer_2 = new wxBoxSizer(wxVERTICAL);
    sizer_1->Add(EventWindow, 3, wxEXPAND, 0);
    sizer_1->Add(sizer_2, 1, wxEXPAND, 0);
    SetSizer(sizer_1);
    sizer_1->Fit(this);
    Layout();
    // end wxGlade
}
