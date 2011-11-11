#ckwg +4
# Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
# KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
# Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.


project ( right_track )

cmake_minimum_required(VERSION 2.8)

set(right_track_VERSION_MAJOR 0)
set(right_track_VERSION_MINOR 4)
set(right_track_VERSION_PATCH 1)
set(right_track_VERSION "${right_track_VERSION_MAJOR}.${right_track_VERSION_MINOR}.${right_track_VERSION_PATCH}")

find_package ( Boost REQUIRED )
if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package( VXL REQUIRED )
include(${VXL_CMAKE_DIR}/UseVXL.cmake)

include_directories( ${VXL_VCL_INCLUDE_DIR} )
include_directories( ${VXL_CORE_INCLUDE_DIR} )

option(BUILD_SHARED_LIBS "Build shared libraries." OFF)

set ( library_sources
      BoundedEvent.h                    BoundedEvent.cxx
      DiscreteEvent.h                   DiscreteEvent.cxx
      TextEvent.h                       TextEvent.cxx
      EventContext.h                    EventContext.cxx
      Event.h                           Event.cxx
      EventManager.h                    EventManager.cxx
      EventRecords.h
      EventTransportFile.h              EventTransportFile.cxx
      EventTransportDebug.h             EventTransportDebug.cxx
      EventTransport.h
      EventTransportReader.h
      EventTransportReaderDebug.h       EventTransportReaderDebug.cxx
      RightTrackDefs.h
      RightTrackItem.h                  RightTrackItem.cxx
      ScopedContext.h
      ScopedEvent.h )

option(GUI_ENABLED "Enable the gui" ON)
if (GUI_ENABLED)
    add_subdirectory (gui)
endif()

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR} )

ADD_LIBRARY ( right_track ${library_sources} )
SET_TARGET_PROPERTIES( right_track PROPERTIES
  VERSION ${right_track_VERSION} SOVERSION ${right_track_VERSION}
)

target_link_libraries ( right_track
                      vcl vpl vul vsl )

#
# Build test program
#
add_executable (test_right_track test_right_track.cxx)
target_link_libraries (test_right_track
                      right_track vcl vpl vul vsl)


add_executable (right_track_dump right_track_dump.cxx)
target_link_libraries (right_track_dump
                      right_track vcl vpl vul vsl)

#
# install commands
#
set ( install_includes
    RightTrackDefs.h RightTrackItem.h
    Event.h BoundedEvent.h DiscreteEvent.h TextEvent.h
    EventContext.h
    ScopedEvent.h ScopedContext.h
)

INSTALL ( TARGETS right_track_dump right_track
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)

INSTALL ( FILES ${install_includes}
        DESTINATION include/right_track
)
