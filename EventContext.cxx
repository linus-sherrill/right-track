/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include <EventContext.h>

#include <EventManager.h>


namespace RightTrack {


// ----------------------------------------------------------------
/** Constructor.
 *
 *
 */
EventContext::
EventContext(std::string name)
  : RightTrackItem(name)
{
  Manager()->RegisterContext(this);
}


EventContext::
~EventContext()
{

}


void EventContext::
Push()
{
  Manager()->PushContext(this);
}


void EventContext::
Pop()
{
  Manager()->PopContext(this);
}

} // end namespace
