cmake_minimum_required(VERSION 2.8)

include ( FindBoost )
if(Boost_FOUND)
   include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package( VXL REQUIRED )
include(${VXL_CMAKE_DIR}/UseVXL.cmake)

include_directories( ${VXL_VCL_INCLUDE_DIR} )
include_directories( ${VXL_CORE_INCLUDE_DIR} )


set ( library_sources
      BoundedEvent.h                    BoundedEvent.cxx
      DiscreteEvent.h                   DiscreteEvent.cxx
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

add_subdirectory (gui)

include_directories ( ${CMAKE_CURRENT_SOURCE_DIR} )

add_library ( right_track SHARED ${library_sources} )

target_link_libraries ( right_track
                      vcl vpl vul vsl )


#
# Build test program
#
add_executable (test_right_track test_right_track.cxx)
target_link_libraries (test_right_track
                      right_track vcl vpl vul vsl)


add_executable (test_read_trace test_read_trace.cxx)
target_link_libraries (test_read_trace
                      right_track vcl vpl vul vsl)
