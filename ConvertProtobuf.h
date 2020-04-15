/*ckwg +5
 * Copyright 2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef RIGHT_TRACK_CONVERTPROTOBUF_H
#define RIGHT_TRACK_CONVERTPROTOBUF_H

#include <EventRecords.h>
#include <EventTransportProtobuf.pb.h>


namespace RightTrack {
namespace Internal {

void ConvertProtobuf( const ::RightTrack::Internal::EventDefinition& msg,
                      ::RightTrack::proto::EventEnvelope& env );

void ConvertProtobuf( const ::RightTrack::Internal::EventStart& msg,
                      ::RightTrack::proto::EventEnvelope& env );

void ConvertProtobuf( const ::RightTrack::Internal::EventEnd& msg,
                      ::RightTrack::proto::EventEnvelope& env );

void ConvertProtobuf( const ::RightTrack::Internal::EventText& msg,
                      ::RightTrack::proto::EventEnvelope& env );

void ConvertProtobuf( const ::RightTrack::Internal::ContextDefinition& msg,
                      ::RightTrack::proto::EventEnvelope& env );

void ConvertProtobuf( const ::RightTrack::Internal::ContextPush& msg,
                      ::RightTrack::proto::EventEnvelope& env );

void ConvertProtobuf( const ::RightTrack::Internal::ContextPop& msg,
                      ::RightTrack::proto::EventEnvelope& env );


void ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                      ::RightTrack::Internal::EventDefinition& msg);

void ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                      ::RightTrack::Internal::EventStart& msg);

void ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                      ::RightTrack::Internal::EventEnd& msg);

void ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                      ::RightTrack::Internal::EventText& msg);

void ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                      ::RightTrack::Internal::ContextDefinition& msg);

void ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                      ::RightTrack::Internal::ContextPush& msg);

void ConvertProtobuf( const ::RightTrack::proto::EventEnvelope& env,
                      ::RightTrack::Internal::ContextPop& msg);

} } // end namespace

#endif /* RIGHT_TRACK_CONVERTPROTOBUF_H */
