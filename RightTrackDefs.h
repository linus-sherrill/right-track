/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_EVENTDEFS_H_
#define _RIGHT_TRACK__EVENTDEFS_H_

#include <stdint.h>

namespace RightTrack {

typedef double EventData_t;

  namespace Internal {

typedef uint64_t EventTimestamp_t;
typedef uint32_t ItemId_t;
typedef uint32_t EventPid_t;



}
}

#endif /* _RIGHT_TRACK__EVENTDEFS_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
