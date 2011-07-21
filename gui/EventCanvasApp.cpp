/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <EventCanvasApp.h>

#include <wx/dcclient.h>
#include <Model.h>

#include <boost/foreach.hpp>



// Define our custom event table
BEGIN_EVENT_TABLE (EventCanvasApp, wxScrolledWindow )
    EVT_LEFT_UP (                  EventCanvasApp::OnMouseLeftUpEvent)
    EVT_LEFT_DOWN (                EventCanvasApp::OnMouseLeftDownEvent)
    EVT_MOTION (                   EventCanvasApp::OnMouseMotionEvent)
END_EVENT_TABLE()



// ----------------------------------------------------------------
/** Constructor.
 *
 *
 */
EventCanvasApp::EventCanvasApp(wxWindow* parent,
                         int id,
                         const wxPoint& pos,
                         const wxSize& size,
                               long style)
  : wxScrolledWindow(parent, id, pos, size, style),
  m_pixelsPerSecond(0),
  m_defaultPixelsPerSecond(0)
{
  m_yIncrement = 25;

  m_cursor_1.Move(25);
  m_cursor_2.Move(150);
}


EventCanvasApp::
~EventCanvasApp()
{

}


// ----------------------------------------------------------------
/** Reset view parameters.
 *
 * This method resets all view parameters. We are called when new data
 * is loaded into the model, amoungst other causes.
 */
void EventCanvasApp::
ResetView()
{
  m_pixelsPerSecond = 0;
  SetDefaultScaling();

  m_yIncrement = 25;

  m_cursor_1.Move(25);
  m_cursor_2.Move(150);

  double ct_1 = XcoordToSeconds( m_cursor_1.GetLocation() );
  double ct_2 = XcoordToSeconds( m_cursor_2.GetLocation() );
  GetModel()->SetCursorTimes(ct_1, ct_2);

  DrawNow();
}


// ----------------------------------------------------------------
/** Reset cursot based on model data.
 *
 *
 */
void EventCanvasApp::
ResetCursorToModel()
{
  double ct_1;
  double ct_2;
  GetModel()->GetCursorTimes (ct_1, ct_2);

  std::cout << "Time from model: " << ct_1 << ", " << ct_2  //TEMP
            << "  Pixels per sec: " << m_pixelsPerSecond << "\n";

  int x_coord = ct_1  * m_pixelsPerSecond + 15;
  m_cursor_1.Move (x_coord);

  x_coord = ct_2  * m_pixelsPerSecond + 15 ;
  m_cursor_2.Move (x_coord);
}


// ----------------------------------------------------------------
/** Reset cursors to be in the display window.
 *
 *
 */
void EventCanvasApp::
ResetCursors()
{
  wxRect rect = GetCurrentView();

  int x = rect.x + (rect.GetWidth() * 0.3);
  m_cursor_1.Move(x);

  x = rect.x + (rect.GetWidth() * 0.6);
  m_cursor_2.Move(x);

  double ct_1 = XcoordToSeconds( m_cursor_1.GetLocation() );
  double ct_2 = XcoordToSeconds( m_cursor_2.GetLocation() );
  GetModel()->SetCursorTimes(ct_1, ct_2);
}


// ----------------------------------------------------------------
/** Draw on demand - now!!
 *
 *
 */
void EventCanvasApp::
DrawNow ()
{
  wxClientDC dc (this);
  DoPrepareDC(dc);
  DrawEvents(dc);
  DrawCursors();
}


// ----------------------------------------------------------------
/** Handle OnDraw event.
 *
 *
 */
void EventCanvasApp::
OnDraw( wxDC & dc)
{
  // Erase canvas
  dc.Clear();

  DrawEvents(dc);
  DrawCursors();
}


// ----------------------------------------------------------------
/** Draw all events in listed order.
 *
 * Draw all events into content area. The drawing area origin has been
 * modified to take into account the scrolled position.
 */
void EventCanvasApp::
DrawEvents(wxDC& dc)
{
  SetDefaultScaling();

  SetVirtualSize ( CalculateVirtualSize() );

  wxRect view = GetCurrentView(); // current view in virtual coords
  GetModel()->SetTimeBounds (XcoordToSeconds(view.GetLeft()), XcoordToSeconds(view.GetRight()) );

  int start_idx = view.GetTop() / m_yIncrement;
  int end_idx = view.GetBottom() / m_yIncrement;

  // backup one row to make sure thare are no partial row rendering.
  if (start_idx > 0) start_idx--;

  // for each event in their drawing order
  for (int ev_idx = start_idx; ev_idx <= end_idx; ev_idx++)
  {
    // Stop at the last element
    if ((unsigned)ev_idx >= GetModel()->m_drawOrder.size())
    {
      break;
    }

    ItemId_t ev = GetModel()->m_drawOrder[ev_idx];
    int y_coord = (ev_idx + 1) * m_yIncrement; // in virtual coords

    // Handle selected event
    if (GetModel()->IsEventSelected(ev))
    {
      // Draw light background for this event at y_coord +/- 12 pixels
      //
      dc.SetPen (wxPen (GetModel()->m_selectColor, 1, wxSOLID) );
      dc.SetBrush (wxBrush (GetModel()->m_selectColor, wxSOLID) );
    }
    else
    {
      // Use default color
      wxColour col = dc.GetBackground().GetColour();
      dc.SetPen (wxPen (col, 1, wxSOLID) );
      dc.SetBrush (dc.GetBackground() );
    }
    dc.DrawRectangle ( view.x, y_coord - 12, view.GetWidth(), 24);

    EventHistory_t * eh = & GetModel()->m_eventMap[ev];
    if (eh->EventType() == Event::ET_BOUNDED_EVENT)
    {
      DrawBoundedEvent (dc, eh, y_coord);
    }
    else
    {
      DrawDiscreteEvent (dc, eh, y_coord);
    }

    // Increment to next drawing line
    y_coord += m_yIncrement;
  } // end foreach

}


// ----------------------------------------------------------------
/** Draw bounded events.
 *
 * Draw a single bounded event history at specified Y location.
 *
 * @param[in] dc - drawing context
 * @param[in] eh - event history to draw
 * @param[in] y_coord - y coordinate to use for drawing
 */
void EventCanvasApp::
DrawBoundedEvent(wxDC & dc, EventHistory_t * eh, int y_coord)
{
  wxRect view = GetCurrentView(); // current view in virtual coords

  // Draw base line at y_coord
  dc.SetPen( eh->eventBaselinePen );
  dc.DrawLine (view.x, y_coord, view.x + view.GetWidth(), y_coord);

  // draw event name
  // Should render in g_EventNames panel
  wxFont fnt(7, wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL);
  dc.SetFont (fnt);
  dc.DrawText( eh->EventName(), view.x + 2, y_coord);


  Model::time_iterator_t it;
  Model::time_iterator_t bit = eh->EventHistory.begin();
  Model::time_iterator_t eit = eh->EventHistory.end();

  // Don't optimize this way if zoom factor is less than some zoom
  // factor because the time to do the optimization must be less than
  // the full draw time for the optimization to be of value.
  if (XZoomFactor() >= 2)
  {
    Model::time_iterator_t obit = eh->EventHistory.begin();
    Model::time_iterator_t oeit = eh->EventHistory.end();

    int l = view.GetLeft();
    int r = view.GetRight();

    for (it = bit; it != eit; it++)
    {
      if (it->ev_action == EventHistoryElement_t::END)
      {
        int x_coord = SecondsToXcoord (it->event_time);
        if ( (l >= 0) && (x_coord >= l) )
        {
          obit = --it; // set begin iterator
          l = -1; // switch to next state
        }
        else if ( x_coord > r )
        {
          oeit = it; // set end interator
          if (it != eit) oeit++;
          break;
        }
      } // end END element
    } // end for

    // copy optimized iterators
    bit = obit;
    eit = oeit;
  } // end if zoom factor (optimization)

  // draw the event history.
  for (it = bit; it != eit; it++)
  {
    int x_event_start;
    int x_coord = SecondsToXcoord (it->event_time);

    switch (it->ev_action)
    {
    case EventHistoryElement_t::START:
      // draw starting marker
      dc.SetPen( eh->startMarkerPen );
      dc.SetBrush (eh->startMarkerBrush );

      dc.DrawRectangle ( x_coord -1, y_coord, 3, -10);

      x_event_start = x_coord;
      break;

    case EventHistoryElement_t::END:
      // draw line from x_event_start to x_coord
      dc.SetPen( eh->eventDurationPen );
      dc.DrawLine (x_event_start, y_coord, x_coord, y_coord );

      // draw endmarker at x_coord
      dc.SetPen( eh->endMarkerPen );
      dc.SetBrush (eh->endMarkerBrush );

      dc.DrawRectangle ( x_coord -1, y_coord, 3, 10);
      break;
    } // end switch

  } // end for

}


// ----------------------------------------------------------------
/** Draw discrete events.
 *
 *
 */
void EventCanvasApp::
DrawDiscreteEvent(wxDC & dc, EventHistory_t * eh, int y_coord)
{
  wxRect view = GetCurrentView(); // current view in virtual coords

  // Draw base line at y_coord
  dc.SetPen( eh->eventBaselinePen );
  dc.DrawLine (view.x, y_coord, view.x + view.GetWidth(), y_coord);

  // draw event name
  wxFont fnt(7, wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL);
  dc.SetFont (fnt);
  dc.DrawText( eh->EventName(), 2, y_coord);

  Model::time_iterator_t it;
  Model::time_iterator_t bit = eh->EventHistory.begin();
  Model::time_iterator_t eit = eh->EventHistory.end();

  dc.SetPen( eh->eventMarkerPen );
  dc.SetBrush (eh->startMarkerBrush );

  // Don't optimize this way if zoom factor is less than some zoom
  // factor because the time to do the optimization must be less than
  // the full draw time for the optimization to be of value.
  if (XZoomFactor() >= 2)
  {
    Model::time_iterator_t obit = eh->EventHistory.begin();
    Model::time_iterator_t oeit = eh->EventHistory.end();

    int l = view.GetLeft();
    int r = view.GetRight();

    for (it = bit; it != eit; it++)
    {
      int x_coord = SecondsToXcoord (it->event_time);
      if ( (l >= 0) && (x_coord >= l) )
      {
        if (it != bit)
        {
          obit = it; // set begin iterator
        }
        l = -1; // switch to next state
      }
      else if ( x_coord > r )
      {
        oeit = it; // set end interator
        break;
      }
    } // end for

    // copy optimized iterators
    bit = obit;
    eit = oeit;
  } // end if zoom factor (optimization)

  for (it = bit; it != eit; it++)
  {
    int x_coord = SecondsToXcoord (it->event_time);

    // draw event marker
    wxPoint marker[3]; // make triangle marker
    marker[0] = wxPoint(-4,  0);
    marker[1] = wxPoint( 4,  0);
    marker[2] = wxPoint( 0, -10);
    dc.DrawPolygon ( WXSIZEOF (marker), marker, x_coord, y_coord);

  } // end for

}


// ----------------------------------------------------------------
/** Enable display of cursors.
 *
 *
 */
void EventCanvasApp::
EnableCursors(bool enab)
{
  wxSize sz = GetClientSize();

  m_cursor_1.Enable(enab);
  m_cursor_2.Enable(enab);

  DrawNow(); // update display

}


// ----------------------------------------------------------------
/** Draw cursor.
 *
 * This method draws the cursor at the specified location.
 *
 * It's almost like we need a cursor object to assist in detecting
 * mouse click and drag.
 */
void EventCanvasApp::
DrawCursors ()
{
  wxClientDC dc (this);

  wxRect rect = GetCurrentView();

  m_cursor_1.Draw( dc, rect);
  m_cursor_2.Draw( dc, rect);
}


// ----------------------------------------------------------------
/** Normalize cursors.
 *
 * This method normalizes the cursors so that cursor 1 is always less
 * than cursor 2.
 */
void EventCanvasApp::
NormalizeCursors()
{
  int x1 = m_cursor_1.GetLocation();
  int x2 = m_cursor_2.GetLocation();

  if (x1 > x2)
  {
    // swap cursor locations
    m_cursor_1.Move(x2);
    m_cursor_2.Move(x1);
  }
}


// ----------------------------------------------------------------
/** Handle mouse left down events
 *
 *
 */
void EventCanvasApp::
OnMouseLeftDownEvent ( wxMouseEvent& event)
{
  wxPoint pt (event.GetPosition() );
  // Model * p_model = GetModel();

  // Need to convert Content area coords to virtual coords
  int ppuX, ppuY, startX, startY;
  GetScrollPixelsPerUnit( &ppuX, &ppuY);
  GetViewStart (&startX, &startY);
  pt.x += startX * ppuX;
  pt.y += startY * ppuY;

  m_cursorDrag = 0;

  if (m_cursor_1.IsSelected(pt))
  {
    m_cursorDrag = 1;
  }
  else if (m_cursor_2.IsSelected(pt))
  {
    m_cursorDrag = 2;
  }

  return;
}


// ----------------------------------------------------------------
/** Handle mouse left button up
 *
 *
 */
void EventCanvasApp::
OnMouseLeftUpEvent ( wxMouseEvent& event)
{
  wxPoint pt (event.GetPosition() );
  Model * p_model = GetModel();

  // Need to convert Content area coords to virtual coords
  int ppuX, ppuY, startX, startY;
  GetScrollPixelsPerUnit( &ppuX, &ppuY);
  GetViewStart (&startX, &startY);
  pt.x += startX * ppuX;
  pt.y += startY * ppuY;

  std::cout << "Mouse left up event at ["
            << pt.x << ", " << pt.y << "]\n";

  if (m_cursorDrag > 0)
  {
    if (m_cursorDrag == 1)
    {
      m_cursor_1.Move (pt.x);
    }
    else
    {
      m_cursor_2.Move (pt.x);
    }

    NormalizeCursors();

    double ct_1 = XcoordToSeconds( m_cursor_1.GetLocation() );
    double ct_2 = XcoordToSeconds( m_cursor_2.GetLocation() );
    GetModel()->SetCursorTimes(ct_1, ct_2);

    m_cursorDrag = 0; // release drag object

    Refresh();
    return;
  }

  // 1) determine which event by looking at Y coord
  int event_idx = (((float) pt.y / m_yIncrement) + 0.5 - 1);

  // 2) determine offset time by looking at X coord
  double ots = XcoordToSeconds(pt.x);

  // 3) locate actual event record based on time
  ItemId_t item_id (-1);
  if ( (event_idx >= 0) && ((unsigned)event_idx < p_model->m_drawOrder.size()) )
  {
    item_id = p_model->m_drawOrder[event_idx];
    // Set selected item id in model
    p_model->SelectEvent (item_id);
  }

  // 3-a) locate the specific event based on 'ots' if there is one
  // TBD - call method in Model to locate the event.

  // 4) display event data by filling in Model.
  std::cout << "event idx: " << event_idx
            << "  offset time: " << ots
            << "  item id: " << item_id
            << "  pixels per second: " << m_pixelsPerSecond
            << "  time offset: " <<  GetModel()->TimeOffset()
            << std::endl;

  EventHistory_t * eh = & p_model->m_eventMap[item_id];

  BoundedEventStatistics stats;

  CalculateEventStats (eh, &stats);
  p_model->m_ei_eventCount = eh->EventHistory.size();

  //
  // look through <eh> for end event just greater than (ots + time_offset)
  //


  std::cout << "Event name: " << eh->ev_def.event_name
            << "  count: " << p_model->m_ei_eventCount << std::endl;

  GetModel()->SetEventInfo (stats);
}


// ----------------------------------------------------------------
/** Calculate event statistics.
 *
 *
 */
bool EventCanvasApp::
CalculateEventStats ( EventHistory_t * eh, BoundedEventStatistics * stats)
{
  stats->m_name = eh->EventName();

  // Must be a bounded event
  if (eh->EventType() != Event::ET_BOUNDED_EVENT)
  {
    stats->m_count = eh->EventHistory.size();
    return false;
  }

  EventHistory_t::const_iterator_t ix = eh->EventHistory.begin();
  EventHistory_t::const_iterator_t ex = eh->EventHistory.end();

  stats->m_minDuration = 1e300;
  stats->m_maxDuration = 0.0;
  double last_start = ix->event_time;
  double sum (0);
  double sum_sq (0);
  double count (0);

  for (; ix != ex; ix++)
  {
    if (ix->ev_action == EventHistoryElement_t::END)
    {
      double duration = ix->event_time - last_start;
      if (stats->m_maxDuration < duration) stats->m_maxDuration = duration;
      if (stats->m_minDuration > duration) stats->m_minDuration = duration;
      sum += duration;
      sum_sq += (duration * duration);
      count ++;
    }
    else if (ix->ev_action == EventHistoryElement_t::START)
    {
      last_start = ix->event_time;
    }
  } // end for

  stats->m_count = static_cast< int >(count);
  stats->m_avgDuration = sum / count;
  stats->m_stdDuration = sqrt((sum_sq - (sum * stats->m_avgDuration)) / (count - 1) );

  return true;
}


// ----------------------------------------------------------------
/** Handle mouse motion events
 *
 *
 */
void EventCanvasApp::
OnMouseMotionEvent ( wxMouseEvent& event)
{
  // wxPoint pt (event.GetPosition() );
  // Model * p_model = GetModel();

  // Test to see if we are moving an event.
  if (m_cursorDrag > 0)
  {


  }
}


// ----------------------------------------------------------------
/** Set default scaling.
 *
 * Default scaling is usually calcualted once after the temporal range
 * of all events is determined. The default scaling, also the minimum
 * x-zoom, is also determined.
 *
 * The default scaling may need to be recalculated of the window is
 * resized.
 */
void EventCanvasApp::
SetDefaultScaling()
{
  wxSize sz = this->GetClientSize();
  int width = sz.x;

  // reduce width by 25 pixels for draw margins.
  m_defaultPixelsPerSecond = static_cast< double >(width - 25) / GetModel()->EventTimeRange();

  // only update once
  if ( m_pixelsPerSecond <= 0)
  {
    m_pixelsPerSecond = m_defaultPixelsPerSecond;
  }
}


// ----------------------------------------------------------------
/** Calculate virtual canvas size.
 *
 * This method calculates the required virtual size needed to handle
 * the current set of events at the current zoom level.
 */
wxSize EventCanvasApp::
CalculateVirtualSize()
{
  wxSize v_sz;

  v_sz.x = GetModel()->EventTimeRange() * m_pixelsPerSecond;
  v_sz.y = (GetModel()->EventCount() + 2) * m_yIncrement;

  return v_sz;
}


// ----------------------------------------------------------------
/** Convert seconds to draw coordinates.
 *
 *
 */
int EventCanvasApp::
SecondsToXcoord(double ts) const
{
  // Add margin of 15 to conversion;
  int x_coord ((ts - GetModel()->TimeOffset()) * m_pixelsPerSecond + 15 );

  return x_coord;
}


// ----------------------------------------------------------------
/** Convert X coordinate to seconds.
 *
 *
 */
double EventCanvasApp::
XcoordToSeconds( int xcoord) const
{
  double sec = (double) (xcoord - 15) / m_pixelsPerSecond;
  if (sec < 0) sec = 0;
  return sec;
}

// ----------------------------------------------------------------
/** Zoom X scaling
 *
 *
 */
void EventCanvasApp::
XZoom ( float factor )
{
  if (factor < 0)
  {
    // zoom to fit window.
    m_pixelsPerSecond = 0;
    SetDefaultScaling();
  }
  else
  {
    this->m_pixelsPerSecond *= factor;

    // Limit the amount of zoom out to what fits on the canvas.
    if (m_pixelsPerSecond < m_defaultPixelsPerSecond)
    {
      m_pixelsPerSecond = m_defaultPixelsPerSecond;
    }
  }

  // update cursor x-coord based on zoom
  ResetCursorToModel();

  Refresh();
}


// ----------------------------------------------------------------
/** Get coordinates of current view.
 *
 * This method returns the virtual coordinates and size of the current
 * view of the Event Canvas based on scroller settings.
 *
 * x, y, width, height
 */
wxRect EventCanvasApp::
GetCurrentView()
{
  // Adjust for scrolled position
  int ppuX, ppuY, startX, startY;
  GetScrollPixelsPerUnit( &ppuX, &ppuY);
  GetViewStart (&startX, &startY);

  wxPoint origin(startX * ppuX, startY * ppuY);
  wxSize sz = this->GetClientSize();

  return wxRect (origin, sz);
}
