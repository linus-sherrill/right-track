/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <EventTransportFile.h>

#include <EventTransportReader.h>

#include <RightTrackDefs.h>

#include <vcl_string.h>
#include <vsl/vsl_binary_io.h>


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
    m_bstream(0)
{

}


EventTransportFile::
~EventTransportFile()
{
  if (m_outStream)
  {
    m_outStream->close();
  }

  delete m_bstream;
  delete m_outStream;
}


int EventTransportFile::
Write(EventDefinition & msg)
{
  m_bstream->clear_serialisation_records();
  vsl_b_write( *m_bstream, (int) PAYLOAD_EVENT_DEFINITION );
  vsl_b_write( *m_bstream, msg.event_id );
  vsl_b_write( *m_bstream, msg.event_time.secs );
  vsl_b_write( *m_bstream, msg.event_time.usecs );
  vsl_b_write( *m_bstream, (int) msg.event_type );
  vsl_b_write( *m_bstream, msg.event_name );
  vsl_b_write( *m_bstream, msg.event_group );
  vsl_b_write( *m_bstream, msg.event_color );

  return (0);
}


int EventTransportFile::
Write(EventStart & msg)
{
  m_bstream->clear_serialisation_records();
  vsl_b_write( *m_bstream, (int) PAYLOAD_EVENT_START );
  vsl_b_write( *m_bstream, msg.event_id );
  vsl_b_write( *m_bstream, msg.event_time.secs );
  vsl_b_write( *m_bstream, msg.event_time.usecs );
  vsl_b_write( *m_bstream, msg.event_pid );
  vsl_b_write( *m_bstream, msg.event_data );

  return (0);
}


int EventTransportFile::
Write(EventEnd & msg)
{
  m_bstream->clear_serialisation_records();
  vsl_b_write( *m_bstream, (int) PAYLOAD_EVENT_END );
  vsl_b_write( *m_bstream, msg.event_id );
  vsl_b_write( *m_bstream, msg.event_time.secs );
  vsl_b_write( *m_bstream, msg.event_time.usecs );
  vsl_b_write( *m_bstream, msg.event_pid );
  vsl_b_write( *m_bstream, msg.event_data );

  return (0);
}


int EventTransportFile::
Write(EventText & msg)
{
  m_bstream->clear_serialisation_records();
  vsl_b_write( *m_bstream, (int) PAYLOAD_EVENT_TEXT );
  vsl_b_write( *m_bstream, msg.event_id );
  vsl_b_write( *m_bstream, msg.event_time.secs );
  vsl_b_write( *m_bstream, msg.event_time.usecs );
  vsl_b_write( *m_bstream, msg.event_pid );
  vsl_b_write( *m_bstream, msg.event_text );

  return (0);
}


int EventTransportFile::
Write(ContextDefinition & msg)
{
  m_bstream->clear_serialisation_records();
  vsl_b_write( *m_bstream, (int) PAYLOAD_CONTEXT_DEFINITION );
  vsl_b_write( *m_bstream, msg.context_name );
  vsl_b_write( *m_bstream, msg.context_id );

  return (0);
}


int EventTransportFile::
Write(ContextPush & msg)
{
  m_bstream->clear_serialisation_records();
  vsl_b_write( *m_bstream, (int) PAYLOAD_CONTEXT_PUSH );
  vsl_b_write( *m_bstream, msg.context_id );
  vsl_b_write( *m_bstream, msg.event_time.secs );
  vsl_b_write( *m_bstream, msg.event_time.usecs );

  return (0);
}


int EventTransportFile::
Write(ContextPop & msg)
{
  m_bstream->clear_serialisation_records();
  vsl_b_write( *m_bstream, (int) PAYLOAD_CONTEXT_POP );
  vsl_b_write( *m_bstream, msg.context_id );
  vsl_b_write( *m_bstream, msg.event_time.secs );
  vsl_b_write( *m_bstream, msg.event_time.usecs );

  return (0);
}


// ----------------------------------------------------------------
/** Open output file.
 *
 * Filename based on application and PID.
 */
bool EventTransportFile::
OpenFile (vcl_string name)
{
  m_outStream = new vcl_ofstream;
  m_outStream->open(name.c_str());

  m_bstream = new vsl_b_ostream(m_outStream);

  return (true);
}


// ----------------------------------------------------------------
/** Event reader.
 *
 *
 */
int EventTransportFile::
ReadEvents(vcl_string const& resource,
           EventTransportReader & reader)
{
  vsl_b_ifstream bstream( resource.c_str() );
  if (! bstream)
  {
    // log error
    return (1);
  }

  int status(0);

  // loop over all input in stream
  for ( ; ; )
  {
    int payload;

    // start reading event struct
    bstream.clear_serialisation_records();
    vsl_b_read (bstream, payload);

    if (bstream.is().eof())
    {
      break;
    }

    switch (payload)
    {
    case PAYLOAD_EVENT_DEFINITION:
    {
      EventDefinition msg;
      vsl_b_read (bstream, msg.event_id);
      vsl_b_read (bstream, msg.event_time.secs );
      vsl_b_read (bstream, msg.event_time.usecs );

      int ev_type; // need to coerce enum
      vsl_b_read (bstream, ev_type);
      msg.event_type = static_cast< RightTrack::Internal::Event::EventType_t >(ev_type);

      vsl_b_read (bstream, msg.event_name);
      vsl_b_read (bstream, msg.event_group);
      vsl_b_read (bstream, msg.event_color);

      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_EVENT_START:
    {
      EventStart msg;

      vsl_b_read (bstream, msg.event_id);
      vsl_b_read (bstream, msg.event_time.secs );
      vsl_b_read (bstream, msg.event_time.usecs );
      vsl_b_read (bstream, msg.event_pid);
      vsl_b_read (bstream, msg.event_data);

      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_EVENT_END:
    {
      EventEnd msg;

      vsl_b_read (bstream, msg.event_id);
      vsl_b_read (bstream, msg.event_time.secs );
      vsl_b_read (bstream, msg.event_time.usecs );
      vsl_b_read (bstream, msg.event_pid);
      vsl_b_read (bstream, msg.event_data);

      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_EVENT_TEXT:
    {
      EventText msg;

      vsl_b_read (bstream, msg.event_id);
      vsl_b_read (bstream, msg.event_time.secs );
      vsl_b_read (bstream, msg.event_time.usecs );
      vsl_b_read (bstream, msg.event_pid);
      vsl_b_read (bstream, msg.event_text);

      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_CONTEXT_DEFINITION:
    {
      ContextDefinition msg;

      vsl_b_read (bstream, msg.context_name);
      vsl_b_read (bstream, msg.context_id);

      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_CONTEXT_PUSH:
    {
      ContextPush msg;

      vsl_b_read (bstream, msg.context_id);
      vsl_b_read (bstream, msg.event_time.secs );
      vsl_b_read (bstream, msg.event_time.usecs );

      status = reader.NewEvent(msg);
    }
    break;

    case PAYLOAD_CONTEXT_POP:
    {
      ContextPop msg;

      vsl_b_read (bstream, msg.context_id);
      vsl_b_read (bstream, msg.event_time.secs );
      vsl_b_read (bstream, msg.event_time.usecs );

      status = reader.NewEvent(msg);
    }
    break;

    default:
      vcl_cerr << "Unrecognised payload type: " << payload << vcl_endl;
      // TBD log error
      break;
    } // end switch

    // non-zero status terminates
    if (status != 0)
    {
      break;
    }

    if (! bstream)
    {
      // error
      vcl_cerr << "Error reading input\n";
      break;
    }

  } // end while

  return (status);
}


} // end namespace
} // end namespace
