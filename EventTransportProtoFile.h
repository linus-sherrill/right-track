/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef _RIGHT_TRACK_EVENT_TRANSPORT_PROTO_FILE_H_
#define _RIGHT_TRACK_EVENT_TRANSPORT_PROTO_FILE_H_

#include "EventTransport.h"

#include <ostream>
#include <fstream>

namespace RightTrack {
namespace Internal {

// ----------------------------------------------------------------
/** Event transport writing protobuf to a file.
 *
 *
 */
class EventTransportProtoFile
  : public EventTransport
{
public:
  EventTransportProtoFile();
  virtual ~EventTransportProtoFile();

  int Write(EventDefinition & msg) override;
  int Write(EventStart & msg) override;
  int Write(EventEnd & msg) override;
  int Write(EventText & msg) override;
  int Write(ContextDefinition & msg) override;
  int Write(ContextPush & msg) override;
  int Write(ContextPop & msg) override;

  int ReadEvents(std::string const& resource,
                 EventTransportReader & reader) override;

  bool OpenFile (std::string name);


private:
  // support for writing
  std::ofstream * m_outStream;

}; // end class EventTransportProtobuf

} // end namespace
} // end namespace

#endif // _RIGHT_TRACK_EVENT_TRANSPORT_PROTO_FILE_H_
