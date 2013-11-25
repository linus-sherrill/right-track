/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <Event.h>

#include <EventManager.h>

namespace RightTrack {
namespace Internal {

// ----------------------------------------------------------------
/** Constructor.
 *
 * @param[in] name -
 * @param[in] group -
 * @param[in] color -
 */
Event::
Event(std::string const& name, std::string group, int color)
  : RightTrackItem(name),
    m_eventGroup(group),
    m_eventColor(color)
{

}


Event::
~Event()
{

}


// ----------------------------------------------------------------
/** Get event group.
 *
 *
 */
std::string const&
Event::
EventGroup() const
{
  return this->m_eventGroup;
}


// ----------------------------------------------------------------
/** Get event color.
 *
 *
 */
int Event::
EventColor() const
{
  return this->m_eventColor;
}


void Event::
EventColor (int rgb)
{
  this->m_eventColor = rgb;
}


void Event::
EventColor (unsigned char r, unsigned char g, unsigned char b)
{
  this->m_eventColor = ((((r << 8) | g) << 8) | b);
}


} // end namespace
} // end namespace

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
