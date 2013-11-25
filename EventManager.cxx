/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <EventManager.h>

#include <BoundedEvent.h>
#include <DiscreteEvent.h>
#include <EventContext.h>
#include <EventTransportFile.h>
#include <EventTransportDebug.h>

#include <vul/vul_get_timestamp.h>
#include <sstream>
#include <vpl/vpl.h>


namespace RightTrack {
namespace Internal {

EventManager * EventManager::s_instance = 0;


// ----------------------------------------------------------------
/** At exit handler.
 *
 * Should really use an object lifetime manager.
 */
void EventManagerAtExitHandler()
{
  delete EventManager::Instance();
}


// ----------------------------------------------------------------
/** Constructor.
 *
 *
 */
EventManager::
EventManager()
  : m_nextId(1),  // initial item number
    m_transport(0)
{

  // Internal::EventTransportDebug * trans = new Internal::EventTransportDebug();
  Internal::EventTransportFile * trans = new Internal::EventTransportFile();

  // Create file name
  std::stringstream filename;
  filename << "RightTrack_" << vpl_getpid() << ".rtrk";
  m_filename = filename.str();
  trans->OpenFile(m_filename.c_str());

  m_transport = trans;

  // register our at-exit handler so we can be cleaned up.
  atexit (EventManagerAtExitHandler);
}


EventManager::
~EventManager()
{
  delete m_transport;
  s_instance = 0;
}


// ----------------------------------------------------------------
/** Singleton instance method.
 *
 *
 */
EventManager * EventManager::
Instance()
{
  static boost::mutex local_lock;

  if (0 == s_instance)
  {
    boost::mutex::scoped_lock lock(local_lock);
    if (0 == s_instance)
    {
      s_instance = new EventManager();
    }
  }

  return (s_instance);
}


// ----------------------------------------------------------------
/** Configure this manager.
 *
 * This method configures this event manager.
 * Output method (to file, to stream, ...)
 * output file name.  This may be better handled by a delegate object
 * that is created by the application (or use the default)
 */
void EventManager::
Configure(int argc, char * argv[])
{

/* Design discussion:

Things we have to configure:
- app name
- app instance name
- output stream for events (file name?)

 */

  // Get our host name
  char buffer[1024];
  gethostname (buffer, sizeof buffer);
  m_systemName = buffer;


}


// ----------------------------------------------------------------
/** Get next item id.
 *
 *
 */
ItemId_t EventManager::
GetNextId()
{
  boost::mutex::scoped_lock lock(m_lock);

  ItemId_t id = m_nextId++;

  return id;
}


// ----------------------------------------------------------------
/** Get current timestamp.
 *
 * This method returns the current time stamp in microseconds.
 *
 * The base of this time stamp does not really matter as long as all
 * processes use the same origin. This easier to deal with on a single
 * system, but this \e must work in a distributed environment.
 */
EventTimestamp_t EventManager::
CurrentTimestamp()
{
  int secs, msecs;

  vul_get_timestamp( secs, msecs);

  EventTimestamp_t retval(secs, msecs * 1000);
  return retval;
}


// ----------------------------------------------------------------
/** Get process id
 *
 *
 */
EventPid_t EventManager::
GetPid()
{
  return pthread_self();
  // return vpl_getpid();
}


// ----------------------------------------------------------------
/** Get filename
 *
 *
 */
std::string const & EventManager::
GetFilename() const
{
  return m_filename;
}


// ----------------------------------------------------------------
/** Register new event.
 *
 *
 */
void EventManager::
RegisterEvent(Event * ev)
{
  boost::mutex::scoped_lock lock(m_lock);

  // create event definition
  EventDefinition msg;
  msg.event_id = ev->ItemId();
  msg.event_name = ev->ItemName();

  msg.event_type = ev->EventType();
  msg.event_group = ev->EventGroup();
  msg.event_color = ev->EventColor();

  msg.event_time = CurrentTimestamp();

  m_transport->Write (msg);
}


// ----------------------------------------------------------------
/** Create a start event message.
 *
 *
 */
void EventManager::
StartEvent (Event * ev, ::RightTrack::EventData_t val)
{
  boost::mutex::scoped_lock lock(m_lock);

  // create a start event message
  EventStart msg;
  msg.event_id = ev->ItemId();
  msg.event_time = CurrentTimestamp();
  msg.event_pid = GetPid();
  msg.event_data = val;

  m_transport->Write (msg);
}


// ----------------------------------------------------------------
/** Create a text event message.
 *
 *
 */
void EventManager::
StartEvent (Event * ev, std::string const& val)
{
  boost::mutex::scoped_lock lock(m_lock);

  // create a start event message
  EventText msg;
  msg.event_id = ev->ItemId();
  msg.event_time = CurrentTimestamp();
  msg.event_pid = GetPid();
  msg.event_text = val;

  m_transport->Write (msg);
}


// ----------------------------------------------------------------
/** Generate end event message.
 *
 *
 */
void EventManager::
EndEvent (Event * ev, ::RightTrack::EventData_t val)
{
  boost::mutex::scoped_lock lock(m_lock);

  // create  event message
  EventEnd msg;
  msg.event_id = ev->ItemId();
  msg.event_time = CurrentTimestamp();
  msg.event_pid = GetPid();
  msg.event_data = val;

  m_transport->Write (msg);
}


void EventManager::
RegisterContext (EventContext * ec)
{
  boost::mutex::scoped_lock lock(m_lock);

  // create event definition
 ContextDefinition msg;
  msg.context_id = ec->ItemId();
  msg.context_name = ec->ItemName();

  m_transport->Write (msg);
}


void EventManager::
PushContext (EventContext * ec)
{
  boost::mutex::scoped_lock lock(m_lock);
  ContextPush msg;

  msg.context_id = ec->ItemId();
  msg.event_time = CurrentTimestamp();

  m_transport->Write (msg);
}


void EventManager::
PopContext (EventContext * ec)
{
  boost::mutex::scoped_lock lock(m_lock);
  ContextPop msg;

  msg.context_id = ec->ItemId();
  msg.event_time = CurrentTimestamp();

  m_transport->Write (msg);
}


} // end namespace
} // end namespace

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
