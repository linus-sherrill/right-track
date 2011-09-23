/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_TEXT_EVENT_
#define _RIGHT_TRACK_TEXT_EVENT_

#include "Event.h"

namespace RightTrack {

// ----------------------------------------------------------------
/**
 *
 *
 */
class TextEvent
  : public Internal::Event
{
public:
  TextEvent(vcl_string name,
                vcl_string group = vcl_string(),
                int color = -1);
  virtual ~TextEvent();

  virtual void Start(::RightTrack::EventData_t val = 0);
  virtual void Start(vcl_string const& val);
  virtual void Start(char const* val);

  virtual EventType_t EventType() const;


private:
  virtual void End(::RightTrack::EventData_t val = 0);

}; // end class TextEvent

} // end namespace

#endif /* _RIGHT_TRACK_TEXT_EVENT_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
