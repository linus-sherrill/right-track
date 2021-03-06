/*ckwg +5
 * Copyright 2010-2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include "EventTransportReaderGui.h"
#include <iostream>


EventTransportReaderGui::
EventTransportReaderGui(Model * m)
  : m_model(m)
{

}


EventTransportReaderGui::
~EventTransportReaderGui()
{

}



// ----------------------------------------------------------------
/**
 *
 *
 */
int EventTransportReaderGui::
NewEvent(EventDefinition const& msg)
{
  EventDef * def;

  switch (msg.event_type)
  {
  case Event::ET_TEXT_EVENT:
  case Event::ET_DISCRETE_EVENT:
    def = new DiscreteEventDef();
    break;

  case Event::ET_BOUNDED_EVENT:
    def = new BoundedEventDef();
    break;

  default:
    // display message;
    wxMessageBox( wxT("Internal error - unexpected event type"),
                  wxT("Error"), wxICON_ERROR | wxOK);
  } // end switch

  def->m_eventName = wxString (msg.event_name.c_str(), wxConvUTF8);
  def->m_groupName = wxString (msg.event_group.c_str(), wxConvUTF8);
  def->m_time = (double) msg.event_time.secs + (msg.event_time.usecs / 1e6); // convert usec to float seconds
  def->m_eventId = msg.event_id;
  def->m_color = msg.event_color;


  m_model->m_drawOrder.push_back(def->m_eventId);
  m_model->m_eventMap[def->m_eventId] = EventDef::handle_t(def);

  return (0);
}


// ----------------------------------------------------------------
/**
 *
 *
 */
int EventTransportReaderGui::
NewEvent(EventStart const& msg)
{
  Model::event_iterator_t ix;
  ix = m_model->m_eventMap.find (msg.event_id);
  if (ix == m_model->m_eventMap.end())
  {
    return (1); // event not found
  }

  // build specific type object
  if (ix->second->EventType() == Event::ET_DISCRETE_EVENT)
  {
    DiscreteOccurrence * occ = new DiscreteOccurrence();

    occ->m_eventPid = msg.event_pid;
    occ->m_eventData = msg.event_data;
    occ->m_eventTime = (double) msg.event_time.secs + (msg.event_time.usecs / 1e6); // convert usec to float seconds

    ix->second->m_list.push_back (BaseOccurrence::handle_t(occ) );
  }
  else
  {
    BoundedOccurrence * occ = new BoundedOccurrence();

    occ->m_eventPid = msg.event_pid;
    occ->m_startData = msg.event_data;
    occ->m_startTime = (double) msg.event_time.secs + (msg.event_time.usecs / 1e6); // convert usec to float seconds

    ix->second->m_list.push_back (BaseOccurrence::handle_t(occ) );
  }

  return (0);
}


// ----------------------------------------------------------------
/** Handle text event.
 *
 * Currently this is hacked in as a discrete event with the user
 * comment already filled in.
 */
int EventTransportReaderGui::
NewEvent(EventText const& msg)
{
  Model::event_iterator_t ix;
  ix = m_model->m_eventMap.find (msg.event_id);
  if (ix == m_model->m_eventMap.end())
  {
    return (1); // event not found
  }

  // build specific type object
  DiscreteOccurrence * occ = new DiscreteOccurrence();

  occ->m_eventPid = msg.event_pid;
  occ->m_eventData = 0;
  occ->SetUserComment( wxString(msg.event_text.c_str(), wxConvUTF8) );
  occ->m_eventTime = (double) msg.event_time.secs + (msg.event_time.usecs / 1e6); // convert usec to float seconds

  ix->second->m_list.push_back (BaseOccurrence::handle_t(occ) );

  return (0);
}


// ----------------------------------------------------------------
/**
 *
 *
 */
int EventTransportReaderGui::
NewEvent(EventEnd const& msg)
{
  Model::event_iterator_t ix;
  ix = m_model->m_eventMap.find (msg.event_id);
  if (ix == m_model->m_eventMap.end())
  {
    return (1); // event not found
  }

  // These are for only bounded events
  // The last occurrence in the list is the start that matches this end.
  BaseOccurrence::handle_t ref = ix->second->m_list.back(); // get last our added occurrence
  BoundedOccurrence * bop = ref->GetBoundedOccurrence();
  assert (bop != 0);

  // Update that element with ending info
  bop->m_endTime = (double) msg.event_time.secs + (msg.event_time.usecs / 1e6); // convert usec to float seconds
  bop->m_endData = msg.event_data;

  return (0);
}


// ----------------------------------------------------------------
/**
 *
 *
 */
int EventTransportReaderGui::
NewEvent(ContextDefinition const& msg)
{
  ContextDef ev;
  ev.ctxt_def = msg;

  // TBD
  return (0);
}


// ----------------------------------------------------------------
/**
 *
 *
 */
int EventTransportReaderGui::
NewEvent(ContextPush const& msg)
{
  ContextHistoryElement ev;
  ev.m_startTime = (double) msg.event_time.secs + (msg.event_time.usecs / 1e6); // convert usec to float seconds
 // TBD

  return (0);
}


// ----------------------------------------------------------------
/**
 *
 *
 */
int EventTransportReaderGui::
NewEvent(ContextPop const& msg)
{
  ContextHistoryElement ev;
  ev.m_endTime = (double) msg.event_time.secs + (msg.event_time.usecs / 1e6); // convert usec to float seconds
  // TBD

  return (0);
}


