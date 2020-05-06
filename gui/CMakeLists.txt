#ckwg +4
# Copyright 2010, 2020 by Kitware, Inc. All Rights Reserved. Please refer to
# KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
# Kitware, Inc., 1712 Route 9, Clifton Park, NY 12065.

find_package ( wxWidgets REQUIRED net gl core base adv )
include ( ${wxWidgets_USE_FILE} )

set (gui_sources
    gen_files/EventCanvas.h             gen_files/EventCanvas.cpp
    gen_files/EventTable.h              gen_files/EventTable.cpp
   #  gen_files/EventViewFrame.h          gen_files/EventViewFrame.cpp
    gen_files/RT_MainFrame.h               gen_files/RT_MainFrame.cpp
    gen_files/TextEditDialog.h          gen_files/TextEditDialog.cpp

    EventTransportReaderGui.h           EventTransportReaderGui.cxx
    main.cpp
    MainFrameApp.h                      MainFrameApp.cpp
    TextEditDialogApp.h                 TextEditDialogApp.cpp
    Model.h                             Model.cpp
    EventSort.h                         EventSort.cpp

    EventCanvasApp.h                    EventCanvasApp.cpp
    TimeLineCursor.h                    TimeLineCursor.cpp
    DisplayableIterator.h               DisplayableIterator.cxx
    EventTableApp.h                     EventTableApp.cpp

    event_database.h                    event_database.cpp
)

add_executable (right_track_gui  ${gui_sources} )
set_target_properties( right_track_gui PROPERTIES
  VERSION ${right_track_VERSION}
)
add_definitions(-DRIGHT_TRACK_VERSION="${right_track_VERSION}")

include_directories ( . .. gen_files )

target_link_libraries ( right_track_gui ${wxWidgets_LIBRARIES} right_track )

set_target_properties (right_track_gui PROPERTIES
      COMPILE_DEFINITIONS    "${wxWidgets_DEFINITIONS};wxWidgets_DEFINITIONS_DEBUG"
      COMPILE_FLAGS          "${wxWidgets_CXX_FLAGS}"
)


#
# install commands
#
install ( TARGETS right_track_gui
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)
