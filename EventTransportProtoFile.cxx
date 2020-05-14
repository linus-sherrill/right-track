/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "EventTransportProtoFile.h"

#include <EventTransportReader.h>
#include <RightTrackDefs.h>
#include <ConvertProtobuf.h>

#include <EventTransportProtobuf.pb.h>

#include <string>
#include <iostream>
#include <sstream>

namespace RightTrack {
namespace Internal {

using namespace ::RightTrack::proto;


/*
 * Local support routine(s)
 */
// ------------------------------------------------------------------
static bool s_read( std::istream& str, void* buffer, size_t size)
{
  str.read( static_cast< char* >(buffer), size );
  if (! str )
  {
    if (str.eof())
    {
      return false;
    }

    //+ maybe return false
    throw( std::runtime_error( "Error reading from stream" ) );
  }

  return true;
}

// ----------------------------------------------------------------------------
static bool s_write( std::ostream& str, void const* buffer, size_t size )
{
  str.write( static_cast< char const* >(buffer), size );
  if (! str )
  {
    return false;
  }
  return true;
}

// ----------------------------------------------------------------------------
/*
 *
 * Write out event envelope to stream
 *
 * <env-size> <env-data>
 */
static bool
WriteEvent( std::ofstream& str,
            const ::RightTrack::proto::EventEnvelope& env )
{
  // Serialize the protobuf
  // Write data length
  const uint32_t len( env.ByteSizeLong() );

  if ( ! s_write( str, &len, sizeof(len) ) )
  {
    return false;
  }

  // serialize to string stream
  if ( ! env.SerializeToOstream( &str ) )
  {
    return false;
  }
  return true;
}


// ----------------------------------------------------------------
/** Constructor
 *
 *
 */
EventTransportProtoFile::
EventTransportProtoFile()
  : m_outStream(0)
{
}


EventTransportProtoFile::
~EventTransportProtoFile()
{
  if (m_outStream)
  {
    m_outStream->close();
  }

  delete m_outStream;
}

// ----------------------------------------------------------------------------
int EventTransportProtoFile::
Write(EventDefinition & msg)
{
  ::RightTrack::proto::EventEnvelope env;
  ConvertProtobuf( msg, env );
  WriteEvent( *m_outStream, env );

  return (0);
}

// ----------------------------------------------------------------------------
int EventTransportProtoFile::
Write(EventStart & msg)
{
  ::RightTrack::proto::EventEnvelope env;
  ConvertProtobuf( msg, env );
  WriteEvent( *m_outStream, env );

  return (0);
}

// ----------------------------------------------------------------------------
int EventTransportProtoFile::
Write(EventEnd & msg)
{
  ::RightTrack::proto::EventEnvelope env;
  ConvertProtobuf( msg, env );
  WriteEvent( *m_outStream, env );

  return (0);
}

// ----------------------------------------------------------------------------
int EventTransportProtoFile::
Write(EventText & msg)
{
  ::RightTrack::proto::EventEnvelope env;
  ConvertProtobuf( msg, env );
  WriteEvent( *m_outStream, env );

  return (0);
}

// ----------------------------------------------------------------------------
int EventTransportProtoFile::
Write(ContextDefinition & msg)
{
  ::RightTrack::proto::EventEnvelope env;
  ConvertProtobuf( msg, env );
  WriteEvent( *m_outStream, env );

  return (0);
}

// ----------------------------------------------------------------------------
int EventTransportProtoFile::
Write(ContextPush & msg)
{
  ::RightTrack::proto::EventEnvelope env;
  ConvertProtobuf( msg, env );
  WriteEvent( *m_outStream, env );

  return (0);
}

// ----------------------------------------------------------------------------
int EventTransportProtoFile::
Write(ContextPop & msg)
{
  ::RightTrack::proto::EventEnvelope env;
  ConvertProtobuf( msg, env );
  WriteEvent( *m_outStream, env );

  return (0);
}

// ----------------------------------------------------------------------------
/** Open output file.
 *
 * Filename based on application and PID.
 */
bool EventTransportProtoFile::
OpenFile (std::string name)
{
  m_outStream = new std::ofstream;
  m_outStream->open(name.c_str(),
                    std::ios_base::out
                    | std::ios_base::trunc
                    | std::ios_base::binary );
    if ( ! m_outStream )
    {
      std::cerr << "Unable to open output file \"" << name << "\"" << std::endl;
      return false;
    }

  return (true);
}


// ----------------------------------------------------------------------------
/** Event reader.
 *
 *
 */
int EventTransportProtoFile::
ReadEvents(const std::string& resource,
           EventTransportReader & reader)
{
  std::ifstream str;
  str.open(resource.c_str(),
              std::ios_base::in
              | std::ios_base::binary );

  if ( !  str )
  {
    //+ display error
    return 1;
  }

  int status(0);

  // loop over all input in stream
  while( true )
  {

    // Read the length of the next item
    uint32_t len;

    if ( ! s_read( str, &len, sizeof(len) ) )
    {
      // end of file
      return 0;
    }

    // read data into string stream
    char data_buffer[len];
    s_read( str, data_buffer, len );

    std::string buffer( data_buffer, len );
    std::stringstream buffer_stream( buffer );

    ::RightTrack::proto::EventEnvelope env;
    env.ParseFromIstream( &buffer_stream );

    // Dispatch based on envelope contents
    switch( env.type() )
    {
    case EventEnvelope_Type_EVENT_DEFINITION:
    {
      ::RightTrack::Internal::EventDefinition msg;
      ConvertProtobuf( env, msg );
      status = reader.NewEvent(msg);
    }
    break;

    case EventEnvelope_Type_EVENT_START:
    {
      ::RightTrack::Internal::EventStart msg;
      ConvertProtobuf( env, msg );
      status = reader.NewEvent(msg);
    }
    break;

    case EventEnvelope_Type_EVENT_END:
    {
      ::RightTrack::Internal::EventEnd msg;
      ConvertProtobuf( env, msg );
      status = reader.NewEvent(msg);
    }
    break;

    case EventEnvelope_Type_EVENT_TEXT:
    {
      ::RightTrack::Internal::EventText msg;
      ConvertProtobuf( env, msg );
      status = reader.NewEvent(msg);
    }
    break;

    case EventEnvelope_Type_CONTEXT_DEFINITION:
    {
      ::RightTrack::Internal::ContextDefinition msg;
      ConvertProtobuf( env, msg );
      status = reader.NewEvent(msg);
    }
    break;

    case EventEnvelope_Type_CONTEXT_PUSH:
    {
      ::RightTrack::Internal::ContextPush msg;
      ConvertProtobuf( env, msg );
      status = reader.NewEvent(msg);
    }
    break;

    case EventEnvelope_Type_CONTEXT_POP:
    {
      ::RightTrack::Internal::ContextPop msg;
      ConvertProtobuf( env, msg );
      status = reader.NewEvent(msg);
    }
    break;

    default:
      break;
    }

  } // end loop

  return (status);
}

} // end namespace
} // end namespace
