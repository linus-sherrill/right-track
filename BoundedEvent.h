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
  BoundedEvent(vcl_string name,
        vcl_string group = vcl_string(),
        int color = -1);

  virtual ~BoundedEvent();

  virtual void Start(::RightTrack::EventData_t val = 0);
  virtual void End(::RightTrack::EventData_t val = 0);

  virtual EventType_t EventType() const;


private:
  bool m_started;

}; // end class BoundedEvent

} // end namesapce

#endif /* _RIGHT_TRACK_BOUNDEDEVENT_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
