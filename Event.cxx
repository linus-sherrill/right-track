/*ckwg +5
 * Copyright 2010, 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 1719 Route 9, Clifton Park, NY 12065.
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
{ }

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

void Event::
Start()
{
  ::RightTrack::EventData_t vect;
  this->Start( vect );
}


void Event::
End()
{
  ::RightTrack::EventData_t  vect;
  this->End( vect );
}

void Event::
Start( ::RightTrack::EventDatum_t val )
{
  ::RightTrack::EventData_t vect(1, val );
  this->Start( vect );
}


void Event::
End( ::RightTrack::EventDatum_t val )
{
  ::RightTrack::EventData_t vect(1, val );
  this->End( vect );
}


} // end namespace
} // end namespace
