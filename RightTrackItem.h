/*ckwg +5
 * Copyright 2010-2018 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_ITEM_H_
#define _RIGHT_TRACK_ITEM_H_

#include "RightTrackDefs.h"

#include <string>

namespace RightTrack {
namespace Internal {

// partial types
class EventManager;

// ----------------------------------------------------------------
/** Base class for all Right Track classes.
 *
 *
 */
class RightTrackItem
{
public:
  virtual ~RightTrackItem();

  ItemId_t ItemId() const;
  std::string const& ItemName() const;


protected:
  RightTrackItem(std::string const& name); // CTOR

  Internal::EventManager* Manager();


private:
  RightTrackItem(const RightTrackItem& ) = delete;
  RightTrackItem& operator=( const RightTrackItem& ) = delete;

  Internal::EventManager* m_manager;
  ItemId_t m_itemId;
  std::string m_itemName;

}; // end class RightTrackItem

} // end namespace
} // end namespace

#endif /* _RIGHT_TRACK_ITEM_H_ */
