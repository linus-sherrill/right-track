/*ckwg +29
 * Copyright 2010-2011, 2020 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "EventTransportReaderGui.h"
#include <iostream>

EventTransportReaderGui
::EventTransportReaderGui( Model* m, wxString const& origin_name )
  : m_model( m ),
    m_originString( origin_name )
{}

// ----------------------------------------------------------------
/**
 *
 *
 */
int
EventTransportReaderGui
::NewEvent( EventDefinition const& msg )
{
  EventDef::handle_t def;

  switch ( msg.event_type )
  {
    case Event::ET_TEXT_EVENT:
    case Event::ET_DISCRETE_EVENT:
      def = std::make_shared<DiscreteEventDef>();
      break;

    case Event::ET_BOUNDED_EVENT:
      def = std::make_shared<BoundedEventDef>();
      break;

    default:
      // display message;
      wxMessageBox( wxT( "Internal error - unexpected event type" ),
                    wxT( "Error" ), wxICON_ERROR | wxOK );
      return ( 1 );
  } // end switch

  def->m_eventName = wxString( msg.event_name.c_str(), wxConvUTF8 );
  def->m_groupName = wxString( msg.event_group.c_str(), wxConvUTF8 );
  
  // Convert usec to float seconds
  def->m_time = (double) msg.event_time.secs + ( msg.event_time.usecs / 1e6 );
  def->m_eventId = msg.event_id;

  int r = ( msg.event_color >> 16 ) & 0xff;
  int g = ( msg.event_color >>  8 ) & 0xff;
  int b = ( msg.event_color >>  0 ) & 0xff;
  def->m_color = wxColour( r, g, b );

  def->m_originName = m_originString;

  m_model->m_drawOrder.push_back( def->m_eventId );
  m_model->m_eventMap[ def->m_eventId ] = def;

  return ( 0 );
}

// ----------------------------------------------------------------
/**
 *
 *
 */
int
EventTransportReaderGui
::NewEvent( EventStart const& msg )
{
  Model::event_iterator_t ix;
  ix = m_model->m_eventMap.find( msg.event_id );
  if ( ix == m_model->m_eventMap.end() )
  {
    return ( 1 ); // event not found
  }

  // build specific type object
  if ( ix->second->EventType() == Event::ET_DISCRETE_EVENT )
  {
    DiscreteOccurrence* occ = new DiscreteOccurrence();

    occ->m_eventPid = msg.event_pid;
    occ->m_eventData = msg.event_data;
    
    // Convert usec to float seconds
   occ->m_eventTime = (double) msg.event_time.secs + ( msg.event_time.usecs / 1e6 );
    ix->second->m_list.push_back( BaseOccurrence::handle_t( occ ) );
  }
  else
  {
    BoundedOccurrence* occ = new BoundedOccurrence();

    occ->m_eventPid = msg.event_pid;
    occ->m_startData = msg.event_data;
    
    // Convert usec to float seconds
    occ->m_startTime = (double) msg.event_time.secs + ( msg.event_time.usecs / 1e6 );

    ix->second->m_list.push_back( BaseOccurrence::handle_t( occ ) );
  }

  return ( 0 );
}

// ----------------------------------------------------------------
/** Handle text event.
 *
 * Currently this is hacked in as a discrete event with the user
 * comment already filled in.
 */
int
EventTransportReaderGui
::NewEvent( EventText const& msg )
{
  Model::event_iterator_t ix;
  ix = m_model->m_eventMap.find( msg.event_id );
  if ( ix == m_model->m_eventMap.end() )
  {
    return ( 1 ); // event not found
  }

  // build specific type object
  DiscreteOccurrence* occ = new DiscreteOccurrence();

  occ->m_eventPid = msg.event_pid;
  occ->SetUserComment( wxString( msg.event_text.c_str(), wxConvUTF8 ) );
  
  // Convert usec to float seconds
  occ->m_eventTime = (double) msg.event_time.secs + ( msg.event_time.usecs / 1e6 );

  ix->second->m_list.push_back( BaseOccurrence::handle_t( occ ) );

  return ( 0 );
}

// ----------------------------------------------------------------
/**
 *
 *
 */
int
EventTransportReaderGui
::NewEvent( EventEnd const& msg )
{
  Model::event_iterator_t ix;
  ix = m_model->m_eventMap.find( msg.event_id );

  if ( ix == m_model->m_eventMap.end() )
  {
    return ( 1 ); // event not found
  }

  // These are for only bounded events
  // The last occurrence in the list is the start that matches this end.
  BaseOccurrence::handle_t ref = ix->second->m_list.back(); // get last our
                                                            // added occurrence
  BoundedOccurrence* bop = ref->GetBoundedOccurrence();
  assert( bop != 0 );

  // Update that element with ending info
  // Convert usec to float seconds
  bop->m_endTime = (double) msg.event_time.secs + ( msg.event_time.usecs / 1e6 );
  bop->m_endData = msg.event_data;

  return ( 0 );
}

// ----------------------------------------------------------------
/**
 *
 *
 */
int
EventTransportReaderGui
::NewEvent( ContextDefinition const& msg )
{
  EventDef::handle_t def;
  def = std::make_shared<ContextDef>();
  def->m_eventName = wxString( msg.context_name.c_str(), wxConvUTF8 );
  def->m_eventId = msg.context_id;

  m_model->m_drawOrder.push_back( def->m_eventId );
  m_model->m_eventMap[ def->m_eventId ] = def;

  return ( 0 );
}

// ----------------------------------------------------------------
/**
 *
 *
 */
int
EventTransportReaderGui
::NewEvent( ContextPush const& msg )
{
  ContextHistoryElement* ev = new ContextHistoryElement();

  Model::event_iterator_t ix;
  ix = m_model->m_eventMap.find( msg.context_id );
  if ( ix == m_model->m_eventMap.end() )
  {
    return ( 1 ); // event not found
  }

  // Convert usec to float seconds
  ev->m_startTime = (double) msg.event_time.secs + ( msg.event_time.usecs / 1e6 );

  auto event = ix->second->GetContextEvent();
  event->m_list.push_back( BaseOccurrence::handle_t( ev ) );
  event->m_eventStack.push( ev );

  return ( 0 );
}

// ----------------------------------------------------------------

/**
 *
 *
 */
int
EventTransportReaderGui
::NewEvent( ContextPop const& msg )
{
  Model::event_iterator_t ix;
  ix = m_model->m_eventMap.find( msg.context_id );
  if ( ix == m_model->m_eventMap.end() )
  {
    std::cout << "Error in pop for ContextID " << msg.context_id
    << ". Context is not registered.\n";
    return 1 ; // event not found
  }

  // Check the event stack is not empty
  auto event = ix->second->GetContextEvent();

  if ( event->m_eventStack.empty() )
  {
    // This is not a fatal error in that we don't want to invalidate
    // the whole set of events just due to one problem.
    std::cout << "Error in pop for context \"" << event->m_eventName
    << "\". Received a pop and context stack is empty.\n";
    return 1;
  }

  // The element at the top of the stack matches this pop.
  ContextHistoryElement* evo = event->m_eventStack.top();

  // Update that element with ending info. If end time is not zero, then
  // there has already been a pop for this context.
  assert( evo->m_endTime == 0 );

  // Convert usec to float seconds
  evo->m_endTime = (double) msg.event_time.secs + ( msg.event_time.usecs / 1e6 );

  // remove this element since it has been matched.
  event->m_eventStack.pop();

  return ( 0 );
}