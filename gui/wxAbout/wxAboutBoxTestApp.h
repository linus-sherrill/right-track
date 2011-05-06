/////////////////////////////////////////////////////////////////////////////
// Name:        wxAboutBoxTestApp.h
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     24/01/2009 15:43:26
// RCS-ID:      
// Copyright:   Copyright (c) Volodymir (T-Rex) Tryapichko, 2009
// Licence:     
/////////////////////////////////////////////////////////////////////////////

#ifndef _WXABOUTBOXTESTAPP_H_
#define _WXABOUTBOXTESTAPP_H_


/*!
 * Includes
 */

////@begin includes
#include "wx/image.h"
#include "wxAboutBoxTestMainFrame.h"
////@end includes

/*!
 * Forward declarations
 */

////@begin forward declarations
////@end forward declarations

/*!
 * Control identifiers
 */

////@begin control identifiers
////@end control identifiers

/*!
 * wxAboutBoxTestApp class declaration
 */

class wxAboutBoxTestApp: public wxApp
{    
    DECLARE_CLASS( wxAboutBoxTestApp )
    DECLARE_EVENT_TABLE()

public:
    /// Constructor
    wxAboutBoxTestApp();

    void Init();

    /// Initialises the application
    virtual bool OnInit();

    /// Called on exit
    virtual int OnExit();

////@begin wxAboutBoxTestApp event handler declarations

////@end wxAboutBoxTestApp event handler declarations

////@begin wxAboutBoxTestApp member function declarations

////@end wxAboutBoxTestApp member function declarations

////@begin wxAboutBoxTestApp member variables
////@end wxAboutBoxTestApp member variables
};

/*!
 * Application instance declaration 
 */

////@begin declare app
DECLARE_APP(wxAboutBoxTestApp)
////@end declare app

#endif
    // _WXABOUTBOXTESTAPP_H_
