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
//    EVT_PAINT(                   MainFrameApp::OnPaint)
    EVT_IDLE (   MainFrameApp::OnIdle)
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
/** Idle time processing
 *
 *
 */
void MainFrameApp::
OnIdle(wxIdleEvent& evt)
{
  if (m_pendingUpdate != 0)
  {
    DoModelUpdate(m_pendingUpdate);

    m_pendingUpdate = 0;
  }
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

  g_EventFrame->ResetView();

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
  g_EventFrame->XZoom(1.5);
}


void MainFrameApp::
ZoomOutHandler(wxCommandEvent &event)
{
  // make features smaller - less pix per sec
  g_EventFrame->XZoom(1.0/1.5);
}


void MainFrameApp::
ZoomFillHandler(wxCommandEvent &event)
{
  g_EventFrame->XZoom(-1);
}


void MainFrameApp::
CursorMenuHandler(wxCommandEvent &event)
{
  bool val = event.IsChecked();
  g_EventFrame->EnableCursors(val);
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
// about box TBD
}


// ----------------------------------------------------------------
/** Handle model update event
 *
 *
 */
void MainFrameApp::
ModelUpdate(unsigned code)
{
  m_pendingUpdate |= code;

}

void MainFrameApp::
DoModelUpdate(unsigned code)
{
  if (code & Model::UPDATE_INFO)
  {
    UpdateEventInfo();
    UpdateTimeline();
    UpdateCursorTimes();
  }

  if (code & Model::UPDATE_EVENTS)
  {
    g_EventFrame->DrawNow();
  }
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
  wxString name;
  int count;
  double data;

  pm->GetEventInfo (name, count, data);

  this->g_TotalEventCount->SetLabel(wxString::Format(wxT("Total events: %d"), pm->EventCount()) );
  this->g_EventName->SetLabel(name);

  this->g_EventCount->SetLabel (wxString::Format(wxT("Count: %d"), count ) );
}


// ----------------------------------------------------------------
/** Update timeline
 *
 * This method updates the time scale at the bottom of the event
 * display frame. The start and end time are displayed and a rough
 * scale of the intermeidate time is also attempted.
 */
void MainFrameApp::
UpdateTimeline()
{

  //
  // Get start and end time - fill in the bounds fields
  double start, end;
  GetModel()->GetTimeBounds( start, end);

  this->g_StartTime->SetLabel (wxString::Format(wxT("%1.3f sec"), start) );
  this->g_EndTime->SetLabel   (wxString::Format(wxT("%1.3f sec"), end) );
}


// ----------------------------------------------------------------
/** Update cursor times
 *
 *
 */
void MainFrameApp::
UpdateCursorTimes()
{
  double c1, c2;

  GetModel()->GetCursorTimes(c1, c2);

  this->g_Curs1Time->SetLabel (wxString::Format( wxT("Cursor 1: %1.3f sec"), c1) );
  this->g_Curs2Time->SetLabel (wxString::Format( wxT("Cursor 2: %1.3f sec"), c2) );
  this->g_CursDtime->SetLabel (wxString::Format( wxT("Cursor diff: %1.3f sec"), c2 - c1) );
}

// ----------------------------------------------------------------
/** Draw event names in the correct panel.
 *
 *
 */
void MainFrameApp::
DrawNames (wxDC&dc, int start_idx, int end_idx)
{

  // for each event in their drawing order
  for (int ev_idx = start_idx; ev_idx <= end_idx; ev_idx++)
  {
    // Stop at the last element
    if ((unsigned)ev_idx >= GetModel()->m_drawOrder.size())
    {
      break;
    }

    ItemId_t ev = GetModel()->m_drawOrder[ev_idx];
    int y_coord = (ev_idx + 1) * 25; // in virtual coords

    EventHistory_t * eh = & GetModel()->m_eventMap[ev];

    // draw event name
    // Should render in g_EventNames panel
    wxFont fnt(7, wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL);
    dc.SetFont (fnt);
    dc.DrawText( eh->EventName(), 2, y_coord);

  } // end for
}


