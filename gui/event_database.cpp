/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "event_database.h"

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
  result << wxString::Format(wxT("Event Count: %d\n"), m_stats.m_count);
  result << wxString::Format(wxT("Min dur: %f\n"), m_stats.m_minDuration);
  result << wxString::Format(wxT("Max dur: %f\n"), m_stats.m_maxDuration);
  result << wxString::Format(wxT("Avg dir: %f\n"), m_stats.m_avgDuration);
  result << wxString::Format(wxT("std: %f"), m_stats.m_stdDuration);

  return result;
}



// ----------------------------------------------------------------
/** Find occurrence by time
 *
 *
 */
BoundedOccurrence const* BoundedEventDef::
FindByTime (double time)
{
  iterator_t ix = m_list.begin();
  iterator_t eix = m_list.end();

  for ( ; ix != eix; ix++)
  {
    if (ix->ContainsTime(time))
    {
      return &(*ix);
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

    result << wxString::Format(wxT("Duration: %f\n"), (m_endTime - m_startTime) );
    result << wxString::Format(wxT("Start data: %f\n"), m_startData );
    result << wxString::Format(wxT("End data: %f\n"), m_endData );

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

    result << wxString::Format(wxT("Data: %f\n"), m_eventData );

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

