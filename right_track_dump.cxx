/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include "BoundedEvent.h"
#include "DiscreteEvent.h"
#include "ScopedEvent.h"

#include <EventManager.h>
#include <EventTransportFile.h>
#include <EventTransportReaderDebug.h>



int main (int argc, char * argv[])
{
  if (argc < 2)
  {
    std::cout << "Need to specify file name\n";
    exit(1);
  }

  // Test reading the created file.
  ::RightTrack::Internal::EventTransportFile etf;
  ::RightTrack::Internal::EventTransportReaderDebug etrd;

  etf.ReadEvents(argv[1], etrd);


}

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
