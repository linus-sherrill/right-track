/*ckwg +5
 * Copyright 2010, 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 1712 Route 9, Clifton Park, NY 12065.
 */

#ifndef _EVENT_CANVAS_APP_H_
#define _EVENT_CANVAS_APP_H_

#include <wx/wx.h>

#include <Model.h>
#include <RightTrackDefs.h>
#include <TimeLineCursor.h>

class MainFrameApp;
struct EventHistory_t;

using namespace RightTrack;
using namespace RightTrack::Internal;

// ----------------------------------------------------------------

/** Click location.
 *
 * This structure represents a selected event and occurrence.  Both
 * event and occurrence can be null if the mouser's aim is not so
 * good.
 */
struct SelectedEvent
{
  // Raw click location
  wxPoint cl_rawClick;

  // Pointer to event clicked on or zero if none.
  EventDef* cl_event;

  // Pointer to event occurrence clicked on or zero if none.
  BaseOccurrence* cl_occurrence;

  // CTOR
  SelectedEvent()
  : cl_event( 0 )
  , cl_occurrence( 0 )
  {}
};

// ----------------------------------------------------------------

/** Event drawing canvas.
 *
 *
 */
class EventCanvasApp
  : public wxScrolledWindow
{
public:
  EventCanvasApp( wxWindow* parent,
                  int id,
                  const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize,
                  long style = wxDEFAULT_FRAME_STYLE );

  virtual ~EventCanvasApp() = default;

  void DrawNow();
  void XZoom( float factor );
  double
  XZoomFactor() const { return m_pixelsPerSecond / m_defaultPixelsPerSecond; }
  void SetDefaultScaling();
  void ResetView();
  wxRect GetCurrentView();
  wxPoint ConvertViewToCanvas( wxPoint pt );

  void EnableCursors( bool enab );
  void ResetCursorToModel();
  void ResetCursors();

  void SetMainView( MainFrameApp* main) { m_mainView = main; }
  MainFrameApp* MainView() { return m_mainView; }

protected:
  Model* GetModel() const { return Model::Instance(); }

  void DrawEvents( wxDC& dc );
  void DrawBoundedEvent( wxDC& dc, BoundedEventDef* eh, int y_coord );
  void DrawDiscreteEvent( wxDC& dc, DiscreteEventDef* eh, int y_coord );
  void DrawCursors();
  void NormalizeCursors();
  void DrawCommentAnnotation( wxDC& dc, int x, int y );

  // display management
  wxSize CalculateCanvasSize();
  int SecondsToXcoord( double ts ) const;
  double XcoordToSeconds( int xcoord ) const;

  // Event handlers
  void OnMouseLeftUpEvent( wxMouseEvent& event );
  void OnMouseLeftDownEvent( wxMouseEvent& event );
  void OnMouseMotionEvent( wxMouseEvent& event );
  void OnMouseRightClick( wxMouseEvent& evt );
  
  void OnPopupClick( wxCommandEvent& evt );
  
  void OnDraw( wxDC& dc ) override;

  SelectedEvent DetermineClickLocation( wxPoint pt );

  DECLARE_EVENT_TABLE();

private:
  MainFrameApp* m_mainView;

  double m_pixelsPerSecond; // X scaling factor for zoom
  double m_defaultPixelsPerSecond; // X scaling factor for zoom

  // Cursor handling fields
  TimeLineCursor m_cursor_1;
  TimeLineCursor m_cursor_2;

  // This indicates which cursor is being dragged with 0 meaning
  // neither one is selected or being dragged.
  int m_cursorDrag;

  // This constant sets the pixel height of an event display on the canvas.
  static constexpr int s_yIncrement {25};

}; // end class EventCanvasApp

#endif /* _EVENTCANVAS_H_ */
