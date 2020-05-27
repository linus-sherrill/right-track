// -*- C++ -*-
//
// generated by wxGlade 0.9.5 on Wed May 27 16:06:46 2020
//
// Example for compiling a single file project under Linux using g++:
//  g++ MyApp.cpp $(wx-config --libs) $(wx-config --cxxflags) -o MyApp
//
// Example for compiling a multi file project under Linux using g++:
//  g++ main.cpp $(wx-config --libs) $(wx-config --cxxflags) -o MyApp Dialog1.cpp Frame1.cpp
//

#ifndef EVENTCANVAS_H
#define EVENTCANVAS_H

#include <wx/wx.h>
#include <wx/image.h>

// begin wxGlade: ::dependencies
// end wxGlade

// begin wxGlade: ::extracode
#include <EventCanvasApp.h>
// end wxGlade


class EventCanvas: public EventCanvasApp {
public:
    // begin wxGlade: EventCanvas::ids
    // end wxGlade

    EventCanvas(wxWindow* parent, wxWindowID id, const wxPoint& pos=wxDefaultPosition, const wxSize& size=wxDefaultSize, long style=0);

private:
    // begin wxGlade: EventCanvas::methods
    void set_properties();
    void do_layout();
    // end wxGlade

protected:
    // begin wxGlade: EventCanvas::attributes
    // end wxGlade
}; // wxGlade: end class


#endif // EVENTCANVAS_H
