// -*- C++ -*-
//
// generated by wxGlade 0.8.2 on Mon Nov 26 17:05:49 2018
//
// Example for compiling a single file project under Linux using g++:
//  g++ MyApp.cpp $(wx-config --libs) $(wx-config --cxxflags) -o MyApp
//
// Example for compiling a multi file project under Linux using g++:
//  g++ main.cpp $(wx-config --libs) $(wx-config --cxxflags) -o MyApp Dialog1.cpp Frame1.cpp
//

#ifndef TEXTEDITDIALOG_H
#define TEXTEDITDIALOG_H

#include <wx/wx.h>
#include <wx/image.h>

// begin wxGlade: ::dependencies
#include <wx/statline.h>
// end wxGlade

// begin wxGlade: ::extracode
// end wxGlade


class TextEditDialog: public wxDialog {
public:
    // begin wxGlade: TextEditDialog::ids
    // end wxGlade

    TextEditDialog(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=wxDEFAULT_DIALOG_STYLE);

private:
    // begin wxGlade: TextEditDialog::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: TextEditDialog::attributes
    wxTextCtrl* TextField;
    wxButton* ok_button;
    wxStaticLine* static_line_3;
    wxButton* clear_button;
    wxStaticLine* static_line_4;
    wxButton* cancel_button;
    // end wxGlade

    DECLARE_EVENT_TABLE();

public:
    void handle_clear(wxCommandEvent &event); // wxGlade: <event_handler>
}; // wxGlade: end class


#endif // TEXTEDITDIALOG_H
