/*ckwg +5
 * Copyright 2010-2011, 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_EVENT_TRANSPORT_READER_GUI_H_
#define _RIGHT_TRACK_EVENT_TRANSPORT_READER_GUI_H_

#include <EventTransportReader.h>
#include "Model.h"


namespace RightTrack {
  namespace Internal {


// ----------------------------------------------------------------
/** Event receiver GUI class.
 *
 * This class is used to handle event structure creation when
 * unmarshling events from the transport medium.  One of the
 * NewEvent() methods is called with the appropriate type when that
 * type of event is read from the medium.
 */
class EventTransportReaderGui
  : public EventTransportReader
{
public:
  EventTransportReaderGui(Model * m, wxString const& originString);
  virtual ~EventTransportReaderGui() = default;

  int NewEvent(EventDefinition const& msg) override;
  int NewEvent(EventStart const& msg) override;
  int NewEvent(EventEnd const& msg) override;
  int NewEvent(EventText const& msg) override;
  int NewEvent(ContextDefinition const& msg) override;
  int NewEvent(ContextPush const& msg) override;
  int NewEvent(ContextPop const& msg) override;


private:
  Model * m_model;
  wxString m_originString;
  
}; // end class EventTransportReaderGui


} // end namespace
} // end namespace

#endif /* _RIGHT_TRACK_EVENT_TRANSPORT_READER_GUI_H_ */
