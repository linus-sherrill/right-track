/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include <RightTrackItem.h>
#include <EventManager.h>

namespace RightTrack {
  namespace Internal {

// ----------------------------------------------------------------
/** Constructor.
 *
 *
 */
RightTrackItem::
RightTrackItem(std::string const& name)
  : m_itemName(name)
{
  // Get handle to the manager.
  m_manager = ::RightTrack::Internal::EventManager::Instance();

  m_itemId = Manager()->GetNextId();
}


RightTrackItem::
~RightTrackItem()
{

}


// ----------------------------------------------------------------
/** Return Event ID.
 *
 *
 */
ItemId_t RightTrackItem::
ItemId() const
{
  return m_itemId;
}


// ----------------------------------------------------------------
/** Return event name.
 *
 *
 */
std::string const & RightTrackItem::
ItemName() const
{
  return m_itemName;
}


// ----------------------------------------------------------------
/** Return event manager object.
 *
 *
 */
Internal::EventManager * RightTrackItem::
Manager()
{
  return m_manager;
}


} // end name space
} // end name space
