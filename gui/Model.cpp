/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <Model.h>

#include <MainFrameApp.h>

#include <wx/wx.h>
#include <wx/app.h>

#include <EventTransportFile.h>
#include <EventTransportReaderGui.h>
#include <EventTransportReaderDebug.h>


Model * Model::s_instance(0);


// ----------------------------------------------------------------
/** Constructor.
 *
 *
 */
Model::
Model(MainFrameApp * frame)
  : m_parentFrame(frame),
    m_timingOffset(1e300),
    m_maxTime(0)
{

  s_instance = this;
  Reset();
}


Model * Model::
Instance()
{
  return (s_instance);
}


// ----------------------------------------------------------------
/** Reset model.
 *
 *
 */
void Model::
Reset()
{
  m_timingOffset = 1e300;

  m_drawOrder.clear();
  m_eventMap.clear();

  // These default colors are arbitrary
  m_defaultBaselineColor = wxColor ( 120, 120, 120 );
  m_defaultLineColor = wxColor ( 128, 128, 128 ); // gray
  m_defaultEventColor = wxColor ( 35, 19, 238 );

  m_startEventColor = wxColor ( 10, 167, 6 ); // green
  m_endEventColor = wxColor ( 229, 22, 22 ); // red

  m_selectColor = wxColor (222, 207, 15);

  m_selectedEvent = -1;
}


// ----------------------------------------------------------------
/** Read events from file.
 *
 *
 */
int Model::
ReadFromFile( const char * file )
{
  // instantiate our reader
  // EventTransportReaderDebug reader;
  EventTransportReaderGui reader (this);
  EventTransportFile file_io;
  int status(0);

  status = file_io.ReadEvents (file, reader);
  if (status != 0)
  {
    return status;
  }

  ScanEvents();

  // Reset scaling of views.  wxID_RESET

  ModelUpdate(UPDATE_INFO);

  return (status);
}


// ----------------------------------------------------------------
/** Get timespan of all events.
 *
 * This method returns the length of time covered from the first event
 * to the last event.
 */
double Model::
EventTimeRange() const
{
  return  (m_maxTime - m_timingOffset);
}


// ----------------------------------------------------------------
/** Scan events
 *
 * Scan the list of events and setup display objects.
 *
 * This is the place to mess with the colouring of the event displays.
 */
void Model::
ScanEvents()
{
  event_iterator_t ix;
  for (ix = m_eventMap.begin(); ix != m_eventMap.end(); ix++)
  {
    EventHistory_t * eh = &(ix->second);

    eh->m_enabled = true; // all start enabled

    // Setup general colors
    eh->eventBaselinePen = wxPen ( m_defaultBaselineColor, 1, wxSOLID );

    eh->startMarkerPen = wxPen ( m_startEventColor, 1, wxSOLID );
    eh->startMarkerBrush = wxBrush ( m_startEventColor, wxSOLID );

    eh->endMarkerPen = wxPen ( m_endEventColor, 1, wxSOLID );
    eh->endMarkerBrush = wxBrush ( m_endEventColor, wxSOLID );

    // Check for event specific colours
    if (eh->ev_def.event_color == -1)
    {
      // Use default colors
      eh->eventDurationPen = wxPen ( m_defaultEventColor, 2, wxSOLID );
      eh->eventMarkerPen   = wxPen ( m_defaultEventColor, 1, wxSOLID );
    }
    else
    {
      int r = (eh->ev_def.event_color >> 24) & 0xff;
      int g = (eh->ev_def.event_color >> 16) & 0xff;
      int b = (eh->ev_def.event_color >> 00) & 0xff;

      wxColour def_color ( r, g, b );

      eh->eventDurationPen = wxPen ( def_color, 1, wxSOLID );
      eh->eventMarkerPen   = wxPen ( def_color, 1, wxSOLID );
    }

    //
    // Scan event list to determine temporal bounds
    //
      Model::time_iterator_t it = eh->EventHistory.begin();
      Model::time_iterator_t eit = eh->EventHistory.end();

      for ( ; it != eit; it++)
      {
        double ts = it->event_time;

        if (ts < m_timingOffset)
        {
          m_timingOffset = ts;
        }

        if (ts > m_maxTime)
        {
          m_maxTime = ts;
        }
      } // end for it

  } // end for

}


// ----------------------------------------------------------------
/** Get/set cursor times
 *
 *
 */
void Model::
SetCursorTimes (double t1, double t2)
{
  m_cursor_1_time = t1;
  m_cursor_2_time = t2;

  ModelUpdate(UPDATE_CURSOR);
}


void Model::
GetCursorTimes (double& t1, double& t2)
{
  t1 = m_cursor_1_time;
  t2 = m_cursor_2_time;
}


void Model::
SetTimeBounds (double start, double end)
{
  m_viewTimeStart = start;
  m_viewTimeEnd = end;

  ModelUpdate(UPDATE_INFO);
}

void Model::
GetTimeBounds (double& start, double& end)
{
  start = m_viewTimeStart;
  end = m_viewTimeEnd;
}


void Model::
SetEventInfo ( wxString const& name, int count, double data)
{
  m_evc_name = name;
  m_evc_count = count;
  m_evc_data = data;

  ModelUpdate(UPDATE_INFO);
}

void Model::
GetEventInfo ( wxString& name, int& count, double& data)
{
  name = m_evc_name;
  count= m_evc_count;
  data = m_evc_data;
}


void Model::
SelectEvent (ItemId_t event)
{
  m_selectedEvent = event;

  // Need to redraw events
  ModelUpdate(UPDATE_EVENTS);
}


bool Model::
IsEventSelected (ItemId_t event) const
{
  return (m_selectedEvent == event);
}


ItemId_t Model::
GetSelectedEvent() const
{
  return m_selectedEvent;
}


// ----------------------------------------------------------------
/** Send message to windows when something changed.
 *
 *
 */
void Model::
ModelUpdate(unsigned code)
{
  m_parentFrame->ModelUpdate(code);
}


// ----------------------------------------------------------------
/** Move currently selected event up one row.
 *
 *
 */
void Model::
MoveSelectedEventUp()
{
  ItemId_t item = GetSelectedEvent();
  if (item < 0)
  {
    return; // no selected event
  }

  // Scan the drawing order vector

  // Start loop at 1 since if the desired element is at index 0, it
  // can not be moved higher, so don't even try.
  size_t limit = m_drawOrder.size();
  for (size_t i = 1; i < limit; ++i)
  {
    if (m_drawOrder[i] == item)
    {
      // swap [i] and [i-1]
      ItemId_t temp = m_drawOrder[i];
      m_drawOrder[i] = m_drawOrder[i-1];
      m_drawOrder[i-1] = temp;

      // Need to redraw events
      ModelUpdate(UPDATE_EVENTS);

      break;
    }
  } // end for
}


// ----------------------------------------------------------------
/** Move currently selected event down one row.
 *
 *
 */
void Model::
MoveSelectedEventDown()
{
  ItemId_t item = GetSelectedEvent();
  if (item < 0)
  {
    return; // no selected event
  }

  // Scan the drawing order vector

  // End loop at next to the last element, since if the one we are
  // looking for is there, it can not be moved.
  size_t limit = m_drawOrder.size() -1;
  for (size_t i = 1; i < limit; ++i)
  {
    if (m_drawOrder[i] == item)
    {
      // swap [i] and [i+1]
      ItemId_t temp = m_drawOrder[i];
      m_drawOrder[i] = m_drawOrder[i+1];
      m_drawOrder[i+1] = temp;

      // Need to redraw events
      ModelUpdate(UPDATE_EVENTS);

      break;
    }
  } // end for
}
