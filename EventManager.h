/*ckwg +5
 * Copyright 2010-2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_EVENT_MANAGER_
#define _RIGHT_TRACK_EVENT_MANAGER_

#include "RightTrackDefs.h"

#include <string>
#include <mutex>

namespace RightTrack {

// Partial types
class EventContext;

namespace Internal {

// Partial types
class Event;
class EventTransport;


// ----------------------------------------------------------------
/** Event Manager.
 *
 * - manages events for a single address space (process)
 *
 */
class EventManager
{
public:
  static EventManager * Instance();
  ~EventManager();

  void Configure(int argc, char * argv[]);

  ItemId_t GetNextId();
  void RegisterEvent(Event * ev);
  void StartEvent (Event * ev, std::string const& val);
  void StartEvent (Event * ev, ::RightTrack::EventData_t const& val);
  void EndEvent (Event * ev, ::RightTrack::EventData_t const& val);

  void RegisterContext (EventContext * ec);
  void PushContext (EventContext * ec);
  void PopContext (EventContext * ec);

  std::string const& GetFilename() const;


private:
  EventManager(); // CTOR
  EventManager( EventManager const& ) = delete;
  EventManager& operator=( EventManager const& ) = delete;

   EventTimestamp_t CurrentTimestamp();

  EventPid_t GetPid();

  std::mutex m_lock;

  std::string m_systemName;
  std::string m_filename;

  ItemId_t m_nextId;

  // pointer to output transport object.
  EventTransport * m_transport;
  // Transport object must be able to read what it writes.

  static EventManager * s_instance;
}; // end class EventManager

} // end namespace
} // end namespace

#endif /* _RIGHT_TRACK_EVENT_MANAGER_ */
