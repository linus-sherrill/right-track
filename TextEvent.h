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
  TextEvent(std::string name,
            std::string group = std::string(),
            int color = -1);

  virtual ~TextEvent();

  virtual void Start(std::string const& val);
  virtual void Start(char const* val);

  EventType_t EventType() const override;


private:
  void Start(::RightTrack::EventData_t const& val) override;
  void End(::RightTrack::EventData_t const& val) override;

}; // end class TextEvent

} // end namespace

#endif /* _RIGHT_TRACK_TEXT_EVENT_ */
