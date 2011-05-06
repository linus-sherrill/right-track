/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <MainFrameApp.h>

#include <wx/colordlg.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/dcclient.h>
#include <wx/pen.h>


// Define our custom event table
BEGIN_EVENT_TABLE (MainFrameApp, MainFrame)
    EVT_UPDATE_UI( wxID_REFRESH, MainFrameApp::OnModelUpdate)
END_EVENT_TABLE()


MainFrameApp::
MainFrameApp(wxWindow* parent,
             int id,
             const wxString& title,
             const wxPoint& pos,
             const wxSize& size,
             long style)
  : MainFrame (parent, id, title, pos, size, style)
{

}

MainFrameApp::
~MainFrameApp()
{

}


// ----------------------------------------------------------------
/* Menu handlers
 *
 *
 */

void MainFrameApp::
FileOpenHandler(wxCommandEvent &event)
{
  wxFileDialog dialog (this, wxT("Open event file"),
                       wxEmptyString, // default directory
                       wxEmptyString, // default file name
                       wxT("RightTrack files (*.rtrk)|*.rtrk|All files (*)|*"));  // file types
  if (dialog.ShowModal() != wxID_OK)
  {
    return; // cancel pressed
  }

  wxString path = dialog.GetPath();

  // Reset model
  GetModel()->Reset();

  // Tell model to read events from file
  if (GetModel()->ReadFromFile ( path.char_str()) != 0 )
  {
    // display message;
    wxMessageBox( wxT("Error reading event file"),
                  wxT("Error"), wxICON_ERROR | wxOK);
  }



  // update content in event loop
  Refresh();
}


// ----------------------------------------------------------------
/** Get color selection.
 *
 *
 */
void MainFrameApp::
ColorPickerHandler(wxCommandEvent &event)
{
  // Enable all color variants
  GetModel()->m_persistentColourData.SetChooseFull(true);

  // display open file dialog.
  wxColourDialog dialog (this, &GetModel()->m_persistentColourData);
  if (dialog.ShowModal() == wxID_OK)
  {
    wxColourData ret_data = dialog.GetColourData();
    wxColour col = ret_data.GetColour();

    // Store 'col' in the correct place  TBD
  }

}


void MainFrameApp::
ZoomInHandler(wxCommandEvent &event)
{
  // magnify image - more pixels per second
  g_EventWindow->XZoom(2.0);
}


void MainFrameApp::
ZoomOutHandler(wxCommandEvent &event)
{
  // make features smaller - less pix per sec
  g_EventWindow->XZoom(0.5);
}


void MainFrameApp::
ZoomFillHandler(wxCommandEvent &event)
{
  g_EventWindow->XZoom(-1);
}


// ----------------------------------------------------------------
/** Quit the program.
 *
 *
 */
void MainFrameApp::
QuitHandler(wxCommandEvent &event)
{
  Close(true);
}


// ----------------------------------------------------------------
/** Display about box.
 *
 *
 */
void MainFrameApp::
AboutHandler(wxCommandEvent &event)
{
// about bod TBD
}


// ----------------------------------------------------------------
/** Handle model update event
 *
 *
 */
void MainFrameApp::
OnModelUpdate(wxUpdateUIEvent& event)
{
  std::cout << "@@@@ OnModelUpdate()\n";

  UpdateEventInfo();

  // Refresh();
}


// ----------------------------------------------------------------
/** Update Event information group
 *
 *
 */
void MainFrameApp::
UpdateEventInfo()
{
  Model * pm = GetModel();
  wxString val;

  val = wxT("Total events: ");
  val << pm->EventCount();
  this->g_TotalEventCount->SetLabel(val);

  this->g_EventName->SetLabel(pm->m_ei_eventName);

  val = wxT("Count: ");
  val << pm->m_ei_eventCount;
  this->g_EventCount->SetLabel (val);

}

