/////////////////////////////////////////////////////////////////////////////
// Name:        wxAboutBoxTestApp.cpp
// Purpose:     
// Author:      Volodymir (T-Rex) Tryapichko
// Modified by: 
// Created:     24/01/2009 15:43:26
// RCS-ID:      
// Copyright:   Copyright (c) Volodymir (T-Rex) Tryapichko, 2009
// Licence:     
/////////////////////////////////////////////////////////////////////////////

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif

////@begin includes
////@end includes

#include "wxAboutBoxTestApp.h"

////@begin XPM images
////@end XPM images


/*!
 * Application instance implementation
 */

////@begin implement app
IMPLEMENT_APP( wxAboutBoxTestApp )
////@end implement app


/*!
 * wxAboutBoxTestApp type definition
 */

IMPLEMENT_CLASS( wxAboutBoxTestApp, wxApp )


/*!
 * wxAboutBoxTestApp event table definition
 */

BEGIN_EVENT_TABLE( wxAboutBoxTestApp, wxApp )

////@begin wxAboutBoxTestApp event table entries
////@end wxAboutBoxTestApp event table entries

END_EVENT_TABLE()


/*!
 * Constructor for wxAboutBoxTestApp
 */

wxAboutBoxTestApp::wxAboutBoxTestApp()
{
    Init();
}


/*!
 * Member initialisation
 */

void wxAboutBoxTestApp::Init()
{
////@begin wxAboutBoxTestApp member initialisation
////@end wxAboutBoxTestApp member initialisation
}

/*!
 * Initialisation for wxAboutBoxTestApp
 */

bool wxAboutBoxTestApp::OnInit()
{    
	SetAppName(_("AboutBox Sample"));
////@begin wxAboutBoxTestApp initialisation
	// Remove the comment markers above and below this block
	// to make permanent changes to the code.

#if wxUSE_XPM
	wxImage::AddHandler(new wxXPMHandler);
#endif
#if wxUSE_LIBPNG
	wxImage::AddHandler(new wxPNGHandler);
#endif
#if wxUSE_LIBJPEG
	wxImage::AddHandler(new wxJPEGHandler);
#endif
#if wxUSE_GIF
	wxImage::AddHandler(new wxGIFHandler);
#endif
	wxAboutBoxTestMainFrame* mainWindow = new wxAboutBoxTestMainFrame( NULL );
	mainWindow->Show(true);
////@end wxAboutBoxTestApp initialisation

    return true;
}


/*!
 * Cleanup for wxAboutBoxTestApp
 */

int wxAboutBoxTestApp::OnExit()
{    
////@begin wxAboutBoxTestApp cleanup
	return wxApp::OnExit();
////@end wxAboutBoxTestApp cleanup
}

