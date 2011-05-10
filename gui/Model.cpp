/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <Model.h>

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
Model()
  : m_timingOffset(~0),
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
  m_timingOffset = ~0;

  m_drawOrder.clear();
  m_eventMap.clear();

  // These default colors are arbitrary
  m_defaultBaselineColor = wxColor ( 120, 120, 120 );
  m_defaultLineColor = wxColor ( 128, 128, 128 ); // gray
  m_defaultEventColor = wxColor ( 35, 19, 238 );

  m_startEventColor = wxColor ( 10, 167, 6 ); // green
  m_endEventColor = wxColor ( 229, 22, 22 ); // red

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

  Refresh();
}


// ----------------------------------------------------------------
/** Get timespan of all events.
 *
 * This method returns the length of time covered from the first event
 * to the last event.
 */
EventTimestamp_t Model::
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
        EventTimestamp_t ts = it->event_time;

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
/** Send message to windows when something changed.
 *
 *
 */
void Model::
Refresh()
{
  wxTheApp->GetTopWindow()->Refresh();
}
