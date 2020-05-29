/*ckwg +5
 * Copyright 2010, 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 1712 Route 9, Clifton Park, NY 12065.
 */

#ifndef _RIGHT_TRACK_EVENT_
#define _RIGHT_TRACK_EVENT_

#include "RightTrackItem.h"

#include <vector>

namespace RightTrack {
namespace Internal {

// ----------------------------------------------------------------

/** Event Base Class.
 *
 * This class represents the abstract base class for all events.
 */
class Event
  : public RightTrackItem
{
public:
  enum EventType_t
  {
    ET_UNDEFINED_EVENT = 0,
    ET_BOUNDED_EVENT,
    ET_DISCRETE_EVENT,
    ET_TEXT_EVENT,
    ET_CONTEXT,
  };

  // -- CONSTRUCTORS --
  Event( std::string const& name,
         std::string group,
         int color );

  virtual ~Event() = default;

  void Start();
  void End();

  void Start( ::RightTrack::EventDatum_t val );
  void End( ::RightTrack::EventDatum_t val );

  virtual void Start( ::RightTrack::EventData_t const& val ) = 0;
  virtual void End( ::RightTrack::EventData_t const& val ) = 0;

  virtual EventType_t EventType() const = 0;
  std::string const& EventGroup() const;
  int EventColor() const;
  void EventColor( int rgb );
  void EventColor( unsigned char r, unsigned char g, unsigned char b );

protected:
private:
  std::string m_eventGroup;   // as in "a.b.c.eventName"
  unsigned int m_eventColor;
}; // end class Event

}// namespace Internal
}// namespace RightTrack

#endif /* _RIGHT_TRACK_EVENT_ */
