/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef RIGHT_TRACK_EVENT_RECORDS_
#define RIGHT_TRACK_EVENT_RECORDS_


#include "RightTrackDefs.h"
#include "Event.h"

namespace RightTrack {
  namespace Internal {

/**
 * This file contains the definitions of event structures as they are
 * streamed to a consumer. The goal is that these structures can be
 * directly written to the consumer over some simple transport or to a
 * file directly, without any martialing overhead.
 *
 * It is understood that some more comprehensive interprocess
 * transports will have to martial these structures.  We just don't
 * want to have to write martialing code if written to a file.
 */



// ----------------------------------------------------------------
/** Event envelope
 *
 * This structure defines the header for all events.  Only use
 * explicit data types to preserve cross platform compatibility.
 */
struct EventEnvelope
{
  uint32_t marker;              // start marker
  uint32_t payload_type;        // type of payload (code)
  uint32_t data_length;         // payload length (in bytes)

  char payload[4];                 // start payload

}; // end class EventEnvelope

//
// Define individual payload codes
//
enum {
  PAYLOAD_EVENT_DEFINITION = 1,
  PAYLOAD_EVENT_START,
  PAYLOAD_EVENT_END,
  PAYLOAD_CONTEXT_DEFINITION,
  PAYLOAD_CONTEXT_PUSH,
  PAYLOAD_CONTEXT_POP,
  PAYLOAD_EVENT_TEXT,
};



// ----------------------------------------------------------------
/** Event creation.
 *
 *
 */

struct EventDefinition
{
  EventTimestamp_t event_time;
  Event::EventType_t event_type; // [bounded, discrete]
  ItemId_t event_id;

  std::string event_name;
  std::string event_group;
  int event_color;
};


struct EventStart
{
  ItemId_t event_id;
  EventTimestamp_t event_time;
  EventPid_t event_pid;
  EventData_t event_data;
};


struct EventEnd
{
  ItemId_t event_id;
  EventTimestamp_t event_time;
  EventPid_t event_pid;
  EventData_t event_data;
};


struct EventText
{
  ItemId_t event_id;
  EventTimestamp_t event_time;
  EventPid_t event_pid;

  std::string event_text;
};


// ----------------------------------------------------------------
struct ContextDefinition
{
  std::string context_name;
  ItemId_t context_id;
};


struct ContextPush
{
  ItemId_t context_id;
  EventTimestamp_t event_time;
};


struct ContextPop
{
  ItemId_t context_id;
  EventTimestamp_t event_time;
};


} // end name space
} // end name space

#endif /* RIGHT_TRACK_EVENT_RECORDS_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
