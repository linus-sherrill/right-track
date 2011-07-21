/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <EventTransportReaderDebug.h>

#include <vcl_iostream.h>



namespace RightTrack {
  namespace Internal {


int EventTransportReaderDebug::
NewEvent(EventDefinition const& msg)
{
  vcl_cout << "Event definition: "
           << "  name: \"" << msg.event_name << "\""
           << "  id: " << msg.event_id
           << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
           << "  type: " << msg.event_type
           << "  group: " << msg.event_group
           << "  color: " << msg.event_color
           << vcl_endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(EventStart const& msg)
{
  vcl_cout << "Event start: "
           << "  id: " << msg.event_id
           << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
           << "  pid: " << msg.event_pid
           << "  data: " << msg.event_data
           << vcl_endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(EventEnd const& msg)
{
  vcl_cout << "Event end: "
           << "  id: " << msg.event_id
           << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
           << "  pid: " << msg.event_pid
           << "  data: " << msg.event_data
           << vcl_endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(ContextDefinition const& msg)
{
  vcl_cout << "Context definition: "
           << "  name: \"" << msg.context_name << "\""
           << "  id: " << msg.context_id
           << vcl_endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(ContextPush const& msg)
{
   vcl_cout << "Context push: "
            << "  id: " << msg.context_id
            << vcl_endl;

  return (0);
}


int EventTransportReaderDebug::
NewEvent(ContextPop const& msg)
{
   vcl_cout << "Context pop: "
                 << "  id: " << msg.context_id
                 << vcl_endl;

  return (0);
}

  } // end namespace
} // end namespace
