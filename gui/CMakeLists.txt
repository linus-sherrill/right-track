FIND_PACKAGE ( wxWidgets REQUIRED net gl core base )
INCLUDE ( ${wxWidgets_USE_FILE} )

SET (gui_sources
    gen_files/EventViewFrame.h          gen_files/EventViewFrame.cpp
    gen_files/MainFrame.h               gen_files/MainFrame.cpp
    gen_files/EventCanvas.h             gen_files/EventCanvas.cpp

    EventTransportReaderGui.h           EventTransportReaderGui.cxx
    main.cpp
    MainFrameApp.h                      MainFrameApp.cpp
    Model.h                             Model.cpp
    EventCanvasApp.h                    EventCanvasApp.cpp
    TimeLineCursor.h                    TimeLineCursor.cpp
)

ADD_EXECUTABLE (right_track_gui  ${gui_sources} )
SET_TARGET_PROPERTIES( right_track_gui PROPERTIES
  VERSION ${right_track_VERSION}
)

INCLUDE_DIRECTORIES ( . .. gen_files )

SET_TARGET_PROPERTIES (right_track_gui PROPERTIES
      COMPILE_DEFINITIONS    "${wxWidgets_DEFINITIONS};wxWidgets_DEFINITIONS_DEBUG"
      COMPILE_FLAGS          "${wxWidgets_CXX_FLAGS}"
)

TARGET_LINK_LIBRARIES ( right_track_gui ${wxWidgets_LIBRARIES} right_track )


#
# install commands
#
INSTALL ( TARGETS right_track_gui
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
)
