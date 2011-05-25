/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <BoundedEvent.h>

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
BoundedEvent::
BoundedEvent(vcl_string name, vcl_string group, int color)
  : Event(name, group, color)
{
  Manager()->RegisterEvent(this);

}


BoundedEvent::
~BoundedEvent()
{
  // Terminate event if still running
  if (m_started)
  {
    End();
    m_started = false;
  }
}


// ----------------------------------------------------------------
/** Start bounded event;
 *
 *
 */
void BoundedEvent::
Start( ::RightTrack::EventData_t val )
{
  // See if the event is still running
  if (m_started)
  {
    // TBD
  }

  Manager()->StartEvent( this, val );
  m_started = true;
}


// ----------------------------------------------------------------
/** End bounded event.
 *
 *
 */
void BoundedEvent::
End( ::RightTrack::EventData_t val )
{
  // Test for eventrunning
  if (m_started)
  {
    Manager()->EndEvent( this, val );
  }

  m_started = false;
}

// ----------------------------------------------------------------
/** Return our event type.
 *
 *
 */
Internal::Event::EventType_t BoundedEvent::
EventType() const
{
  return ::RightTrack::Internal::Event::ET_BOUNDED_EVENT;
}


} // end namespace

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
