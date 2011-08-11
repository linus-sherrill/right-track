FIND_PACKAGE ( wxWidgets REQUIRED net gl core base adv )
INCLUDE ( ${wxWidgets_USE_FILE} )

SET (gui_sources
    gen_files/EventViewFrame.h          gen_files/EventViewFrame.cpp
    gen_files/MainFrame.h               gen_files/MainFrame.cpp
    gen_files/EventCanvas.h             gen_files/EventCanvas.cpp
    gen_files/TextEditDialog.h          gen_files/TextEditDialog.cpp

    EventTransportReaderGui.h           EventTransportReaderGui.cxx
    main.cpp
    MainFrameApp.h                      MainFrameApp.cpp
    TextEditDialogApp.h                 TextEditDialogApp.cpp
    Model.h                             Model.cpp
    EventCanvasApp.h                    EventCanvasApp.cpp
    TimeLineCursor.h                    TimeLineCursor.cpp
    event_database.h                    event_database.cpp
    DisplayableIterator.h               DisplayableIterator.cxx
)

ADD_EXECUTABLE (right_track_gui  ${gui_sources} )
SET_TARGET_PROPERTIES( right_track_gui PROPERTIES
  VERSION ${right_track_VERSION}
)
add_definitions(-DRIGHT_TRACK_VERSION="${right_track_VERSION}")

INCLUDE_DIRECTORIES ( . .. gen_files )

TARGET_LINK_LIBRARIES ( right_track_gui ${wxWidgets_LIBRARIES} right_track )

SET_TARGET_PROPERTIES (right_track_gui PROPERTIES
      COMPILE_DEFINITIONS    "${wxWidgets_DEFINITIONS};wxWidgets_DEFINITIONS_DEBUG"
      COMPILE_FLAGS          "${wxWidgets_CXX_FLAGS}"
)


#
# install commands
#
INSTALL ( TARGETS right_track_gui
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)
