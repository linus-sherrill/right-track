/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "BoundedEvent.h"
#include "DiscreteEvent.h"
#include "TextEvent.h"
#include "ScopedEvent.h"
#include "EventContext.h"

#include <EventManager.h>
#include <EventTransportProtoFile.h>
#include <EventTransportReaderDebug.h>

#include <unistd.h>

void foo()
{
  SCOPED_EVENT;

  sleep(1);
}


int main ()
{
  ::RightTrack::BoundedEvent be("main", "main-group");
  ::RightTrack::DiscreteEvent de("main.de");
  ::RightTrack::TextEvent te("main.te");
  ::RightTrack::EventContext ctxt("main foo");

  be.Start();
  ctxt.Push();
  for (int i = 0; i < 10; i++)
  {
    std::string str;
    str = "loop ";
    te.Start(str);
    sleep(1);

    de.Start(i);
    foo();
    sleep(1);
  }
  ctxt.Pop();
  be.End();

}
