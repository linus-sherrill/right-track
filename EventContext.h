/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef _RIGHT_TRACK_EVENT_CONTEXT_
#define _RIGHT_TRACK_EVENT_CONTEXT_

#include "RightTrackItem.h"

namespace RightTrack {


// ----------------------------------------------------------------
/** Event context temporal grouping.
 *
 *
 */
class EventContext
  : public Internal::RightTrackItem
{
public:
  EventContext(vcl_string name);
  virtual ~EventContext();

  void Push();
  void Pop();


private:


}; // end class EventContext

} // end namespace

#endif /* _RIGHT_TRACK_EVENT_CONTEXT_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
