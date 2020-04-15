/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_DISCRETE_EVENT_
#define _RIGHT_TRACK_DISCRETE_EVENT_

#include "Event.h"

namespace RightTrack {

// ----------------------------------------------------------------
/**
 *
 *
 */
class DiscreteEvent
  : public Internal::Event
{
public:
  DiscreteEvent(std::string name,
                std::string group = std::string(),
                int color = -1);
  virtual ~DiscreteEvent();

  using Event::Start;
  void Start( ::RightTrack::EventData_t const& val ) override;
  inline void Signal() { Event::Start(); }

  EventType_t EventType() const override;

private:
  void End( ::RightTrack::EventData_t const& val) override;

}; // end class DiscreteEvent

} // end namespace

#endif /* _RIGHT_TRACK_DISCRETE_EVENT_ */
