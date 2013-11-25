/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <EventTransportReaderDebug.h>

#include <iostream>



namespace RightTrack {
  namespace Internal {


int EventTransportReaderDebug::
NewEvent(EventDefinition const& msg)
{
  std::cout << "Event definition: "
           << "  name: \"" << msg.event_name << "\""
           << "  id: " << msg.event_id
           << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
           << "  type: " << msg.event_type
           << "  group: " << msg.event_group
           << "  color: " << msg.event_color
           << std::endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(EventStart const& msg)
{
  std::cout << "Event start: "
           << "  id: " << msg.event_id
           << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
           << "  pid: " << msg.event_pid
           << "  data: " << msg.event_data
           << std::endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(EventEnd const& msg)
{
  std::cout << "Event end: "
           << "  id: " << msg.event_id
           << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
           << "  pid: " << msg.event_pid
           << "  data: " << msg.event_data
           << std::endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(EventText const& msg)
{
  std::cout << "Event text: "
           << "  id: " << msg.event_id
           << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
           << "  pid: " << msg.event_pid
           << "  text: \"" << msg.event_text << "\""
           << std::endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(ContextDefinition const& msg)
{
  std::cout << "Context definition: "
           << "  name: \"" << msg.context_name << "\""
           << "  id: " << msg.context_id
           << std::endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(ContextPush const& msg)
{
   std::cout << "Context push: "
            << "  id: " << msg.context_id
            << std::endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(ContextPop const& msg)
{
   std::cout << "Context pop: "
                 << "  id: " << msg.context_id
                 << std::endl;

  return (0);
}

  } // end namespace
} // end namespace
