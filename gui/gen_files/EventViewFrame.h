// -*- C++ -*- generated by wxGlade 0.6.3 on Wed Jul 13 11:45:57 2011

#include <wx/wx.h>
#include <wx/image.h>

#ifndef EVENTVIEWFRAME_H
#define EVENTVIEWFRAME_H

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade


class EventViewFrame: public wxFrame {
public:
    // begin wxGlade: EventViewFrame::ids
    // end wxGlade

    EventViewFrame(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    // begin wxGlade: EventViewFrame::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: EventViewFrame::attributes
    wxScrolledWindow* EventWindow;
    // end wxGlade
}; // wxGlade: end class


#endif // EVENTVIEWFRAME_H
