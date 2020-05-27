/*ckwg +5
 * Copyright 2011, 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <MainFrameApp.h>

#include <wx/aboutdlg.h>
#include <wx/colordlg.h>
#include <wx/dcclient.h>
#include <wx/file.h>
#include <wx/filedlg.h>
#include <wx/msgdlg.h>
#include <wx/pen.h>
#include <wx/treectrl.h>

#include "DisplayableIterator.h"
#include "EventSort.h"
#include "EventTableApp.h"
#include "TextEditDialogApp.h"

#include <sstream>

// -----------------------------------------------------------------------
// Define our custom event table
BEGIN_EVENT_TABLE( MainFrameApp, RT_MainFrame )
//    EVT_PAINT(                   MainFrameApp::OnPaint)
EVT_IDLE( MainFrameApp::OnIdle )

// Menu event handlers
EVT_MENU( FileOpen_id, MainFrameApp::FileOpenHandler )
EVT_MENU( FileMerge_id, MainFrameApp::MergeEventsHandler )
EVT_MENU( wxID_SAVE, MainFrameApp::SaveHandler )
EVT_MENU( wxID_SAVEAS, MainFrameApp::SaveAsHandler )
EVT_MENU( wxID_EXIT, MainFrameApp::QuitHandler )
EVT_MENU( wxID_ABOUT, MainFrameApp::AboutHandler )

// Main frame event handlers
EVT_TOOL( ColorPicker_id, MainFrameApp::ColorPickerHandler )
EVT_TOOL( wxID_ZOOM_IN, MainFrameApp::ZoomInHandler )
EVT_TOOL( wxID_ZOOM_OUT, MainFrameApp::ZoomOutHandler )
EVT_TOOL( wxID_ZOOM_FIT, MainFrameApp::ZoomFillHandler )
EVT_MENU( CursorMenu_id, MainFrameApp::CursorMenuHandler )
EVT_MENU( ResetCursor_id, MainFrameApp::ResetCursorHandler )
EVT_MENU( SortEventName_id, MainFrameApp::EventDataHandler )
EVT_MENU( MenuRefresh_id, MainFrameApp::RefreshHandler )
EVT_MENU( DataSetAnnotation_id, MainFrameApp::EditDataSetAnnotationHandler )

EVT_TOOL( move_top_id, MainFrameApp::handle_move_top )
EVT_TOOL( wxID_UP, MainFrameApp::handle_move_up )
EVT_TOOL( wxID_DOWN, MainFrameApp::handle_move_down )
EVT_TOOL( move_bottom_id, MainFrameApp::handle_move_bottom )

EVT_TOOL( filter_events_id, MainFrameApp::handle_filter_events )
EVT_TEXT_ENTER( timeline_endpoint_update_id, MainFrameApp::handle_enter_timeline )

EVT_TEXT_ENTER( set_1_id, MainFrameApp::Curs1Set )
EVT_SPIN_DOWN( spin_1_id, MainFrameApp::Curs1Down )
EVT_SPIN_UP( spin_1_id, MainFrameApp::Curs1Up )
EVT_TEXT_ENTER( set_2_id, MainFrameApp::Curs2Set )
EVT_SPIN_DOWN( spin_2_id, MainFrameApp::Curs2Down )
EVT_SPIN_UP( spin_2_id, MainFrameApp::Curs2Up )

EVT_MENU( SortEventName_id, MainFrameApp::SortEventNameHandler )
EVT_MENU( SortNumOccur_id, MainFrameApp::SortNumOccurHandler )
EVT_MENU( SortAvg_id, MainFrameApp::SortAvgHandler )
EVT_MENU( SortMaxDuration_id, MainFrameApp::SortMaxDurationHandler )
EVT_MENU( SortMinDuration_id, MainFrameApp::SortMinDurationHandler )
EVT_MENU( SortPctAct_id, MainFrameApp::SortPctActHandler )

END_EVENT_TABLE()

// -------------------------------------------------------------------------------
MainFrameApp
::MainFrameApp( wxWindow* parent,
                int id,
                const wxString& title,
                const wxPoint& pos,
                const wxSize& size,
                long style )
  : RT_MainFrame( parent, id, title, pos, size, style )
{
  // Attach ourselves as an observer of the model
  Observe( *GetModel() );
}

// ----------------------------------------------------------------
/** Idle time processing
 *
 *
 */
void
MainFrameApp
::OnIdle( wxIdleEvent& evt )
{
  DoModelUpdate();
}

// ----------------------------------------------------------------
/* Menu handlers
 *
 *
 */

/// Add raw event file into database
void
MainFrameApp
::MergeEventsHandler( wxCommandEvent& event )
{
  wxFileDialog dialog( this, wxT( "Open raw event file" ),
                       wxEmptyString, // default directory
                       wxEmptyString, // default file name
                       wxT( "RightTrack files (*.rtrk)|*.rtrk|All files (*)|*" ) );

  if ( dialog.ShowModal() != wxID_OK )
  {
    return; // cancel pressed
  }

  wxString path = dialog.GetPath();

  try
  {
    // Tell model to read events from file
    GetModel()->ReadFromFile( path.char_str() );
  }
  catch ( std::exception& e )
  {
    std::stringstream msg;
    msg << "Error reading raw event file. " << e.what();
    // display message;
    wxMessageBox( wxString( msg.str() ),
                  wxT( "Error" ), wxICON_ERROR | wxOK );
  }

  InitializeEventTree();
  g_EventFrame->ResetView();
  g_EventFrame->DrawNow();
}

// ----------------------------------------------------------------
/// Load saved database file
void
MainFrameApp
::FileOpenHandler( wxCommandEvent& event )
{
  wxFileDialog dialog( this, wxT( "Open event database file" ),
                       wxEmptyString, // default directory
                       wxEmptyString, // default file name
                       wxT( "RightTrack files (*.rtmdl)|*.rtmdk" ) );

  if ( dialog.ShowModal() != wxID_OK )
  {
    return; // cancel pressed
  }

  wxString path = dialog.GetPath();

  // Reset model
  GetModel()->Reset();

  try
  {
    // Tell model to read events from file
    GetModel()->LoadDataBaseFile( path.char_str() );
  }
  catch ( std::exception& e )
  {
    std::stringstream msg;
    msg << "Error reading session file." << e.what();
    // display message;
    wxMessageBox( wxString( msg.str() ),
                  wxT( "Error" ), wxICON_ERROR | wxOK );
  }

  InitializeEventTree();
  g_EventFrame->ResetView();
  g_EventFrame->DrawNow();
}

// ----------------------------------------------------------------
void
MainFrameApp
::SaveHandler( wxCommandEvent& event )
{
  // if no file name set in model, call SaveAsHandler
  if ( GetModel()->m_modelFileName.empty() )
  {
    FileSaveAs();
    return;
  }

  FileSave();
}

// ------------------------------------------------------------------
// Save to current file name.
//
// @returns true - if file saved; false - if save aborted
//
bool
MainFrameApp
::FileSave()
{
  try
  {
    GetModel()->SaveToFile();
    return true;
  }
  catch ( std::exception& e )
  {
    std::stringstream msg;
    msg << "Error writing session to file." <<
      e.what();
    // display message;
    wxMessageBox( wxString( msg.str() ),
                  wxT( "Error" ), wxICON_ERROR | wxOK );
  }

  return false;
}

// ----------------------------------------------------------------
// Saves file to user suplied name.
//
// @returns true - if file saved; false - if save aborted
//
void
MainFrameApp
::SaveAsHandler( wxCommandEvent& event )
{
  FileSaveAs();
}

// ----------------------------------------------------------------
bool
MainFrameApp
::FileSaveAs()
{
  // Get file name from dialog
  wxFileDialog dialog( this, wxT( "Save event database file" ),
                       wxEmptyString, // default directory
                       wxEmptyString, // default file name
                       wxT( "RightTrack files (*.rtmdl)|*.rtmdl" ),
                       wxFD_SAVE | wxFD_OVERWRITE_PROMPT );  // file types
  if ( dialog.ShowModal() != wxID_OK )
  {
    return false; // cancel pressed
  }

  try
  {
    wxString path = dialog.GetPath();
    GetModel()->SaveAsToFile( path );
    return true;
  }
  catch ( std::exception& e )
  {
    std::stringstream msg;
    msg << "Error writing session to file." <<
      e.what();
    // display message;
    wxMessageBox( wxString( msg.str() ),
                  wxT( "Error" ), wxICON_ERROR | wxOK );
  }

  return false;
}

// ----------------------------------------------------------------
/** Get color selection.
 *
 *
 */
void
MainFrameApp
::ColorPickerHandler( wxCommandEvent& event )
{
  // Enable all color variants
  GetModel()->m_persistentColourData.SetChooseFull( true );

  // display open file dialog.
  wxColourDialog dialog( this, &GetModel()->m_persistentColourData );
  if ( dialog.ShowModal() == wxID_OK )
  {
    wxColourData ret_data = dialog.GetColourData();
    wxColour col = ret_data.GetColour();

    // Store 'col' in the correct place  TBD
  }
}

// ----------------------------------------------------------------
void
MainFrameApp
::ZoomInHandler( wxCommandEvent& event )
{
  // magnify image - more pixels per second
  g_EventFrame->XZoom( 1.5 );
}

// ----------------------------------------------------------------
void
MainFrameApp
::ZoomOutHandler( wxCommandEvent& event )
{
  // make features smaller - less pix per sec
  g_EventFrame->XZoom( 1.0 / 1.5 );
}

// ----------------------------------------------------------------
void
MainFrameApp
::ZoomFillHandler( wxCommandEvent& event )
{
  g_EventFrame->XZoom( -1 );
}

// ----------------------------------------------------------------
void
MainFrameApp
::CursorMenuHandler( wxCommandEvent& event )
{
  bool val = event.IsChecked();
  g_EventFrame->EnableCursors( val );
}

// ----------------------------------------------------------------
void
MainFrameApp
::ResetCursorHandler( wxCommandEvent& event )
{
  g_EventFrame->ResetCursors();
}

// ----------------------------------------------------------------
/** Refresh view.
 *
 *
 */
void
MainFrameApp
::RefreshHandler( wxCommandEvent& event )
{
  this->Refresh();
}

// ----------------------------------------------------------------
/** Generate event summary data.
 *
 * scan through all active events and collect event summary info
 */
void
MainFrameApp
::EventDataHandler( wxCommandEvent& event )
{
  wxString result;

  // Display header line
  result << wxT(
    "Event-name, num-occur, min-duration, max-duration, avg-dur, std-dev, active-percent\n" );

  DisplayableIterator event_it;

  EventTableApp* event_table = new EventTableApp( this, -1, wxT( "Event Table" ) );
  event_table->InitGrid( event_it );
  event_table->Show();
}

// ----------------------------------------------------------------
/** Edit data set annotation.
 *
 *
 */
void
MainFrameApp
::EditDataSetAnnotationHandler( wxCommandEvent& event )
{
  TextEditDialogApp dialog( this, -1, wxT( "Data Set Annotation" ) );
  dialog.SetText( GetModel()->DataSetAnnotation() );
  if ( dialog.ShowModal() == wxID_OK )
  {
    GetModel()->DataSetAnnotation() = dialog.GetText();
  }
}

// ----------------------------------------------------------------
/** Quit the program.
 *
 *
 */
void
MainFrameApp
::QuitHandler( wxCommandEvent& event )
{
  if ( GetModel()->ModelNeedsSave() )
  {
    const bool active_file_name = !GetModel()->m_modelFileName.empty();

    // display dialog asking to save or quit without saving or cancel
    // Don't Save  -  Cancel  - Save
    std::stringstream msg;
    msg << "Do you want to save the changes you made in the document \"" <<
    ( active_file_name ? GetModel()->m_modelFileName : "Untitled" ) <<
      "\"?";

    // cancel -> Cancel
    // yes -> Save
    // no -> Don't save

    wxMessageDialog dialog( this, wxString( msg.str().c_str() ),
                            "Your changes will be lost if you don't save them.", //
                                                                                 // caption
                            ( wxYES_NO | wxCANCEL | wxICON_EXCLAMATION ) );

    //                            Yes        NO        Cancel
    dialog.SetYesNoCancelLabels( "Save", "Don't Save", "Cancel" );

    auto result = dialog.ShowModal();
    if ( result == wxID_CANCEL )
    {
      // Cancelled the quit operation
      return;
    }

    if ( result == wxID_YES )
    {
      // Activate the save/save-as code
      if ( active_file_name )
      {
        FileSave(); // use existing name
      }
      else
      {
        FileSaveAs();
      }
    }
  } // end model needs save

  Close( true );
}

// ----------------------------------------------------------------
/** Display about box.
 *
 *
 */
void
MainFrameApp
::AboutHandler( wxCommandEvent& event )
{
  // This object will contain all information displayed in About box
  wxAboutDialogInfo info;

  // Call of AddDeveloper() method adds a record to list of developers
  info.AddDeveloper( wxT( "Linus Sherrill" ) );

  // Call of AddDocWriter() method adds a record to list of documentation
  // writers
  info.AddDocWriter( wxT( "Donald Duck" ) );

  // Call of AddArtist() method adds a record to list of artists
  // info.AddArtist( wxT("Scrooge Mc.Duck") );

  // Call of AddTranslator() method adds a record to list of translators
  // info.AddTranslator( wxT("Mickey Mouse") );

  // This method adds application description.
  info.SetDescription( wxT( "Right Track event viewer.\n"
                            "Displays and enables analysis of Right Track event files." ) );

  // This method sets application version string
  info.SetVersion( wxT( RIGHT_TRACK_VERSION ) );

  // SetName() method sets application name displayed in About box.
  // It is better to pass wxApp::GetAppName() to this method
  info.SetName( wxTheApp->GetAppName() );

  // Sets application Web-site URL
  info.SetWebSite( wxT( "http://www.kitware.com" ) );

  // Sets the icon which will be displayed in About box.
// +  info.SetIcon( wxICON(wxICON_AAA) );

  // Sets application license string. Only wxGTK port has native way of
  // displaying application license. All other ports will use generic way for
  // this purpose.
  info.SetLicence( wxT(
                     "Copyright 2011-2020 by Kitware, Inc. All Rights Reserved. Please refer to\n"
                     "KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,\n"
                     "Kitware, Inc., 1712 Route 9, Clifton Park, NY 12065." ) );

  info.SetCopyright( wxT( "(C) 2011-2020 Kitware Inc." ) );
  // At last, we can display about box
  wxAboutBox( info );
}

// ----------------------------------------------------------------
/** Handle model update event
 *
 *
 */

// Called from subject Notify()
int
MainFrameApp
::Update( Subject& subj, Subject::NotifyType_t type )
{
  // Add code to set to be consumed by DoModelUpdate
  m_pendingUpdates.insert( type );
}

// This is called from the idle loop to apply all pending updates.
void
MainFrameApp
::DoModelUpdate()
{
  // Order of checking is important!

  if ( m_pendingUpdates.count( Model::UPDATE_cursor_info ) > 0 )
  {
    // Update cursor objects with model data
    UpdateCursorTimes(); // update time display areas
    g_EventFrame->ResetCursorToModel(); // move lines to new location

    // This is a cheap way of getting the frame updated.
    m_pendingUpdates.insert( Model::UPDATE_event_frame );
  }

  if ( m_pendingUpdates.count( Model::UPDATE_event_info ) > 0 )
  {
    UpdateEventInfo();
  }

  if ( m_pendingUpdates.count( Model::UPDATE_time_line ) > 0 )
  {
    UpdateTimeline();
  }

  if ( m_pendingUpdates.count( Model::UPDATE_event_frame ) > 0 )
  {
    g_EventFrame->DrawNow(); // update whole frame
  }

  // erase all updates
  m_pendingUpdates.clear();
}

// ----------------------------------------------------------------
/** Update Event information group sidebar
 *
 *
 */
void
MainFrameApp
::UpdateEventInfo()
{
  Model* pm = GetModel();

  this->g_TotalEventCount->SetLabel( wxString::Format( wxT( "Total events: %d" ),
                                                       pm->EventCount() ) );

  wxString result;
  wxString temp;
  wxString comments;

  // test for selected event
  ItemId_t item = pm->GetSelectedEvent();
  if ( item >= 0 )
  {
    EventDef::handle_t eh = pm->m_eventMap[ item ];
    result = eh->GetEventInfo();
    temp = eh->GetUserComment();
    if ( !temp.empty() ) // collect comment if any
    {
      comments += wxT( "Event:\n" ) + temp;
    }
  }

  // test for selected occurrence
  if ( pm->GetSelectedOccurrence() != 0 )
  {
    result += pm->GetSelectedOccurrence()->GetInfo();

    if ( !comments.empty() )
    {
      comments += wxT( "\n\n" );
    }

    temp = pm->GetSelectedOccurrence()->GetUserComment();

    if ( !temp.empty() ) // collect comment if any
    {
      comments += wxT( "Occurrence:\n" ) + temp;
    }
  }

  this->g_EventInfo->SetValue( result );
  this->g_CommentText->SetValue( comments );
}

// ----------------------------------------------------------------
/** Update timeline
 *
 * This method updates the time scale at the bottom of the event
 * display frame. The start and end time are displayed and a rough
 * scale of the intermeidate time is also attempted.
 */
void
MainFrameApp
::UpdateTimeline()
{
  // Get start and end time - fill in the bounds fields
  double start, end;
  GetModel()->GetTimeBounds( start, end );

  this->g_StartTime->SetValue( wxString::Format( wxT( "%1.3f sec" ), start ) );
  this->g_EndTime->SetValue( wxString::Format( wxT( "%1.3f sec" ), end ) );
}

// ----------------------------------------------------------------
/** Update cursor times
 *
 *
 */
void
MainFrameApp
::UpdateCursorTimes()
{
  double c1, c2;

  GetModel()->GetCursorTimes( c1, c2 );

  this->g_Curs1Time->SetValue( wxString::Format( wxT( "%1.3f" ), c1 ) );
  this->g_Curs2Time->SetValue( wxString::Format( wxT( "%1.3f" ), c2 ) );
  this->g_CursDtime->SetLabel( wxString::Format( wxT( "Cursor diff: %1.3f sec" ), c2 - c1 ) );
}

void
MainFrameApp
::Curs1Set( wxCommandEvent& event )
{
  // take value from control and send to model
  // update cursor position
  wxString val = this->g_Curs1Time->GetValue();
  double c1, c2;
  Model* pm = GetModel();

  pm->GetCursorTimes( c1, c2 );
  val.ToDouble( &c1 );
  pm->SetCursorTimes( c1, c2 );
}

void
MainFrameApp
::Curs1Down( wxSpinEvent& event )
{
  // decrement value in model
  // update model
  double c1, c2;
  double b1, b2;
  Model* pm = GetModel();

  pm->GetCursorTimes( c1, c2 );
  pm->GetTimeBounds( b1, b2 );
  c1 -= ( b2 - b1 ) / 100.0;
  pm->SetCursorTimes( c1, c2 );
}

void
MainFrameApp
::Curs1Up( wxSpinEvent& event )
{
  // decrement value in model
  // update model
  double c1, c2;
  double b1, b2;
  Model* pm = GetModel();

  pm->GetCursorTimes( c1, c2 );
  pm->GetTimeBounds( b1, b2 );
  c1 += ( b2 - b1 ) / 100.0;

  pm->SetCursorTimes( c1, c2 );
}

void
MainFrameApp
::Curs2Set( wxCommandEvent& event )
{
  // take value from control and send to model
  // update cursor position
  wxString val = this->g_Curs2Time->GetValue();
  double c1, c2;
  Model* pm = GetModel();

  pm->GetCursorTimes( c1, c2 );
  val.ToDouble( &c2 );
  pm->SetCursorTimes( c1, c2 );
}

void
MainFrameApp
::Curs2Down( wxSpinEvent& event )
{
  // decrement value in model
  // update model
  double c1, c2;
  double b1, b2;
  Model* pm = GetModel();

  pm->GetCursorTimes( c1, c2 );
  pm->GetTimeBounds( b1, b2 );
  c2 -= ( b2 - b1 ) / 100.0;
  pm->SetCursorTimes( c1, c2 );
}

void
MainFrameApp
::Curs2Up( wxSpinEvent& event )
{
  // decrement value in model
  // update model
  double c1, c2;
  double b1, b2;
  Model* pm = GetModel();

  pm->GetCursorTimes( c1, c2 );
  pm->GetTimeBounds( b1, b2 );
  c2 += ( b2 - b1 ) / 100.0;
  pm->SetCursorTimes( c1, c2 );
}

// ----------------------------------------------------------------
/** Draw event names in the correct panel.
 *
 * This should only ned to be done once for a specific model.
 * Updated if events are added.
 */
void
MainFrameApp
::InitializeEventTree()
{
  g_EventList->DeleteAllItems();

  wxTreeItemId root_id = g_EventList->AddRoot( wxT( "Events" ) );

  //
  // The goal for this tree ctrl is to allow drag to reorder tracks, and
  // the ability to make groups (or display groups as defined in the
  // input data). Also the ability to hide events that are of no
  // interest.
  //
  // This tree view would be the main control for determining draw order and
  // visibility. So this control will need to be persistent WRT the VIEW.
  //

  // loop over draw order and add event name to the tree
  DisplayableIterator dit;
  wxTreeItemId node = root_id;

  do
  {
    // If not valid, we have gone off the end of the list
    if ( !dit.IsCurrentValid() ) { break; }

    auto the_event = dit.CurrentEvent();
    auto event_name = the_event->EventName();
    AppTreeData* node_data = new AppTreeData; //+ not sure who manages this storage
    node_data->m_itemId = the_event->GetEventId();

    // Append name to current node
    g_EventList->AppendItem( node, event_name, -1, -1, node_data );
  }  while ( dit.Next() );
}

// ----------------------------------------------------------------
/** Move currently selected event up one line.
 *
 *
 */
void
MainFrameApp
::handle_move_top( wxCommandEvent& event )
{
  GetModel()->MoveSelectedEventTop();
}

void
MainFrameApp
::handle_move_up( wxCommandEvent& event )
{
  GetModel()->MoveSelectedEventUp();
}

void
MainFrameApp
::handle_move_down( wxCommandEvent& event )
{
  GetModel()->MoveSelectedEventDown();
}

void
MainFrameApp
::handle_move_bottom( wxCommandEvent& event )
{
  GetModel()->MoveSelectedEventBottom();
}

// ----------------------------------------------------------------
/** Handle events filter tool bar icon.
 *
 *
 */
void
MainFrameApp
::handle_filter_events( wxCommandEvent& event )
{
  bool val = event.IsChecked();

  GetModel()->SetEventFilter( val );
}

void
MainFrameApp
::handle_enter_timeline( wxCommandEvent& event )
{
}

// ----------------------------------------------------------------
/** Handle sort events
 *
 *
 */
void
MainFrameApp
::SortEventNameHandler( wxCommandEvent& event )
{
  GetModel()->SortEvents< SortByName >();
}

void
MainFrameApp
::SortNumOccurHandler( wxCommandEvent& event )
{
  GetModel()->SortEvents< SortByCount >();
}

void
MainFrameApp
::SortAvgHandler( wxCommandEvent& event )
{
  GetModel()->SortEvents< SortByAverageTime >();
}

void
MainFrameApp
::SortMaxDurationHandler( wxCommandEvent& event )
{
  GetModel()->SortEvents< SortByMaxDuration >();
}

void
MainFrameApp
::SortMinDurationHandler( wxCommandEvent& event )
{
  GetModel()->SortEvents< SortByMinDuration >();
}

void
MainFrameApp
::SortPctActHandler( wxCommandEvent& event )
{
  GetModel()->SortEvents< SortByPercentage >();
}
