/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <wx/wx.h>

#ifndef _TIMELINECURSOR_H_
#define _TIMELINECURSOR_H_

// ----------------------------------------------------------------
/** Time Line Cursor
 *
 * This class represents a single cursor placed at a specific X
 * coordinate in the whole event space.
 *
 * Since we are drawn last over all other view content, we can not
 * erase ourselves.
 */
class TimeLineCursor
{
public:
  TimeLineCursor() = default;
  virtual ~TimeLineCursor() = default;

  void Enable(bool v = true) { m_enabled = v; }
  void Draw(wxClientDC& dc, wxRect view);
  void Move (int x_coord); // moves cursor to new location
  int GetLocation() const { return m_xCoord; }

  bool IsSelected ( wxPoint pt); // point in client coords (not virt coords)

private:
  int m_xCoord {0}; // x coordinate of cursor
  bool m_enabled {false};
}; // end class TimeLineCursor

#endif /* _TIMELINECURSOR_H_ */
