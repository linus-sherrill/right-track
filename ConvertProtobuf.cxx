/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "ConvertProtobuf.h"

#include "EventTransport.h"

namespace RightTrack {
namespace Internal {

using namespace ::RightTrack::proto;

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::Internal::EventDefinition& msg,
                 ::RightTrack::proto::EventEnvelope& env )
{
  env.set_type( EventEnvelope_Type_EVENT_DEFINITION );

  auto def = env.mutable_event_definition();

  auto ts = def->mutable_event_time();
  ts->set_secs( msg.event_time.secs );
  ts->set_usecs( msg.event_time.usecs );

  def->set_event_type( msg.event_type );
  def->set_event_id( msg.event_id );
  def->set_event_name( msg.event_name );
  def->set_event_group( msg.event_group );
  def->set_event_color( msg.event_color );
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::Internal::EventStart& msg,
                 ::RightTrack::proto::EventEnvelope& env )
{
  env.set_type( EventEnvelope_Type_EVENT_START );

  auto def = env.mutable_event_start_end();

  auto ts = def->mutable_event_time();
  ts->set_secs( msg.event_time.secs );
  ts->set_usecs( msg.event_time.usecs );

  def->set_event_id( msg.event_id );
  def->set_event_pid( msg.event_pid );

  for ( auto v : msg.event_data )
  {
    def->add_event_data( v );
  }
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::Internal::EventEnd& msg,
                 ::RightTrack::proto::EventEnvelope& env )
{
  env.set_type( EventEnvelope_Type_EVENT_END );

  auto def = env.mutable_event_start_end();

  auto ts = def->mutable_event_time();
  ts->set_secs( msg.event_time.secs );
  ts->set_usecs( msg.event_time.usecs );

  def->set_event_id( msg.event_id );
  def->set_event_pid( msg.event_pid );

  for ( auto v : msg.event_data )
  {
    def->add_event_data( v );
  }
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::Internal::EventText& msg,
                 ::RightTrack::proto::EventEnvelope& env )
{
  env.set_type( EventEnvelope_Type_EVENT_TEXT );

  auto def = env.mutable_event_text();

  auto ts = def->mutable_event_time();
  ts->set_secs( msg.event_time.secs );
  ts->set_usecs( msg.event_time.usecs );

  def->set_event_id( msg.event_id );
  def->set_event_pid( msg.event_pid );
  def->set_event_text( msg.event_text );
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::Internal::ContextDefinition& msg,
                 ::RightTrack::proto::EventEnvelope& env )
{
  env.set_type( EventEnvelope_Type_CONTEXT_DEFINITION );

  auto def = env.mutable_context_def();

  def->set_context_name( msg.context_name );
  def->set_context_id( msg.context_id );
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::Internal::ContextPush& msg,
                 ::RightTrack::proto::EventEnvelope& env )
{
  env.set_type( EventEnvelope_Type_CONTEXT_PUSH );

  auto def = env.mutable_context_push_pop();

  auto ts = def->mutable_event_time();
  ts->set_secs( msg.event_time.secs );
  ts->set_usecs( msg.event_time.usecs );

  def->set_context_id( msg.context_id );
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::Internal::ContextPop& msg,
                 ::RightTrack::proto::EventEnvelope& env )
{
  env.set_type( EventEnvelope_Type_CONTEXT_POP );

  auto def = env.mutable_context_push_pop();

  auto ts = def->mutable_event_time();
  ts->set_secs( msg.event_time.secs );
  ts->set_usecs( msg.event_time.usecs );

  def->set_context_id( msg.context_id );
}

// ============================================================================
void
ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                 ::RightTrack::Internal::EventDefinition& msg )
{
  const auto& proto = env.event_definition();

  const auto& ts = proto.event_time();
  msg.event_time.secs = ts.secs();
  msg.event_time.usecs = ts.usecs();

  msg.event_type =
    static_cast< ::RightTrack::Internal::Event::EventType_t >( proto.
                                                               event_type() );
  msg.event_id = proto.event_id();
  msg.event_name = proto.event_name();
  msg.event_group = proto.event_group();
  msg.event_color = proto.event_color();
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                 ::RightTrack::Internal::EventStart& msg )
{
  const auto& proto = env.event_start_end();

  const auto& ts = proto.event_time();
  msg.event_time.secs = ts.secs();
  msg.event_time.usecs = ts.usecs();

  msg.event_id = proto.event_id();
  msg.event_pid = proto.event_pid();

  const size_t count( proto.event_data_size() );
  for ( size_t i = 0; i < count; ++i )
  {
    msg.event_data.push_back( proto.event_data( i ) );
  }
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                 ::RightTrack::Internal::EventEnd& msg )
{
  const auto& proto = env.event_start_end();

  const auto& ts = proto.event_time();
  msg.event_time.secs = ts.secs();
  msg.event_time.usecs = ts.usecs();

  msg.event_id = proto.event_id();
  msg.event_pid = proto.event_pid();

  const size_t count( proto.event_data_size() );
  for ( size_t i = 0; i < count; ++i )
  {
    msg.event_data.push_back( proto.event_data( i ) );
  }
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                 ::RightTrack::Internal::EventText& msg )
{
  const auto& proto = env.event_text();

  const auto& ts = proto.event_time();
  msg.event_time.secs = ts.secs();
  msg.event_time.usecs = ts.usecs();

  msg.event_id = proto.event_id();
  msg.event_pid = proto.event_pid();
  msg.event_text = proto.event_text();
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                 ::RightTrack::Internal::ContextDefinition& msg )
{
  const auto& proto = env.context_def();

  msg.context_name = proto.context_name();
  msg.context_id = proto.context_id();
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                 ::RightTrack::Internal::ContextPush& msg )
{
  const auto& proto = env.context_push_pop();

  const auto& ts = proto.event_time();
  msg.event_time.secs = ts.secs();
  msg.event_time.usecs = ts.usecs();

  msg.context_id = proto.context_id();
}

// ----------------------------------------------------------------------------
void
ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                 ::RightTrack::Internal::ContextPop& msg )
{
  const auto& proto = env.context_push_pop();

  const auto& ts = proto.event_time();
  msg.event_time.secs = ts.secs();
  msg.event_time.usecs = ts.usecs();

  msg.context_id = proto.context_id();
}

} // namespace Internal
} // namespace RightTrack
