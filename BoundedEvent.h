/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_BOUNDEDEVENT_H_
#define _RIGHT_TRACK_BOUNDEDEVENT_H_

#include "Event.h"

namespace RightTrack {

// ----------------------------------------------------------------
/** Bounded Event.
 *
 * This class represents a temporal event that has a duration
 * delimited by the Start() and End() method calls.
 */
class BoundedEvent
  : public Internal::Event
{
public:
  BoundedEvent(std::string name,
               std::string group = std::string(),
               int color = -1);

  virtual ~BoundedEvent();

  using Event::Start;
  using Event::End;

  void Start( ::RightTrack::EventData_t const& val ) override;
  void End( ::RightTrack::EventData_t const& val ) override;

  EventType_t EventType() const override;

private:
  bool m_started;

}; // end class BoundedEvent

} // end namesapce

#endif /* _RIGHT_TRACK_BOUNDEDEVENT_H_ */
