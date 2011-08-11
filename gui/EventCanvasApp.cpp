/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include "EventCanvasApp.h"

#include <wx/dcclient.h>

#include "Model.h"
#include "DisplayableIterator.h"
#include "TextEditDialogApp.h"

#include <boost/foreach.hpp>



// Define our custom event table
BEGIN_EVENT_TABLE (EventCanvasApp, wxScrolledWindow )
    EVT_LEFT_UP (                  EventCanvasApp::OnMouseLeftUpEvent)
    EVT_LEFT_DOWN (                EventCanvasApp::OnMouseLeftDownEvent)
    EVT_MOTION (                   EventCanvasApp::OnMouseMotionEvent)
    EVT_RIGHT_DOWN (               EventCanvasApp::OnMouseRightClick)

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
  GetModel()->SetCursorTimes(ct_1, ct_2); // causes update request
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
  Model * pm = GetModel();

  SetDefaultScaling();

  SetVirtualSize ( CalculateVirtualSize() );

  wxRect view = GetCurrentView(); // current view in virtual coords
  pm->SetTimeBounds (XcoordToSeconds(view.GetLeft()), XcoordToSeconds(view.GetRight()) );

  int start_idx = view.GetTop() / m_yIncrement;
  int end_idx = view.GetBottom() / m_yIncrement;

  // backup one row to make sure thare are no partial row rendering.
  if (start_idx > 0) start_idx--;

  // rows on screen we need to draw
  int row_count = end_idx - start_idx + 1;

  // row number to draw this event on
  int y_coord = (start_idx + 1) * m_yIncrement;

  DisplayableIterator event_it;
  // scan through list of events to find the start_idx'th DISPLAYABLE
  // element
  event_it.Next(start_idx);

  // for each event in their drawing order
  while ( event_it.IsCurrentValid() && (row_count >= 0))
  {
    ItemId_t ev_id = event_it.CurrentItemId();

    // Handle selected event
    if (pm->IsEventSelected(ev_id))
    {
      // Draw light background for this event at y_coord +/- 12 pixels
      //
      dc.SetPen (wxPen (pm->m_selectColor, 1, wxSOLID) );
      dc.SetBrush (wxBrush (pm->m_selectColor, wxSOLID) );
    }
    else
    {
      // Use default color
      wxColour col = dc.GetBackground().GetColour();
      dc.SetPen (wxPen (col, 1, wxSOLID) );
      dc.SetBrush (dc.GetBackground() );
    }
    dc.DrawRectangle ( view.x, y_coord - 12, view.GetWidth(), 24);

    EventDef::handle_t eh = event_it.CurrentEvent();

    if (eh->EventType() == Event::ET_BOUNDED_EVENT)
    {
      DrawBoundedEvent (dc, eh->GetBoundedEvent(), y_coord);
    }
    else
    {
      DrawDiscreteEvent (dc, eh->GetDiscreteEvent(), y_coord);
    }

    // Increment to next drawing line, row, event, ...
    y_coord += m_yIncrement;
    row_count--;
    event_it.Next();
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
DrawBoundedEvent(wxDC & dc, BoundedEventDef * eh, int y_coord)
{
  wxRect view = GetCurrentView(); // current view in virtual coords

  // Draw base line at y_coord
  dc.SetPen(eh->m_eventBaselinePen);
  dc.DrawLine(view.x, y_coord, view.x + view.GetWidth(), y_coord);

  // draw event name
  // Should render in g_EventNames panel
  wxFont fnt(7, wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL);
  dc.SetFont(fnt);
  dc.DrawText(eh->EventName(), view.x + 2, y_coord);

  // Test for event level comment
  if (eh->IsCommentActive())
  {
    DrawCommentAnnotation (dc, view.x + 6, y_coord);
  }

  BoundedEventDef::iterator_t it;
  BoundedEventDef::iterator_t bit = eh->m_list.begin();
  BoundedEventDef::iterator_t eit = eh->m_list.end();

  // Don't optimize this way if zoom factor is less than some zoom
  // factor because the time to do the optimization must be less than
  // the full draw time for the optimization to be of value.
  if (XZoomFactor() >= 2)
  {
    BoundedEventDef::iterator_t obit = eh->m_list.begin();
    BoundedEventDef::iterator_t oeit = eh->m_list.end();

    int l = view.GetLeft();
    int r = view.GetRight();

    for (it = bit; it != eit; it++)
    {
      if ( ( l >= 0 ) // first state and either endpoint is in the window
           && (( SecondsToXcoord((*it)->GetBoundedOccurrence()->m_startTime) >= l )
               || ( SecondsToXcoord((*it)->GetBoundedOccurrence()->m_endTime) >= l ))
        )
      {
        obit = it; // set begin iterator
        if (it != bit) obit--;
        l = -1; // switch to next state
      }
      else if ( SecondsToXcoord((*it)->GetBoundedOccurrence()->m_endTime) > r )
      {
        oeit = it; // set end interator
        if (it != eit)
        {
          oeit++;
        }
        break;
      }
    } // end for

    // copy optimized iterators
    bit = obit;
    eit = oeit;
  } // end if zoom factor (optimization)

  // draw the event history.
  for (it = bit; it != eit; it++)
  {
    int x_coord_start = SecondsToXcoord((*it)->GetBoundedOccurrence()->m_startTime);
    int x_coord_end = SecondsToXcoord((*it)->GetBoundedOccurrence()->m_endTime);

    // draw starting marker
    dc.SetPen((*it)->GetBoundedOccurrence()->m_startMarkerPen);
    dc.SetBrush((*it)->GetBoundedOccurrence()->m_startMarkerBrush);
    dc.DrawRectangle(x_coord_start - 1, y_coord, 3, -10);

    if ((*it)->GetBoundedOccurrence()->IsCommentActive())
    {
      DrawCommentAnnotation (dc, x_coord_start, y_coord - 10);
    }

    // draw line from x_event_start to x_coord
    dc.SetPen((*it)->GetBoundedOccurrence()->m_eventDurationPen);
    dc.DrawLine(x_coord_start, y_coord, x_coord_end + 1, y_coord);

    // draw endmarker at x_coord
    dc.SetPen((*it)->GetBoundedOccurrence()->m_endMarkerPen);
    dc.SetBrush((*it)->GetBoundedOccurrence()->m_endMarkerBrush);
    dc.DrawRectangle(x_coord_end - 1, y_coord, 3, 10);
  } // end for

}


// ----------------------------------------------------------------
/** Draw discrete events.
 *
 *
 */
void EventCanvasApp::
DrawDiscreteEvent(wxDC & dc, DiscreteEventDef * eh, int y_coord)
{
  wxRect view = GetCurrentView(); // current view in virtual coords

  // Draw base line at y_coord
  dc.SetPen( eh->m_eventBaselinePen );
  dc.DrawLine (view.x, y_coord, view.x + view.GetWidth(), y_coord);

  // draw event name
  wxFont fnt(7, wxFONTFAMILY_SWISS, wxNORMAL, wxNORMAL);
  dc.SetFont (fnt);
  dc.DrawText( eh->EventName(), 2, y_coord);

  // Test for event level comment
  if (eh->IsCommentActive())
  {
    DrawCommentAnnotation (dc, view.x + 6, y_coord);
  }

  EventDef::iterator_t it;
  EventDef::iterator_t bit = eh->m_list.begin();
  EventDef::iterator_t eit = eh->m_list.end();

  // Don't optimize this way if zoom factor is less than some zoom
  // factor because the time to do the optimization must be less than
  // the full draw time for the optimization to be of value.
  if (XZoomFactor() >= 2)
  {
    EventDef::iterator_t obit = eh->m_list.begin();
    EventDef::iterator_t oeit = eh->m_list.end();

    int l = view.GetLeft();
    int r = view.GetRight();

    for (it = bit; it != eit; it++)
    {
      int x_coord = SecondsToXcoord ((*it)->GetDiscreteOccurrence()->m_eventTime);
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
    int x_coord = SecondsToXcoord ((*it)->GetDiscreteOccurrence()->m_eventTime);

    dc.SetPen( (*it)->GetDiscreteOccurrence()->m_eventMarkerPen );
    dc.SetBrush ((*it)->GetDiscreteOccurrence()->m_eventMarkerBrush );

    // draw event marker
    wxPoint marker[3]; // make triangle marker
    marker[0] = wxPoint(-4,  0);
    marker[1] = wxPoint( 4,  0);
    marker[2] = wxPoint( 0, -10);
    dc.DrawPolygon ( WXSIZEOF (marker), marker, x_coord, y_coord);

    // Test for comment active and draw
    if ((*it)->GetDiscreteOccurrence()->IsCommentActive())
    {
      DrawCommentAnnotation (dc, x_coord, y_coord - 10);
    }

  } // end for

}


// ----------------------------------------------------------------
/** Draw comment annotation.
 *
 * This method draws a graphical annotation indicating there is a
 * comment.
 *
 * @param[in] x - x coordinate for drawing
 * @param[in] y - y coordinate for drawing
 */
void EventCanvasApp::
DrawCommentAnnotation(wxDC & dc, int x, int y)
{
  wxPen pen = wxPen ( GetModel()->m_commentMarkerColor, 1, wxSOLID );
  wxBrush brush = wxBrush ( GetModel()->m_commentMarkerColor, wxSOLID );

  dc.SetPen( pen );
  dc.SetBrush (brush );

  dc.DrawCircle(x, y, 2);
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

  // Determine the click location - event and occurrence
  ClickLocation cl = DetermineClickLocation (pt);
  if (cl.event != 0)
  {
    p_model->SelectEvent (cl.event->GetEventId());

    if (cl.occurrence != 0)
    {
      p_model->SelectOccurrence(cl.occurrence);
    }
  }

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
/** determine click location.
 *
 *
 */
ClickLocation EventCanvasApp::
DetermineClickLocation( wxPoint pt )
{
  ClickLocation ret_loc;

  // determine which event by looking at Y coord
  int row_idx = ( (int) (((float) pt.y / m_yIncrement) + 0.5) - 1);

  // determine offset time by looking at X coord
  double ots = XcoordToSeconds(pt.x);

  // Need to skip elements that are not currently displayed
  DisplayableIterator event_it;

  event_it.Next(row_idx);
  if (event_it.IsCurrentValid())
  {
    //+ DEBUG
    std::cout << "row idx: " << row_idx
              << "  offset time: " << ots
              << "  item id: " << event_it.CurrentItemId()
              << "  pixels per second: " << m_pixelsPerSecond
              << "  time offset: " <<  GetModel()->TimeOffset()
              << std::endl;

    EventDef * eh = event_it.CurrentEvent().get();
    ret_loc.event = eh;

    double ctime = ots + GetModel()->TimeOffset();
    double delta = 5.0 / m_pixelsPerSecond; // 5 pixels left and right
    BaseOccurrence * bop = eh->FindByTime (ctime, delta);
    ret_loc.occurrence = bop;  // ok if null is returned by FindByTime()
  } // end is valid event

  return ret_loc;
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
  v_sz.y = (GetModel()->DisplayableEventCount() + 2) * m_yIncrement;

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


// Menu event codes
enum {
  ID_ECM_COLOR_EVENT = 210001,
  ID_ECM_COLOR_OCCURRENCE,
  ID_ECM_EVENT_ANNOTATION,
  ID_ECM_OCCURRENCE_ANNOTATION,
};



// ----------------------------------------------------------------
/** Popup menu click handler.
 *
 *
 */
void EventCanvasApp::
OnPopupClick(wxCommandEvent& event)
{
  // fix data type for client data
  void* data = static_cast< wxMenu* >( event.GetEventObject() )->GetClientData();
  ClickLocation* loc = static_cast< ClickLocation* >(data);
  wxString comment_string;

  // handle menu code
  switch ( event.GetId() )
  {
    case ID_ECM_COLOR_EVENT:
    case ID_ECM_COLOR_OCCURRENCE:
    {
      // Enable all color variants
      GetModel()->m_persistentColourData.SetChooseFull(true);

      // display dialog.
      wxColourDialog dialog(this, & GetModel()->m_persistentColourData);
      if (dialog.ShowModal() == wxID_OK)
      {
        wxColourData ret_data = dialog.GetColourData();
        wxColour col = ret_data.GetColour();

        if ( event.GetId() == ID_ECM_COLOR_EVENT)
        {
          // store in event
          //+ maybe background colour?
          loc->event->m_eventBaselinePen = wxPen(col, 1, wxSOLID);
        }
        else // color occurence
        {
          // store in event
          if (loc->occurrence->GetBoundedOccurrence() != 0)
          {
            BoundedOccurrence* bop = loc->occurrence->GetBoundedOccurrence();
            bop->m_eventDurationPen = wxPen(col, 2, wxSOLID);
          }
          else
          {
            DiscreteOccurrence* dop = loc->occurrence->GetDiscreteOccurrence();
            dop->m_eventMarkerPen = wxPen(col, 1, wxSOLID);
            dop->m_eventMarkerBrush = wxBrush(col, wxSOLID);
          }
        }

      }
      break;
    }

    case ID_ECM_EVENT_ANNOTATION:
    case ID_ECM_OCCURRENCE_ANNOTATION:
    {
      wxString title = loc->event->EventName();

      if (event.GetId() == ID_ECM_EVENT_ANNOTATION)
      {
        comment_string = loc->event->GetUserComment();
      }
      else
      {
        comment_string = loc->occurrence->GetUserComment();
      }

      TextEditDialogApp dialog(this, -1, title);
      dialog.SetText(comment_string);

      if (dialog.ShowModal() == wxID_OK)
      {
        if (event.GetId() == ID_ECM_EVENT_ANNOTATION)
        {
          loc->event->SetUserComment( dialog.GetText() );
        }
        else
        {
          loc->occurrence->SetUserComment( dialog.GetText() );
        }
      }
      break;
    }

  } // switch
} /* OnPopupClick */


// ----------------------------------------------------------------
/** Handle right mouse click.
 *
 *
 */
void EventCanvasApp::
OnMouseRightClick(wxMouseEvent& event)
{
  wxPoint pt (event.GetPosition() );

  // Need to convert Content area coords to world coords
  int ppuX, ppuY, startX, startY;
  GetScrollPixelsPerUnit( &ppuX, &ppuY);
  GetViewStart (&startX, &startY);
  pt.x += startX * ppuX;
  pt.y += startY * ppuY;

  ClickLocation loc = DetermineClickLocation( pt );
  void* data = reinterpret_cast< void* >( &loc );

  // If clicked on occurrence, display one menu
  // Ic clicked on whole event, display another menu

  if (loc.occurrence != 0)
  {
    // Clicked on an occurrence
    wxMenu mnu;

    // client data could be occurrence clicked on
    mnu.SetClientData(data);

    // create new menu
    // Set color of this occurrence
    // Edit annotation of this occurrence
    mnu.Append(ID_ECM_COLOR_OCCURRENCE,  wxT("Set color"));
    mnu.Append(ID_ECM_OCCURRENCE_ANNOTATION,   wxT("Edit annotation"));
    mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, ( wxObjectEventFunction ) & EventCanvasApp::OnPopupClick, NULL, this);
    PopupMenu(&mnu);
  }
  else if (loc.event != 0)
  {
    // clicked on an event
    wxMenu mnu;

    // client data could be occurrence clicked on
    mnu.SetClientData(data);

    // create new menu
    // Set color of event
    // Edit annotation of event
    mnu.Append(ID_ECM_COLOR_EVENT,  wxT("Set color"));
    mnu.Append(ID_ECM_EVENT_ANNOTATION,   wxT("Edit annotation"));
    //+ edit group name?
    mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, ( wxObjectEventFunction ) & EventCanvasApp::OnPopupClick, NULL, this);
    PopupMenu(&mnu);
  }

  // else no action if not in a event area

}
