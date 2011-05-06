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
Event(vcl_string const& name, vcl_string group, int color)
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
vcl_string const&
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
unsigned int Event::
EventColor() const
{
  return this->m_eventColor;
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
