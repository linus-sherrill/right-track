#ckwg +4
# Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
# KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
# Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.


project ( right_track )

cmake_minimum_required(VERSION 3.8)

set(right_track_VERSION_MAJOR 0)
set(right_track_VERSION_MINOR 5)
set(right_track_VERSION_PATCH 0)

set(right_track_VERSION "${right_track_VERSION_MAJOR}.${right_track_VERSION_MINOR}.${right_track_VERSION_PATCH}")

find_package( Protobuf REQUIRED )

set( CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++11" )

set( sources
  BoundedEvent.cxx
  ConvertProtobuf.cxx
  DiscreteEvent.cxx
  Event.cxx
  EventContext.cxx
  EventManager.cxx
  EventTransportDebug.cxx
  EventTransportProtoFile.cxx
  EventTransportReaderDebug.cxx
  RightTrackItem.cxx
  TextEvent.cxx
  )

set( public_includes
  BoundedEvent.h
  DiscreteEvent.h
  Event.h
  EventContext.h
  RightTrackDefs.h
  RightTrackItem.h
  ScopedContext.h
  ScopedEvent.h
  TextEvent.h
  )

set( private_includes
  ConvertProtobuf.h
  EventManager.h
  EventRecords.h
  EventTransport.h
  EventTransportDebug.h
  EventTransportProtoFile.cxx
  EventTransportReader.h
  EventTransportReaderDebug.h
  )


option(BUILD_SHARED_LIBS "Build shared libraries." OFF)

option(GUI_ENABLED "Enable the gui" ON)
if (GUI_ENABLED)
    add_subdirectory (gui)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${PROTOBUF_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

PROTOBUF_GENERATE_CPP( cpp_srcs cpp_hdrs
  EventTransportProtobuf.proto )


ADD_LIBRARY ( right_track
  ${sources}
  ${public_includes}
  ${private_includes}
  ${cpp_hdrs} ${cpp_srcs}
  )

SET_TARGET_PROPERTIES( right_track
  PROPERTIES
    VERSION ${right_track_VERSION}
    SOVERSION ${right_track_VERSION}
)

target_link_libraries ( right_track
  PUBLIC   ${PROTOBUF_LIBRARIES}
  )

###
# Build test program
#
add_executable (test_right_track test_right_track.cxx)
target_link_libraries (test_right_track
                      right_track)


add_executable (right_track_dump right_track_dump.cxx)
target_link_libraries (right_track_dump
                      right_track)

###
# install commands
#
INSTALL ( TARGETS right_track_dump right_track
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

INSTALL ( FILES ${public_includes}
        DESTINATION include/right_track
)

include(CPack)
