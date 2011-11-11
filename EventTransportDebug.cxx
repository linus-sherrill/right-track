/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <EventTransportDebug.h>

#include <RightTrackDefs.h>

#include <vcl_string.h>
#include <vcl_iostream.h>


namespace RightTrack {
namespace Internal {

// ----------------------------------------------------------------
/** Constructor
 *
 *
 */
EventTransportDebug::
EventTransportDebug()
  : m_outStream(0)
{
  m_outStream = &vcl_cout;
}


EventTransportDebug::
~EventTransportDebug()
{

}


int EventTransportDebug::
Write(EventDefinition & msg)
{
  (*m_outStream) << "Event definition: "
                 << "  name: " << msg.event_name
                 << "  id: " << msg.event_id
                 << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
                 << "  type: " << msg.event_type
                 << "  group: " << msg.event_group
                 << "  color: " << msg.event_color
                 << vcl_endl;

  return (0);
}


int EventTransportDebug::
Write(EventStart & msg)
{
  (*m_outStream) << "Event start: "
                 << "  id: " << msg.event_id
                 << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
                 << "  pid: " << msg.event_pid
                 << "  data: " << msg.event_data
                 << vcl_endl;

  return (0);
}


int EventTransportDebug::
Write(EventEnd & msg)
{
  (*m_outStream) << "Event end: "
                 << "  id: " << msg.event_id
                 << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
                 << "  pid: " << msg.event_pid
                 << "  data: " << msg.event_data
                 << vcl_endl;

  return (0);
}


int EventTransportDebug::
Write(EventText & msg)
{
  (*m_outStream) << "Event end: "
                 << "  id: " << msg.event_id
                 << "  time: " << msg.event_time.secs << "." << msg.event_time.usecs
                 << "  pid: " << msg.event_pid
                 << "  text: \"" << msg.event_text << "\""
                 << vcl_endl;

  return (0);
}


int EventTransportDebug::
Write(ContextDefinition & msg)
{
  (*m_outStream) << "Context definition: "
                 << "  name: " << msg.context_name
                 << "  id: " << msg.context_id
                 << vcl_endl;

  return (0);
}


int EventTransportDebug::
Write(ContextPush & msg)
{
   (*m_outStream) << "Context push: "
                 << "  id: " << msg.context_id
                 << vcl_endl;

  return (0);
}


int EventTransportDebug::
Write(ContextPop & msg)
{
   (*m_outStream) << "Context pop: "
                 << "  id: " << msg.context_id
                 << vcl_endl;

  return (0);
}


} // end namespace
} // end namespace
