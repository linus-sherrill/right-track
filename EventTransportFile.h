/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_EVENT_TRANSPORT_FILE_H_
#define _RIGHT_TRACK_EVENT_TRANSPORT_FILE_H_

#include "EventTransport.h"

#include <vcl_ostream.h>
#include <vcl_fstream.h>
#include <vsl/vsl_binary_io.h>


namespace RightTrack {
namespace Internal {

// ----------------------------------------------------------------
/** Event transport to a file.
 *
 *
 */
class EventTransportFile
  : public EventTransport
{
public:
  EventTransportFile();
  virtual ~EventTransportFile();

  virtual int Write(EventDefinition & msg);
  virtual int Write(EventStart & msg);
  virtual int Write(EventEnd & msg);
  virtual int Write(ContextDefinition & msg);
  virtual int Write(ContextPush & msg);
  virtual int Write(ContextPop & msg);

  virtual int ReadEvents(vcl_string const& resource,
                         EventTransportReader & reader);

  bool OpenFile (vcl_string name);


private:
  // support for writing
  vcl_ofstream * m_outStream;
  vsl_b_ostream * m_bstream;

}; // end class EventTransportFile

} // end namespace
} // end namespace

#endif /* _RIGHT_TRACK_EVENT_TRANSPORT_FILE_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
