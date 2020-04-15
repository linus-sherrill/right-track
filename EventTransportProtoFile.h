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

  virtual int Write(EventDefinition & msg);
  virtual int Write(EventStart & msg);
  virtual int Write(EventEnd & msg);
  virtual int Write(EventText & msg);
  virtual int Write(ContextDefinition & msg);
  virtual int Write(ContextPush & msg);
  virtual int Write(ContextPop & msg);

  virtual int ReadEvents(std::string const& resource,
                         EventTransportReader & reader);

  bool OpenFile (std::string name);


private:
  // support for writing
  std::ofstream * m_outStream;

}; // end class EventTransportProtobuf

} // end namespace
} // end namespace

#endif // _RIGHT_TRACK_EVENT_TRANSPORT_PROTO_FILE_H_
