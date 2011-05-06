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
    EVT_MOUSE_EVENTS(              EventCanvasApp::OnMouseEvent)
    EVT_PAINT(                     EventCanvasApp::OnPaint)
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

}


EventCanvasApp::
~EventCanvasApp()
{

}


// ----------------------------------------------------------------
/** Handle Paint event.
 *
 *
 */
void EventCanvasApp::
OnPaint ( wxPaintEvent &event)
{
  wxPaintDC dc (this);
  DrawEvents (dc);
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
  DoPrepareDC (dc);

  DrawEvents(dc);
}
// ----------------------------------------------------------------
/** Handle OnDraw event.
 *
 *
 */
void EventCanvasApp::
OnDraw( wxDC & dc)
{
  std::cout << "@@@@ OnDraw()\n";
}


// ----------------------------------------------------------------
/** Draw all events in listed order.
 *
 *
 */
void EventCanvasApp::
DrawEvents(wxDC& dc)
{
  wxSize sz = this->GetClientSize();
  SetDefaultScaling(sz.x);

  SetVirtualSize ( CalculateVirtualSize() );

  // Erase canvas
  dc.Clear();

  // Adjust for scrolled position
  int ppuX, ppuY, startX, startY;
  GetScrollPixelsPerUnit( &ppuX, &ppuY);
  GetViewStart (&startX, &startY);
  dc.SetDeviceOrigin (-startX * ppuX, -startY * ppuY );

  int y_coord = m_yIncrement;

  // for each event in their drawing order
  BOOST_FOREACH (ItemId_t ev, GetModel()->m_drawOrder)
  {
    wxSize sz = this->GetVirtualSize();

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
 *
 */
void EventCanvasApp::
DrawBoundedEvent(wxDC & dc, EventHistory_t * eh, int y_coord)
{
  wxSize sz = this->GetVirtualSize();

  // Draw base line at y_coord
  dc.SetPen( eh->eventBaselinePen );
  dc.DrawLine (0, y_coord, sz.x, y_coord);

  // draw event name
  wxFont fnt(7, wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL);
  dc.SetFont (fnt);

  dc.DrawText( eh->EventName(), 2, y_coord);

  Model::time_iterator_t it = eh->EventHistory.begin();
  Model::time_iterator_t eit = eh->EventHistory.end();

  for ( ; it != eit; it++)
  {
    int x_event_start;
    int x_coord = SecondsToXcoord (it->event_time);

    switch (it->ev_action)
    {
    case EventHistoryElement_t::START:
      // draw starting marker
      dc.SetPen( eh->startMarkerPen );
      dc.SetBrush (eh->startMarkerBrush );

      dc.DrawRectangle ( x_coord, y_coord, 3, -10);

      x_event_start = x_coord;
      break;

    case EventHistoryElement_t::END:
      // draw line from x_event_start to x_coord
      dc.SetPen( eh->eventDurationPen );
      dc.DrawLine (x_event_start, y_coord, x_coord, y_coord );

      // draw endmarker at x_coord
      dc.SetPen( eh->endMarkerPen );
      dc.SetBrush (eh->endMarkerBrush );

      dc.DrawRectangle ( x_coord, y_coord, 3, 10);
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
  wxSize sz = this->GetVirtualSize();

  // Draw base line at y_coord
  dc.SetPen( eh->eventBaselinePen );
  dc.DrawLine (0, y_coord, sz.x, y_coord);

  // draw event name
  wxFont fnt(7, wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL);
  dc.SetFont (fnt);

  dc.DrawText( eh->EventName(), 2, y_coord);

  Model::time_iterator_t it = eh->EventHistory.begin();
  Model::time_iterator_t eit = eh->EventHistory.end();

  dc.SetPen( eh->eventMarkerPen );
  dc.SetBrush (eh->startMarkerBrush );

  for ( ; it != eit; it++)
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
/** Handle mouse events
 *
 *
 */
void EventCanvasApp::
OnMouseEvent ( wxMouseEvent& event)
{
  wxPoint pt (event.GetPosition() );
  Model * p_model = GetModel();

  if (event.LeftUp())
  {
    std::cout << "Mouse event at ["
            << pt.x << ", " << pt.y << "]\n";

    // Need to convert Content area coords to virtual coords
    int ppuX, ppuY, startX, startY;
    GetScrollPixelsPerUnit( &ppuX, &ppuY);
    GetViewStart (&startX, &startY);
    pt.x += startX * ppuX;
    pt.y += startY * ppuY;

    // 1) determine which event by looking at Y coord
    int event_idx = (((float) pt.y / m_yIncrement) + 0.5 - 1);

    // 2) determine offset time by looking at X coord
    EventTimestamp_t ots = (pt.x / m_pixelsPerSecond) * 1e6; // usec

    // 3) locate actual event record based on time
    ItemId_t item_id (-1);
    if ( (event_idx >= 0) && (event_idx < p_model->m_drawOrder.size()) )
    {
      item_id = p_model->m_drawOrder[event_idx];
    }

    // 4) display event data by filling in Model.
    std::cout << "event idx: " << event_idx
              << "  offset time: " << ots
              << "  item id: " << item_id
              << std::endl;
    EventHistory_t * eh = & p_model->m_eventMap[item_id];
    p_model->m_ei_eventCount = eh->EventHistory.size();

    std::cout << "Event name: " << eh->ev_def.event_name
              << "  count: " << p_model->m_ei_eventCount << std::endl;
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
 *
 * @param[in] width - \e physical width of the display window.
 */
void EventCanvasApp::
SetDefaultScaling( int width)
{
  // reduce width by 25 pixels for draw margins.
  m_defaultPixelsPerSecond = static_cast< double >(width - 25) / GetModel()->EventTimeRange();

  // only update once
  if (0 == m_pixelsPerSecond)
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
SecondsToXcoord(EventTimestamp_t ts) const
{
  // Add margin of 15 to conversion;
  return (ts - GetModel()->TimeOffset()) * m_pixelsPerSecond + 15;
}


// ----------------------------------------------------------------
/** Zoom X scaling
 *
 *
 */
void EventCanvasApp::
XZoom ( float factor)
{
  if (factor < 0)
  {
    m_pixelsPerSecond = 0;
    wxSize sz = this->GetClientSize();
    SetDefaultScaling(sz.x);
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
  Refresh();
}
