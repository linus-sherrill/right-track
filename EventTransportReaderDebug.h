/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef _RIGHT_TRACK_EVENT_TRANSPORT_READER_DEBUG_H_
#define _RIGHT_TRACK_EVENT_TRANSPORT_READER_DEBUG_H_

#include "EventTransportReader.h"

namespace RightTrack {
namespace Internal {

// ----------------------------------------------------------------
/** Event receiver debug class.
 *
 * This class is used to handle event structure creation when
 * unmarshling events from the transport medium.  One of the
 * NewEvent() methods is called with the appropriate type when that
 * type of event is read from the medium.
 */
class EventTransportReaderDebug
  : public EventTransportReader
{
public:
  EventTransportReaderDebug() { }
  virtual ~EventTransportReaderDebug() { }

  int NewEvent(EventDefinition const& msg) override;
  int NewEvent(EventStart const& msg) override;
  int NewEvent(EventEnd const& msg) override;
  int NewEvent(EventText const& msg) override;
  int NewEvent(ContextDefinition const& msg) override;
  int NewEvent(ContextPush const& msg) override;
  int NewEvent(ContextPop const& msg) override;

}; // end class EventTransportReaderDebug

} // end namespace
} // end namespace

#endif /* _RIGHT_TRACK_EVENT_TRANSPORT_READER_DEBUG_H_ */
