/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_EVENT_TRANSPORT_DEBUG_H_
#define _RIGHT_TRACK_EVENT_TRANSPORT_DEBUG_H_

#include "EventTransport.h"

#include <vcl_ostream.h>
#include <vcl_fstream.h>
#include <vsl/vsl_binary_io.h>


namespace RightTrack {
namespace Internal {

// ----------------------------------------------------------------
/** Event transport to a debug.
 *
 *
 */
class EventTransportDebug
  : public EventTransport
{
public:
  EventTransportDebug();
  virtual ~EventTransportDebug();

  virtual int Write(EventDefinition & msg);
  virtual int Write(EventStart & msg);
  virtual int Write(EventEnd & msg);
  virtual int Write(ContextDefinition & msg);
  virtual int Write(ContextPush & msg);
  virtual int Write(ContextPop & msg);

  virtual int ReadEvents(vcl_string const& resource,
                         EventTransportReader & reader) { return 0; }

  bool OpenFile (vcl_string name) { return true; }


protected:


private:
  vcl_ostream * m_outStream;

}; // end class EventTransportDebug

} // end namespace
} // end namespace

#endif /* _RIGHT_TRACK_EVENT_TRANSPORT_DEBUG_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
