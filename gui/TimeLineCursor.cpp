/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "TimeLineCursor.h"


// ----------------------------------------------------------------
/**
 *
 *
 */
TimeLineCursor::
TimeLineCursor()
  : m_xCoord(0),
    m_enabled(false)

{

}


TimeLineCursor::
~TimeLineCursor()
{

}


// ----------------------------------------------------------------
/** Draw cursor in DC.
 *
 * @param[in] dc - drawing contect (visible portion only)
 * @param[in] view - current window view of the full event sapce
 */
void TimeLineCursor::
Draw(wxClientDC& dc, wxRect view)
{
  // Check to see if cursor is visible. It is really an error if not
  // visible. Otherwise, what's the point.
  if ( ! m_enabled || (m_xCoord < view.GetLeft()) || (m_xCoord > view.GetRight()) )
  {
    return;
  }

  // Start with a red line
  dc.SetPen( wxPen( wxColor( 250, 0, 0), 2, wxSOLID ) );
  dc.DrawLine( m_xCoord - view.GetLeft(), 0,
               m_xCoord - view.GetLeft(), view.GetBottom() );

  // draw grab rect.
//  dc.DrawRectangle( m_xCoord-2, view.GetBottom(), 5, -5 );
}


// ----------------------------------------------------------------
/** Move cursor to new location.
 *
 *
 */
void TimeLineCursor::
Move (int x_coord)
{
  m_xCoord = x_coord;
  std::cout << "cursor set to " << x_coord << "\n"; //+ TEMP
  // It would be nice to redraw cursor here, but we don't have enough
  // info.

}


// ----------------------------------------------------------------
/** Test for mouse click on cursor.
 *
 * This method checks to see if the specified point is associated with
 * the cursor.  Currently we only check for clicks that are close in X
 * to the draw location.
 *
 * @param click_point - point to check. Typically this is a mouse click point.
 *
 * @return true if click is on cursor, false otherwise.
 */
bool TimeLineCursor::
IsSelected ( wxPoint click_point)
{
  if ( !m_enabled)
  {
    return false;
  }

  // It would be better to check for clicks in the handle area, but we
  // don't really know the content window size. We need to know the
  // bottom coord since the handle is drawn there.
  //
  // So, as a quick fix, just check for the X coord +/- a little bit.

  if (abs(click_point.x - m_xCoord) < 4)
  {
    return true;
  }

  return false;

}



