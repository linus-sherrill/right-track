/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _EVENT_CANVAS_APP_H_
#define _EVENT_CANVAS_APP_H_

#include <wx/wx.h>

#include <RightTrackDefs.h>
#include <Model.h>
#include <TimeLineCursor.h>


class MainFrameApp;

struct EventHistory_t;

using namespace RightTrack;
using namespace RightTrack::Internal;


// ----------------------------------------------------------------
/** Event drawing canvas.
 *
 *
 */
class EventCanvasApp
//   : public EventCanvas
  : public wxScrolledWindow
{
public:
  EventCanvasApp(wxWindow* parent,
              int id,
              const wxPoint& pos=wxDefaultPosition,
              const wxSize& size=wxDefaultSize,
              long style=wxDEFAULT_FRAME_STYLE);
  virtual ~EventCanvasApp();

  void DrawNow();
  void XZoom(float factor);
  double XZoomFactor() const { return m_pixelsPerSecond / m_defaultPixelsPerSecond; }
  void SetDefaultScaling();
  void ResetView();
  wxRect GetCurrentView();
  void EnableCursors (bool enab);
  void ResetCursorToModel();
  void ResetCursors();

  bool CalculateEventStats ( EventHistory_t * eh, BoundedEventStatistics * stats);


protected:
  Model * GetModel() const { return Model::Instance(); }

  void DrawEvents(wxDC& dc);
  void DrawBoundedEvent(wxDC & dc, EventHistory_t * eh, int y_coord);
  void DrawDiscreteEvent(wxDC & dc, EventHistory_t * eh, int y_coord);
  void DrawCursors ();
  void NormalizeCursors();

  // display management
  wxSize CalculateVirtualSize();
  int SecondsToXcoord(double ts) const;
  double XcoordToSeconds( int xcoord) const;


  // Event handlers / overrides
  virtual void OnMouseLeftUpEvent (wxMouseEvent& event);
  virtual void OnMouseLeftDownEvent (wxMouseEvent& event);
  virtual void OnMouseMotionEvent (wxMouseEvent& event);

  virtual void OnDraw( wxDC & dc);

  DECLARE_EVENT_TABLE();


private:
  MainFrameApp* m_parent;

  double m_pixelsPerSecond; // X scaling factor for zoom
  double m_defaultPixelsPerSecond; // X scaling factor for zoom

  int m_yIncrement;

  // Cursor handling fields
  TimeLineCursor m_cursor_1;
  TimeLineCursor m_cursor_2;

  int m_cursorDrag;


}; // end class EventCanvasApp

#endif /* _EVENTCANVAS_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
