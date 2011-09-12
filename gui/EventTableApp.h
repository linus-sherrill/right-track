/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _EVENTTABLEAPP_H_
#define _EVENTTABLEAPP_H_

#include <EventTable.h>


// ----------------------------------------------------------------
/**
 *
 *
 */
class EventTableApp
  : EventTable
{
public:
  EventTableApp();
  virtual ~EventTableApp();

  void CreateGrid(int row, int col) { data_grid->CreateGrid(row,col); }

protected:



private:



}; // end class EventTableApp



#endif /* _EVENTTABLEAPP_H_ */
