/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include "BoundedEvent.h"
#include "DiscreteEvent.h"
#include "ScopedEvent.h"
#include "EventContext.h"


#include <EventManager.h>
#include <EventTransportFile.h>
#include <EventTransportReaderDebug.h>


void foo()
{
  SCOPED_EVENT;

  sleep(1);
}


int main ()
{
  ::RightTrack::BoundedEvent be("main", "main-group");
  ::RightTrack::DiscreteEvent de("main.de");
  ::RightTrack::EventContext ctxt("main foo");

  be.Start();
  ctxt.Push();
  for (int i = 0; i < 10; i++)
  {
    de.Start(i);
    foo();
    sleep(1);
  }
  ctxt.Pop();
  be.End();

}


// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
