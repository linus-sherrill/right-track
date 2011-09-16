// -*- C++ -*- generated by wxGlade 0.6.3 on Fri Sep 16 17:16:02 2011

#include <wx/wx.h>
#include <wx/image.h>

#ifndef EVENTTABLE_H
#define EVENTTABLE_H

// begin wxGlade: ::dependencies
#include <wx/statline.h>
#include <wx/grid.h>
// end wxGlade

// begin wxGlade: ::extracode

// end wxGlade


class EventTable: public wxFrame {
public:
    // begin wxGlade: EventTable::ids
    // end wxGlade

    EventTable(wxWindow* parent, int id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_FRAME_STYLE);

private:
    // begin wxGlade: EventTable::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: EventTable::attributes
    wxGrid* data_grid;
    wxButton* done_button;
    wxStaticLine* static_line_3;
    wxButton* save_button;
    wxStaticLine* static_line_4;
    wxButton* cancel_button;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    virtual void handle_done(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void handle_save(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void handle_cancel(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


#endif // EVENTTABLE_H
