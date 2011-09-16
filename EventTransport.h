/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef _RIGHT_TRACK_EVENT_TRANSPORT_H_
#define _RIGHT_TRACK_EVENT_TRANSPORT_H_

#include "EventRecords.h"


namespace RightTrack {
namespace Internal {

  class EventTransportReader;

// ----------------------------------------------------------------
/** Event transport base class.
 *
 * This class defines the interface to the event transport
 * component. Transport objects are responsible for getting the event
 * stream to the appropriate backing store. And as such, they must
 * implement this interface.
 *
 * Data format conversions and data martialing is the responsibility
 * of a concrete EventTransport class.
 */
class EventTransport
{
public:
  EventTransport() { }
  virtual ~EventTransport() { }

  virtual int Write(EventDefinition & msg) = 0;
  virtual int Write(EventStart & msg) = 0;
  virtual int Write(EventEnd & msg) = 0;
  virtual int Write(EventText & msg) = 0;
  virtual int Write(ContextDefinition & msg) = 0;
  virtual int Write(ContextPush & msg) = 0;
  virtual int Write(ContextPop & msg) = 0;

  virtual int ReadEvents(vcl_string const& resource,
                         EventTransportReader & reader) = 0;


}; // end class EventTransport

} // end namespace
} // end namespace

#endif /* _RIGHT_TRACK_EVENT_TRANSPORT_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
