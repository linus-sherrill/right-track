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
    m_maxTime(0),
    m_eventFilter(0)
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

  // ModelUpdate(UPDATE_INFO);

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
  wxPen start_marker_pen = wxPen ( m_startEventColor, 1, wxSOLID );
  wxBrush start_marker_brush = wxBrush ( m_startEventColor, wxSOLID );

  wxPen end_marker_pen = wxPen ( m_endEventColor, 1, wxSOLID );
  wxBrush end_marker_brush = wxBrush ( m_endEventColor, wxSOLID );


  event_iterator_t ix;
  for (ix = m_eventMap.begin(); ix != m_eventMap.end(); ix++)
  {
    EventDef::handle_t eh = ix->second;

    eh->m_enabled = true; // all start enabled

    // look for minimum time to use as our base time offset
    // Since the events must be defined before they are used, all we have to do
    // is to look at the eh->m_time
    if (eh->m_time < m_timingOffset)
    {
      m_timingOffset = eh->m_time;
    }


    // Setup general colors
    eh->m_eventBaselinePen = wxPen ( m_defaultBaselineColor, 1, wxSOLID );

    wxPen event_duration_pen;
    wxPen event_marker_pen;

    // Check for event specific colours
    if (eh->m_color == -1)
    {
      // Use default colors
      event_duration_pen = wxPen ( m_defaultEventColor, 2, wxSOLID );
      event_marker_pen   = wxPen ( m_defaultEventColor, 1, wxSOLID );
    }
    else
    {
      int r = (eh->m_color >> 16) & 0xff;
      int g = (eh->m_color >>  8) & 0xff;
      int b = (eh->m_color >>  0) & 0xff;

      wxColour def_color ( r, g, b );

      event_duration_pen = wxPen ( def_color, 2, wxSOLID );
      event_marker_pen   = wxPen ( def_color, 1, wxSOLID );
    }


    // do specific processing by event type.
    if (eh->EventType() == Event::ET_DISCRETE_EVENT)
    {
      DiscreteEventDef * def = eh->GetDiscreteEvent();
      DiscreteEventDef::iterator_t it = def->m_list.begin();
      DiscreteEventDef::iterator_t eit = def->m_list.end();

      for ( ; it != eit; it++)
      {
        double ts = it->m_eventTime;

        if (ts > m_maxTime)
        {
          m_maxTime = ts;
        }

        it->m_eventMarkerPen = event_marker_pen;
        it->m_eventMarkerBrush = start_marker_brush;
      } // end for it

    }
    else
    {
      BoundedEventDef * def = eh->GetBoundedEvent();
      BoundedEventDef::iterator_t it = def->m_list.begin();
      BoundedEventDef::iterator_t eit = def->m_list.end();

      def->m_stats.m_minDuration = 1e300;
      def->m_stats.m_maxDuration = 0.0;
      double sum (0);
      double sum_sq (0);
      double count (0);

      for ( ; it != eit; it++)
      {
        double ts = it->m_endTime;

        if (ts > m_maxTime)
        {
          m_maxTime = ts;
        }

        it->m_startMarkerPen = start_marker_pen;
        it->m_startMarkerBrush = start_marker_brush;

        it->m_eventDurationPen = event_duration_pen;

        it->m_endMarkerPen = end_marker_pen;
        it->m_endMarkerBrush = end_marker_brush;

        double duration = it->m_endTime - it->m_startTime;
        if (def->m_stats.m_maxDuration < duration) def->m_stats.m_maxDuration = duration;
        if (def->m_stats.m_minDuration > duration) def->m_stats.m_minDuration = duration;
        sum += duration;
        sum_sq += (duration * duration);
        count ++;
      } // end for it

      def->m_stats.m_count = static_cast< int >(count);
      def->m_stats.m_avgDuration = sum / count;
      def->m_stats.m_stdDuration = sqrt((sum_sq - (sum * def->m_stats.m_avgDuration)) / (count - 1) );

      if (def->m_list.size() > 1) // need some elementsfor this to work
      {
        def->m_stats.m_activePct = (sum / (def->m_list.back().m_endTime - def->m_list.front().m_startTime)) * 100.0;
      }
      else
      {
        def->m_stats.m_activePct = 0;
      }
    }

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
SelectEvent (ItemId_t event)
{
  m_selectedEvent = event;

  // Need to redraw events
  ModelUpdate(UPDATE_EVENTS + UPDATE_INFO);
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


int Model::
EventCount() const
{
  return this->m_eventMap.size();
}


// ----------------------------------------------------------------
/** Number of displayable events.
 *
 * Calculate the number of displayable events. Good for determining
 * number of displayable lines.
 */
int Model::
DisplayableEventCount() const
{
  int count(0);
  const size_t limit ( m_drawOrder.size() );

  for (size_t i = 0; i < limit; i++)
  {
    if ( IsEventDisplayable ( m_drawOrder[i] ) )
    {
      count++;
    }
  } // end for

  return count;
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
/** Move selected event to top of list.
 *
 *
 */
void Model::
MoveSelectedEventTop()
{
  ItemId_t item = GetSelectedEvent();
  if (item < 0)
  {
    return; // no selected event
  }

  size_t index(0);

  // Scan the drawing order vector

  // Start loop at 1 since if the desired element is at index 0, it
  // can not be moved higher, so don't even try.
  size_t limit = m_drawOrder.size();
  for (size_t i = 1; i < limit; ++i)
  {
    if (m_drawOrder[i] == item)
    {
      index = i;
      break;
    }
  } // end for

  // On exit, index -> selected elements location in draw order,
  // of zero, indicating not found or already at top
  if (0 == index)
  {
    return; // item not found
  }

  // shift list down from [0] .. [index] one slot.
  for (size_t i = index; i > 0; i--)
  {
    m_drawOrder[i] = m_drawOrder[i-1];
  }

  // store item at Top
  m_drawOrder[0] = item;

  // Need to redraw events
  ModelUpdate(UPDATE_EVENTS);
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
  for (size_t i = 0; i < limit; ++i)
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


// ----------------------------------------------------------------
/** Move selected event to bottom of list.
 *
 *
 */
void Model::
MoveSelectedEventBottom()
{
  ItemId_t item = GetSelectedEvent();
  if (item < 0)
  {
    return; // no selected event
  }

  size_t index(0);

  // Scan the drawing order vector

  // End loop at next to the last element, since if the one we are
  // looking for is there, it can not be moved.
  size_t limit = m_drawOrder.size() -1;
  for (size_t i = 0; i < limit; ++i)
  {
    if (m_drawOrder[i] == item)
    {
      index = i;
      break;
    }
  } // end for

  // On exit, index -> selected elements location in draw order,
  // of zero, indicating not found.
  if (0 == index)
  {
    return; // item not found - not expected
  }

  // shift list down from [index] .. [size-1] one slot.
  for (size_t i = index; i < limit-1; i--)
  {
    m_drawOrder[i] = m_drawOrder[i+1];
  }

  // store item at Bottom
  m_drawOrder[limit] = item;

  // Need to redraw events
  ModelUpdate(UPDATE_EVENTS);
}


// ----------------------------------------------------------------
/** Set filter mode.
 *
 *
 */
void Model::
SetEventFilter( bool v )
{
  m_eventFilter = v;

  // Need to redraw events
  ModelUpdate(UPDATE_EVENTS);
}


// ----------------------------------------------------------------
/** Is event filtered out.
 *
 * This method determines if the specified event is filtered out of
 * the display.
 *
 * This interface is designed to support a more general event
 * filtering scheme, but for now the filter only suppresses events
 * with no occurrences.
 *
 * I can see a time when the filter is more script like and can apply
 * a wide range of predicates.
 *
 * @param[in] event - event id to check if filtered
 *
 * @retval true - display event
 * @retval false - do not display event
 */
bool Model::
IsEventDisplayable(ItemId_t event) const
{
  if (m_eventFilter) // is filter enabled
  {
    const_event_iterator_t ix = m_eventMap.find (event);
    size_t count = ix->second->NumOccurrences();
    return (count != 0);
  }

  // display by default
  return true;
}
