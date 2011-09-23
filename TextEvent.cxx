/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <TextEvent.h>

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
TextEvent::
TextEvent(vcl_string name, vcl_string group, int color)
  : Event(name, group, color)
{
  Manager()->RegisterEvent(this);
}


TextEvent::
~TextEvent()
{

}


void TextEvent::
Start(vcl_string const& val)
{
  Manager()->StartEvent( this, val );
}


void TextEvent::
Start(char const* val)
{
  Manager()->StartEvent( this, vcl_string (val) );
}


void TextEvent::
Start(::RightTrack::EventData_t val)
{
  // should never be called
  // Raise an error
}


void TextEvent::
End(::RightTrack::EventData_t val)
{
  // should never be called
  // Raise an error
}


Internal::Event::EventType_t TextEvent::
EventType() const
{
  return ::RightTrack::Internal::Event::ET_TEXT_EVENT;
}

} // end namespace
