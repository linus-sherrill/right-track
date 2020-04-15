/*ckwg +5
 * Copyright 2010-2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "BoundedEvent.h"
#include "DiscreteEvent.h"
#include "ScopedEvent.h"

#include <EventManager.h>
#include <EventTransportProtoFile.h>
#include <EventTransportReaderDebug.h>

#include <iostream>

int main (int argc, char * argv[])
{
  if (argc < 2)
  {
    std::cout << "Need to specify file name\n";
    exit(1);
  }

  // Test reading the created file.
  ::RightTrack::Internal::EventTransportProtoFile etf;
  ::RightTrack::Internal::EventTransportReaderDebug etrd;

  etf.ReadEvents(argv[1], etrd);

}
