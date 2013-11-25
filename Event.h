/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef _RIGHT_TRACK_EVENT_
#define _RIGHT_TRACK_EVENT_


#include "RightTrackItem.h"



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
  enum EventType_t {
    ET_UNDEFINED_EVENT = 0,
    ET_BOUNDED_EVENT,
    ET_DISCRETE_EVENT,
    ET_TEXT_EVENT
  };

  // -- CONSTRUCTORS --
  Event(std::string const& name,
        std::string group,
        int color);
  virtual ~Event();

  // -- ACCESSORS --
  virtual void Start(::RightTrack::EventData_t val = 0) = 0;
  virtual void End(::RightTrack::EventData_t val = 0) = 0;

  virtual EventType_t EventType() const = 0;
  std::string const& EventGroup() const;
  int EventColor() const;
  void EventColor (int rgb);
  void EventColor (unsigned char r, unsigned char g, unsigned char b);


protected:


private:
  std::string m_eventGroup; // as in "a.b.c.eventName"
  unsigned int m_eventColor;

}; // end class Event

} // end namespace
} // end namespace

#endif /* _RIGHT_TRACK_EVENT_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
