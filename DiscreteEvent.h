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
  DiscreteEvent(vcl_string name,
                vcl_string group = vcl_string(),
                int color = -1);
  virtual ~DiscreteEvent();

  virtual void Start(::RightTrack::EventData_t val = 0);
  void Signal() { Start(0); }

  virtual EventType_t EventType() const;


private:
  virtual void End(::RightTrack::EventData_t val = 0);



}; // end class DiscreteEvent

} // end namespace

#endif /* _RIGHT_TRACK_DISCRETE_EVENT_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
