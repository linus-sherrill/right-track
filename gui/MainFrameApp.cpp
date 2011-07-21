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
#include <wx/treectrl.h>


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
  m_pendingUpdate = 0;
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


void MainFrameApp::
ResetCursorHandler(wxCommandEvent &event)
{
  g_EventFrame->ResetCursors();
}


// ----------------------------------------------------------------
/** Refresh view.
 *
 *
 */
void MainFrameApp::
RefreshHandler(wxCommandEvent &event)
{
  this->Refresh();
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
  }

  if (code & Model::UPDATE_EVENTS)
  {
    g_EventFrame->DrawNow();
  }

  if (code & Model::UPDATE_CURSOR)
  {
    // Update cursor objects with model data
    UpdateCursorTimes(); // update time display areas
    g_EventFrame->ResetCursorToModel(); // move lines to new location
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

  this->g_TotalEventCount->SetLabel(wxString::Format(wxT("Total events: %d"), pm->EventCount()) );

  wxString result;

  ItemId_t item = pm->GetSelectedEvent();
  if (item >= 0)
  {
    EventDef::handle_t eh = pm->m_eventMap [ item ];
    this->g_EventInfo->SetValue (eh->GetEventInfo());
  }
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

  this->g_StartTime->SetValue (wxString::Format(wxT("%1.3f sec"), start) );
  this->g_EndTime->SetValue   (wxString::Format(wxT("%1.3f sec"), end) );
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

  this->g_Curs1Time->SetValue (wxString::Format( wxT("%1.3f"), c1) );
  this->g_Curs2Time->SetValue (wxString::Format( wxT("%1.3f"), c2) );
  this->g_CursDtime->SetLabel (wxString::Format( wxT("Cursor diff: %1.3f sec"), c2 - c1) );
}


void MainFrameApp::Curs1Set(wxCommandEvent &event)
{
  // take value from control and send to model
  // update cursor position
  wxString val = this->g_Curs1Time->GetValue ();
  double c1, c2;
  Model * pm = GetModel();

  pm->GetCursorTimes(c1, c2);
  val.ToDouble(&c1);
  pm->SetCursorTimes(c1, c2);
}


void MainFrameApp::Curs1Down(wxSpinEvent &event)
{
  // decrement value in model
  // update model
  double c1, c2;
  double b1, b2;
  Model * pm = GetModel();

  pm->GetCursorTimes(c1, c2);
  pm->GetTimeBounds( b1, b2);
  c1 -= (b2 - b1) / 100.0;
  pm->SetCursorTimes(c1, c2);
}


void MainFrameApp::Curs1Up(wxSpinEvent &event)
{
  // decrement value in model
  // update model
  double c1, c2;
  double b1, b2;
  Model * pm = GetModel();

  pm->GetCursorTimes(c1, c2);
  pm->GetTimeBounds( b1, b2);
  c1 += (b2 - b1) / 100.0;

  pm->SetCursorTimes(c1, c2);
}


void MainFrameApp::Curs2Set(wxCommandEvent &event)
{
  // take value from control and send to model
  // update cursor position
  wxString val = this->g_Curs2Time->GetValue ();
  double c1, c2;
  Model * pm = GetModel();

  pm->GetCursorTimes(c1, c2);
  val.ToDouble(&c2);
  pm->SetCursorTimes(c1, c2);
}


void MainFrameApp::Curs2Down(wxSpinEvent &event)
{
  // decrement value in model
  // update model
  double c1, c2;
  double b1, b2;
  Model * pm = GetModel();

  pm->GetCursorTimes(c1, c2);
  pm->GetTimeBounds( b1, b2);
  c2 -= (b2 - b1) / 100.0;
  pm->SetCursorTimes(c1, c2);
}


void MainFrameApp::Curs2Up(wxSpinEvent &event)
{
  // decrement value in model
  // update model
  double c1, c2;
  double b1, b2;
  Model * pm = GetModel();

  pm->GetCursorTimes(c1, c2);
  pm->GetTimeBounds( b1, b2);
  c2 += (b2 - b1) / 100.0;
  pm->SetCursorTimes(c1, c2);
}


// ----------------------------------------------------------------
/** Draw event names in the correct panel.
 *
 *
 */
void MainFrameApp::
DrawNames ()
{
  class AppTreeData
    : public wxTreeItemData
  {


  };


  wxTreeItemId root_id = g_EventList->AddRoot(wxT("Events"), 0, 0,
                                              new AppTreeData() );
  // Store item id into EventHistory object

//
// The goal for this tree ctrl is to allow drag to reorder tracks, and
// the ability to make groups (or display groups as defined in the
// input data). Also the ability to hide events that are of no
// interest.
//

/*
  Model::event_iterator_t ix;
  for (ix = m_eventMap.begin(); ix != m_eventMap.end(); ix++)
  {
    EventHistory_t * eh = &(ix->second);



  } // end for
*/
/*
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
*/

}




// ----------------------------------------------------------------
/** Move currently selected event up one line.
 *
 *
 */
void MainFrameApp::
handle_move_up(wxCommandEvent &event)
{
  GetModel()->MoveSelectedEventUp();
}


void MainFrameApp::
handle_move_down(wxCommandEvent &event)
{
  GetModel()->MoveSelectedEventDown();
}
