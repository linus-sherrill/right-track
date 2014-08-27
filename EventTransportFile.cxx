/*ckwg +5
 * Copyright 2010,2014 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <EventTransportFile.h>

#include <EventTransportReader.h>

#include <RightTrackDefs.h>

#include <string>

#include <boost/serialization/string.hpp>
#include <boost/serialization/version.hpp>
#include <boost/serialization/split_member.hpp>


namespace {


//experimental

// ================================================================
class SerializeEventDefinition
{
public:
  SerializeEventDefinition( RightTrack::Internal::EventDefinition& msg )
    : m_msg( msg )
  { }


  template < class Archive >
  void save( Archive& ar, const unsigned int version )
  {
    ar & static_cast< int >(RightTrack::Internal::PAYLOAD_EVENT_DEFINITION);
    ar &  msg.event_id;
    ar & msg.event_time.secs;
    ar & msg.event_time.usecs;
    ar & (int) msg.event_type;
    ar & msg.event_name;
    ar & msg.event_group;
    ar & msg.event_color;
  }


  template < class Archive >
  void load( Archive& ar,  const unsigned int version )
  {
    ar & msg.event_id;
    ar & msg.event_time.secs;
    ar & msg.event_time.usecs;

    int tmp; // need to coerce ENUM
    ar & tmp;
    msg.event_type = static_cast< RightTrack::Internal::Event::EventType_t >(tmp);

    ar & msg.event_name;
    ar & msg.event_group;
    ar & msg.event_color;
  }

  BOOST_SERIALIZATION_SPLIT_MEMBER()

private:
  RightTrack::Internal::EventDefinition& m_msg;
};
BOOST_CLASS_VERSION(SerializeEventDefinition, 1)

// ETC

int EventTransportFile::
Write(EventDefinition & msg)
{
  SerializeEventDefinition ser( msg );
  serialize( *m_outArchive, ser, m_version );
  return (0);
}


    case PAYLOAD_EVENT_DEFINITION:
    {
      EventDefinition msg;
      SerializeEventDefinition ser( msg );
      serialize( ia, ser, m_version );
      status = reader.NewEvent(msg);
    }
    break;


// ================================================================

// replace templated class with boost::archive::text_oarchive
template < class Archive >
void save( Archive& ar, RightTrack::Internal::EventDefinition& msg, const unsigned int version )
{
  ar & static_cast< int >(RightTrack::Internal::PAYLOAD_EVENT_DEFINITION);
  ar &  msg.event_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
  ar & (int) msg.event_type;
  ar & msg.event_name;
  ar & msg.event_group;
  ar & msg.event_color;
}


template < class Archive >
void save( Archive& ar, RightTrack::Internal::EventStart& msg, const unsigned int version )
{
  ar & static_cast< int >(RightTrack::Internal::PAYLOAD_EVENT_START);
  ar & msg.event_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
  ar & msg.event_pid;
  ar & msg.event_data;
}


template < class Archive >
void save( Archive& ar, RightTrack::Internal::EventEnd& msg, const unsigned int version )
{
  ar & static_cast< int >(RightTrack::Internal::PAYLOAD_EVENT_END);
  ar & msg.event_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
  ar & msg.event_pid;
  ar & msg.event_data;
}


template < class Archive >
void save( Archive& ar, RightTrack::Internal::EventText& msg, const unsigned int version )
{
  ar & static_cast< int >(RightTrack::Internal::PAYLOAD_EVENT_TEXT);
  ar & msg.event_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
  ar & msg.event_pid;
  ar & msg.event_text;
}


template < class Archive >
void save( Archive& ar, RightTrack::Internal::ContextDefinition& msg, const unsigned int version )
{
  ar & static_cast< int >(RightTrack::Internal::PAYLOAD_CONTEXT_DEFINITION);
  ar & msg.context_name;
  ar & msg.context_id;
}


template < class Archive >
void save( Archive& ar, RightTrack::Internal::ContextPush& msg, const unsigned int version )
{
  ar & static_cast< int >(RightTrack::Internal::PAYLOAD_CONTEXT_PUSH);
  ar & msg.context_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
}


template < class Archive >
void save( Archive& ar, RightTrack::Internal::ContextPop& msg, const unsigned int version )
{
  ar & static_cast< int >(RightTrack::Internal::PAYLOAD_CONTEXT_POP);
  ar & msg.context_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
}


// ================================================================
template < class Archive >
void load( Archive& ar, RightTrack::Internal::EventDefinition& msg, const unsigned int version )
{
  ar & msg.event_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;

  int tmp; // need to coerce ENUM
  ar & tmp;
  msg.event_type = static_cast< RightTrack::Internal::Event::EventType_t >(tmp);

  ar & msg.event_name;
  ar & msg.event_group;
  ar & msg.event_color;
}


template < class Archive >
void load( Archive& ar, RightTrack::Internal::EventStart& msg, const unsigned int version )
{
  ar & msg.event_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
  ar & msg.event_pid;
  ar & msg.event_data;
}


template < class Archive >
void load( Archive& ar, RightTrack::Internal::EventEnd& msg, const unsigned int version )
{
  ar & msg.event_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
  ar & msg.event_pid;
  ar & msg.event_data;
}


template < class Archive >
void load( Archive& ar, RightTrack::Internal::EventText& msg, const unsigned int version )
{
  ar & msg.event_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
  ar & msg.event_pid;
  ar & msg.event_text;
}


template < class Archive >
void load( Archive& ar, RightTrack::Internal::ContextDefinition& msg, const unsigned int version )
{
  ar & msg.context_name;
  ar & msg.context_id;
}


template < class Archive >
void load( Archive& ar, RightTrack::Internal::ContextPush& msg, const unsigned int version )
{
  ar & msg.context_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
}


template < class Archive >
void load( Archive& ar, RightTrack::Internal::ContextPop& msg, const unsigned int version )
{
  ar & msg.context_id;
  ar & msg.event_time.secs;
  ar & msg.event_time.usecs;
}


} // end private namespace


using namespace boost::serialization;

namespace RightTrack {
namespace Internal {


// ----------------------------------------------------------------
/** Constructor
 *
 *
 */
EventTransportFile::
EventTransportFile()
  : m_outStream(0),
    m_outArchive(0),
    m_version(1)
{

}


EventTransportFile::
~EventTransportFile()
{
  if (m_outStream)
  {
    m_outStream->close();
  }

  delete m_outArchive;
  delete m_outStream;
}


int EventTransportFile::
Write(EventDefinition & msg)
{
  save( *m_outArchive, msg );
  return (0);
}


int EventTransportFile::
Write(EventStart & msg)
{
  save( *m_outArchive, msg );
  return (0);
}


int EventTransportFile::
Write(EventEnd & msg)
{
  save( *m_outArchive, msg );
  return (0);
}


int EventTransportFile::
Write(EventText & msg)
{
  save( *m_outArchive, msg );
  return (0);
}


int EventTransportFile::
Write(ContextDefinition & msg)
{
  save( *m_outArchive, msg );
  return (0);
}


int EventTransportFile::
Write(ContextPush & msg)
{
  save( *m_outArchive, msg );
  return (0);
}


int EventTransportFile::
Write(ContextPop & msg)
{
  save( *m_outArchive, msg );
  return (0);
}


// ----------------------------------------------------------------
/** Open output file.
 *
 * Filename based on application and PID.
 */
bool EventTransportFile::
OpenFile (std::string name)
{
  m_outStream = new std::ofstream;
  m_outStream->open( name.c_str() );

  m_outArchive = new boost::archive::text_oarchive( *m_outStream );

  return (true);
}


// ----------------------------------------------------------------
/** Event reader.
 *
 *
 */
int EventTransportFile::
ReadEvents(std::string const& resource,
           EventTransportReader & reader)
{
  std::ifstream ifs( resource.c_str() );
  if (! ifs)
  {
    // log error
    return (1);
  }

  boost::archive::text_iarchive ia( ifs );

  int status(0);

  // loop over all input in stream
  for ( ; ; )
  {
    int payload;

    // start reading event struct
    ia & payload;

    if (ifs.eof())
    {
      break;
    }

    switch (payload)
    {
    case PAYLOAD_EVENT_DEFINITION:
    {
      EventDefinition msg;
      load( ia, msg, m_version );
      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_EVENT_START:
    {
      EventStart msg;
      load( ia, msg, m_version );
      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_EVENT_END:
    {
      EventEnd msg;
      load( ia, msg, m_version );
      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_EVENT_TEXT:
    {
      EventText msg;
      load( ia, msg, m_version );
      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_CONTEXT_DEFINITION:
    {
      ContextDefinition msg;
      load( ia, msg, m_version );
      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_CONTEXT_PUSH:
    {
      ContextPush msg;
      load( ia, msg, m_version );
      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_CONTEXT_POP:
    {
      ContextPop msg;
      load( ia, msg, m_version );
      status = reader.NewEvent(msg);
    }
    break;

    default:
      std::cerr << "Unrecognised payload type: " << payload << std::endl;
      // TBD log error
      break;
    } // end switch

    // non-zero status terminates
    if (status != 0)
    {
      break;
    }

    if ( ! ifs )
    {
      // error
      std::cerr << "Error reading input\n";
      break;
    }

  } // end while

  return (status);
}


} // end namespace
} // end namespace
