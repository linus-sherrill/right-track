/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include "EventTransportReaderGui.h"


EventTransportReaderGui::
EventTransportReaderGui(Model * m)
  : m_model(m)
{

}


EventTransportReaderGui::
~EventTransportReaderGui()
{

}



int EventTransportReaderGui::
NewEvent(EventDefinition const& msg)
{

  EventHistory_t ev;
  ev.ev_def = msg;

  m_model->m_drawOrder.push_back(msg.event_id);
  m_model->m_eventMap[msg.event_id] = ev;

  return (0);
}


int EventTransportReaderGui::
NewEvent(EventStart const& msg)
{
  EventHistoryElement_t ev;
  ev.ev_action = EventHistoryElement_t::START;
  ev.event_time = (double) msg.event_time / 1e6; // convert usec to float seconds
  ev.event_pid = msg.event_pid;
  ev.event_data = msg.event_data;

  Model::event_iterator_t ix;

  ix = m_model->m_eventMap.find (msg.event_id);
  if (ix == m_model->m_eventMap.end())
  {
    return (1); // event not found
  }

  ix->second.EventHistory.push_back (ev);

  return (0);
}


int EventTransportReaderGui::
NewEvent(EventEnd const& msg)
{
  EventHistoryElement_t ev;
  ev.ev_action = EventHistoryElement_t::END;
  ev.event_time = (double) msg.event_time / 1e6; // convert usec to float seconds
  ev.event_pid = msg.event_pid;
  ev.event_data = msg.event_data;

  Model::event_iterator_t ix;

  ix = m_model->m_eventMap.find (msg.event_id);
  if (ix == m_model->m_eventMap.end())
  {
    return (1); // event not found
  }

  ix->second.EventHistory.push_back (ev);

  return (0);
}


int EventTransportReaderGui::
NewEvent(ContextDefinition const& msg)
{
  ContextDef_t ev;
  ev.ctxt_def = msg;

  // TBD
  return (0);
}


int EventTransportReaderGui::
NewEvent(ContextPush const& msg)
{
  ContextHistoryElement_t ev;
  ev.event_time = (double) msg.event_time / 1e6;
 // TBD

  return (0);
}


int EventTransportReaderGui::
NewEvent(ContextPop const& msg)
{
  ContextHistoryElement_t ev;
  ev.event_time = (double) msg.event_time / 1e6;
  // TBD

  return (0);
}


