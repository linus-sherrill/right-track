/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_ITEM_H_
#define _RIGHT_TRACK_ITEM_H_

#include "RightTrackDefs.h"

#include <boost/noncopyable.hpp>
#include <vcl_string.h>


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
  : private boost::noncopyable
{
public:
  virtual ~RightTrackItem();

  ItemId_t ItemId() const;
  vcl_string const& ItemName() const;


protected:
  RightTrackItem(vcl_string const& name); // CTOR

  Internal::EventManager* Manager();


private:
  Internal::EventManager* m_manager;
  ItemId_t m_itemId;
  vcl_string m_itemName;

}; // end class RightTrackItem

} // end namespace
} // end namespace

#endif /* _RIGHT_TRACK_ITEM_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
