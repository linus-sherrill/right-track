/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "event_database.h"

#include "Model.h"



EventDef::
EventDef()
{ }


EventDef::
~EventDef()
 { }


BoundedEventDef * EventDef::
GetBoundedEvent ()
{
  return 0;
}


DiscreteEventDef * EventDef::
GetDiscreteEvent ()
{
  return 0;
}


// ================================================================

// ----------------------------------------------------------------
/** Create event info string.
 *
 *
 */
wxString BoundedEventDef::
GetEventInfo()
{
  wxString  result;

  result << wxString::Format(wxT("Name: %s\n"), EventName().c_str());
  result << wxString::Format(wxT("Occur Count: %d\n"), m_stats.m_count);
  result << wxString::Format(wxT("Min dur: %f\n"), m_stats.m_minDuration);
  result << wxString::Format(wxT("Max dur: %f\n"), m_stats.m_maxDuration);
  result << wxString::Format(wxT("Avg dir: %f\n"), m_stats.m_avgDuration);
  result << wxString::Format(wxT("Std: %f\n"), m_stats.m_stdDuration);
  result << wxString::Format(wxT("Active%%: %f\n"), m_stats.m_activePct);

  return result;
}


// ----------------------------------------------------------------
/** Find occurrence by time
 *
 *
 */
BaseOccurrence * EventDef::
FindByTime (double time, double delta)
{
  iterator_t ix = m_list.begin();
  iterator_t eix = m_list.end();

  for ( ; ix != eix; ix++)
  {
    if ((*ix)->ContainsTime(time, delta))
    {
      return (*ix).get();
    }
  } // end for

    return 0;
}


// ----------------------------------------------------------------
/**
 *
 *
 */
wxString BoundedOccurrence::
GetInfo()
{
    wxString  result;

    result << wxString::Format(wxT("Duration: %f  (From: %f  To: %f)\n"), (m_endTime - m_startTime),
                               Model::Instance()->TimeOffset(m_startTime),
                               Model::Instance()->TimeOffset(m_endTime) );
    result << wxString::Format(wxT("Start data: %f\n"), m_startData );
    result << wxString::Format(wxT("End data: %f\n"), m_endData );
    result << wxString::Format(wxT("Pid: %d\n"), m_eventPid );

    return result;
}


// ----------------------------------------------------------------
/**
 *
 *
 */
wxString DiscreteOccurrence::
GetInfo()
{
    wxString  result;

    result << wxString::Format(wxT("Time: %f\n"), Model::Instance()->TimeOffset(m_eventTime) );
    result << wxString::Format(wxT("Data: %f\n"), m_eventData );
    result << wxString::Format(wxT("Pid: %d\n"), m_eventPid );

    return result;
}


// ================================================================

wxString DiscreteEventDef::
GetEventInfo()
{
  wxString  result;

  result << wxString::Format(wxT("Name: %s\n"), EventName().c_str());
  result << wxString::Format(wxT("Event Count: %d\n"), m_list.size());

  return result;
}



// ================================================================

bool BoundedOccurrence::
ContainsTime (double time, double delta) const
{
  return (time >= m_startTime) && (time <= m_endTime);
}


bool DiscreteOccurrence::
ContainsTime (double time, double delta) const
{
  return (m_eventTime >= time - delta) && (m_eventTime <= time + delta);
}
