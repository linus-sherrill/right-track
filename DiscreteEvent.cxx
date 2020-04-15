/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <DiscreteEvent.h>

#include <EventManager.h>
#include <RightTrackDefs.h>

namespace RightTrack {

// ----------------------------------------------------------------
/** Constructor.
 *
 * @param[in] name - descriptive name for this event. A method or
 * function name is good for events that cover the whole function or
 * method.
 *
 * @param[in] group - name for logically grouping events. Method
 * events that all relate to the same class could be grouped under the
 * class name. Groups can be hierarchically composed using
 * '.'separated names, such as "foo.bar".
 *
 * @param[in] color - a 24 bit value specifying the RGB components
 * using the HTML color encoding scheme. You can use a set of
 * predefined color names or just encode them as a literal.
 */
DiscreteEvent::
DiscreteEvent(std::string name, std::string group, int color)
  : Event(name, group, color)
{
  Manager()->RegisterEvent(this);
}


DiscreteEvent::
~DiscreteEvent()
{

}


void DiscreteEvent::
Start( ::RightTrack::EventData_t const& val)
{
  Manager()->StartEvent( this, val );
}


void DiscreteEvent::
End( ::RightTrack::EventData_t const&)
{
  // should never be called
  // Raise an error
}


Internal::Event::EventType_t DiscreteEvent::
EventType() const
{
  return ::RightTrack::Internal::Event::ET_DISCRETE_EVENT;
}

} // end namespace
