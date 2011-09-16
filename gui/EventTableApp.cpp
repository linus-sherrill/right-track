/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include "EventTableApp.h"

#include <wx/filedlg.h>
#include <wx/file.h>



// ----------------------------------------------------------------
/** Constructor.
 *
 *
 */
EventTableApp::
EventTableApp(wxWindow* parent, int id, const wxString& title,
              const wxPoint& pos,
              const wxSize& size, long style)
  : EventTable (parent, id, title, pos, size, style)
{

}


EventTableApp::
~EventTableApp()
{

}


// ----------------------------------------------------------------
/** Initialize the grid
 *
 *
 */

void EventTableApp::
InitGrid(DisplayableIterator & event_it)
{
  m_dataSource = event_it;

  int row_count(0);
  m_dataSource.Start();

  // count number of rows
  while ( m_dataSource.IsCurrentValid())
  {
    row_count++;
    m_dataSource.Next();
  } // end while

  // create grid of sufficient size
  this->data_grid->CreateGrid( row_count + 1, 7);
  this->data_grid->SetColSize(0, 200); // event name column

  // for each event in their drawing order
  m_dataSource.Start();
  row_count = 0;

  /// @todo make title bold
  this->data_grid->SetCellValue (row_count, 0, wxT("Name") );
  this->data_grid->SetCellValue (row_count, 1, wxT("Count") );
  this->data_grid->SetCellValue (row_count, 2, wxT("Minimum") );
  this->data_grid->SetCellValue (row_count, 3, wxT("Maximum") );
  this->data_grid->SetCellValue (row_count, 4, wxT("Average") );
  this->data_grid->SetCellValue (row_count, 5, wxT("Std Dev") );
  this->data_grid->SetCellValue (row_count, 6, wxT("Percent active") );

  row_count++;

  while ( m_dataSource.IsCurrentValid())
  {
    EventDef::handle_t eh = m_dataSource.CurrentEvent();

    switch (eh->EventType())
    {
    case Event::ET_BOUNDED_EVENT:
    {
      BoundedEventDef * ev = m_dataSource.CurrentEvent()->GetBoundedEvent();
      BoundedEventStatistics stats = ev->m_stats;

      // format stats to a string
      this->data_grid->SetCellValue (row_count, 0, wxString::Format(wxT("%s"),  ev->EventName().c_str()) );
      this->data_grid->SetCellValue (row_count, 1, wxString::Format(wxT("%d"),  stats.m_count) );
      this->data_grid->SetCellValue (row_count, 2, wxString::Format(wxT("%f"),  stats.m_minDuration) );
      this->data_grid->SetCellValue (row_count, 3, wxString::Format(wxT("%f"),  stats.m_maxDuration) );
      this->data_grid->SetCellValue (row_count, 4, wxString::Format(wxT("%f"),  stats.m_avgDuration) );
      this->data_grid->SetCellValue (row_count, 5, wxString::Format(wxT("%f"),  stats.m_stdDuration) );
      this->data_grid->SetCellValue (row_count, 6, wxString::Format(wxT("%f"),  stats.m_activePct) );
    } // end case
    break;

    case Event::ET_DISCRETE_EVENT:
    {
      DiscreteEventDef * ev = m_dataSource.CurrentEvent()->GetDiscreteEvent();
      DiscreteEventStatistics stats = ev->m_stats;

      this->data_grid->SetCellValue (row_count, 0, wxString::Format(wxT("%s"),  ev->EventName().c_str()) );
      this->data_grid->SetCellValue (row_count, 1, wxString::Format(wxT("%d"),  ev->NumOccurrences()) );

    } // end case
    break;

    } // end switch

    row_count++;
    m_dataSource.Next();
  } // end while

}


// ----------------------------------------------------------------
/** Event handlers
 *
 *
 */

void EventTableApp::
handle_done(wxCommandEvent &event)
{
  std::cout << "Done button\n";
  // terminate and close window
  Close(true);
}


// ----------------------------------------------------------------
/** Save grid to a file.
 *
 *
 */
void EventTableApp::
handle_save(wxCommandEvent &event)
{
  wxString result;

  m_dataSource.Start();

  // Open an output file
  wxFileDialog dialog (this, wxT("Save event data file"),
                       wxEmptyString, // default directory
                       wxEmptyString, // default file name
                       wxT("Text files (*.txt)|*.txt|All files (*)|*"),  // file types
                       (wxSAVE | wxOVERWRITE_PROMPT)
    );
  if (dialog.ShowModal() != wxID_OK)
  {
    return; // cancel pressed
  }

  /// @todo use file type to determine formatting options.
  // text, csv, others?

  // Display header line
  result << wxT("Event-name, num-occur, min-duration, max-duration, avg-dur, std-dev, active-percent\n");

  // for each event in their drawing order
  while ( m_dataSource.IsCurrentValid())
  {
    BoundedEventDef * ev = m_dataSource.CurrentEvent()->GetBoundedEvent();
    if (ev)
    {
      BoundedEventStatistics stats = ev->m_stats;

      // format stats to a string
      result << wxString::Format(wxT("%s, "),  ev->EventName().c_str());
      result << wxString::Format(wxT("%d, "),  stats.m_count);
      result << wxString::Format(wxT("%f, "),  stats.m_minDuration);
      result << wxString::Format(wxT("%f, "),  stats.m_maxDuration);
      result << wxString::Format(wxT("%f, "),  stats.m_avgDuration);
      result << wxString::Format(wxT("%f, "),  stats.m_stdDuration);
      result << wxString::Format(wxT("%f\n"),  stats.m_activePct);
    } // end if

    m_dataSource.Next();
  } // end while

  wxString path = dialog.GetPath();
  wxFile file (path, wxFile::write);
  if ( ! file.IsOpened() )
  {
    return;
  }

  file.Write(result);
}


// ----------------------------------------------------------------
/**
 *
 *
 */
void EventTableApp::
handle_cancel(wxCommandEvent &event)
{
  Close(true);
}


