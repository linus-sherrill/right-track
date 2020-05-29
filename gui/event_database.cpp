/*ckwg +5
 * Copyright 2011, 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 1712 Route 9, Clifton Park, NY 12065.
 */

#include "event_database.h"

#include "Model.h"

// -----------------------------------------------------
BoundedEventDef*
EventDef
::GetBoundedEvent()
{
  return nullptr;
}

DiscreteEventDef*
EventDef
::GetDiscreteEvent()
{
  return nullptr;
}

ContextDef*
EventDef
::GetContextEvent()
{
  return nullptr;
}

// ----------------------------------------------------------------
/** Create event info string.
 *
 *
 */
wxString
BoundedEventDef
::GetEventInfo()
{
  wxString result;

  result << wxString::Format( wxT( "Name: %s\n" ), EventName().c_str() );
  result << wxString::Format( wxT( "Occur Count: %d\n" ), m_stats.m_count );
  result << wxString::Format( wxT( "Min dur: %f\n" ), m_stats.m_minDuration );
  result << wxString::Format( wxT( "Max dur: %f\n" ), m_stats.m_maxDuration );
  result << wxString::Format( wxT( "Avg dir: %f\n" ), m_stats.m_avgDuration );
  result << wxString::Format( wxT( "Std: %f\n" ), m_stats.m_stdDuration );
  result << wxString::Format( wxT( "Active%%: %f\n" ), m_stats.m_activePct );
//+ add group name if there is one?

  return result;
}

// ----------------------------------------------------------------
/** Find occurrence by time
 *
 *
 */
BaseOccurrence*
EventDef
::FindByTime( double time, double delta )
{
  iterator_t ix = m_list.begin();
  iterator_t eix = m_list.end();

  for (; ix != eix; ix++ )
  {
    if ( ( *ix )->ContainsTime( time, delta ) )
    {
      return ( *ix ).get();
    }
  } // end for

  return 0;
}

// ----------------------------------------------------------------
/**
 *
 *
 */
wxString
BoundedOccurrence
::GetInfo()
{
  wxString result;

  result << wxT( "------\n" );
  result << wxString::Format( wxT("Duration: %f  (From: %f  To: %f)\n" ),
                             ( m_endTime - m_startTime ),
                              Model::Instance()->TimeOffset( m_startTime ),
                              Model::Instance()->TimeOffset( m_endTime ) );
  if ( !m_startData.empty() )
  {
    result << wxString::Format( wxT( "Start data: %f\n" ), m_startData[ 0 ] );

    for ( unsigned i = 1; i < m_startData.size(); ++i )
    {
      result << wxString::Format( wxT( ", %f" ), m_startData[ i ] );
    }
  }

  if ( !m_endData.empty() )
  {
    result << wxString::Format( wxT( "End data: %f\n" ), m_endData[ 0 ] );

    for ( unsigned i = 1; i < m_endData.size(); ++i )
    {
      result << wxString::Format( wxT( ", %f" ), m_endData[ i ] );
    }
  }

  result << wxString::Format( wxT( "Pid: %d\n" ), m_eventPid );

  return result;
}

// ----------------------------------------------------------------
/**
 *
 *
 */
wxString
DiscreteOccurrence
::GetInfo()
{
  wxString result;

  result << wxT( "------\n" );
  result << wxString::Format( wxT( "Time: %f\n" ), Model::Instance()->TimeOffset( m_eventTime ) );

  if ( !m_eventData.empty() )
  {
    result << wxString::Format( wxT( "Data: %f\n" ), m_eventData[ 0 ] );

    for ( unsigned i = 1; i < m_eventData.size(); ++i )
    {
      result << wxString::Format( wxT( ", %f" ), m_eventData[ i ] );
    }
  }
  result << wxString::Format( wxT( "Pid: %d\n" ), m_eventPid );

  return result;
}

// ================================================================
wxString
DiscreteEventDef
::GetEventInfo()
{
  wxString result;

  result << wxString::Format( wxT( "Name: %s\n" ), EventName().c_str() );
  result << wxString::Format( wxT( "Event Count: %d\n" ), static_cast<int>(m_list.size()) );

  return result;
}

// ================================================================
wxString
ContextDef
::GetEventInfo()
{
  wxString result;

  result << wxString::Format( wxT( "Name: %s\n" ), EventName().c_str() );
  result << wxString::Format( wxT( "Event Count: %d\n" ), static_cast<int>(m_list.size()) );

  return result;
}

wxString
ContextHistoryElement
::GetInfo()
{
  wxString result;

  result << wxT( "------\n" );
  result << wxString::Format( wxT("Duration: %f  (From: %f  To: %f)\n" ),
                             ( m_endTime - m_startTime ),
                             Model::Instance()->TimeOffset( m_startTime ),
                             Model::Instance()->TimeOffset( m_endTime ) );

  result << wxString::Format( wxT( "Pid: %d\n" ), m_eventPid );

  return result;
}



// ================================================================
bool
BoundedOccurrence
::ContainsTime( double time, double delta ) const
{
  return ( time >= m_startTime ) && ( time <= m_endTime );
}

bool
DiscreteOccurrence
::ContainsTime( double time, double delta ) const
{
  return ( m_eventTime >= time - delta ) && ( m_eventTime <= time + delta );
}

bool
ContextHistoryElement
::ContainsTime( double time, double delta ) const
{
  return ( time >= m_startTime ) && ( time <= m_endTime );
}
