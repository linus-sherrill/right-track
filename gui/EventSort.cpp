/*ckwg +5
 * Copyright 2011, 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 1712 Route 9, Clifton Park, NY 12065.
 */

#include "EventSort.h"

// ----------------------------------------------------------------
/** Base class methods
 *
 *
 */
EventSort::
EventSort()
  : m_ascending(true)
{ }

// ----------------------------------------------------------------
/** Sort by methods
 *
 *
 */
bool EventSort::
operator() (ItemId_t const& a, ItemId_t const& b)
{
  EventDef::handle_t ha = GetModel()->m_eventMap[a];
  EventDef::handle_t hb = GetModel()->m_eventMap[b];

  if (m_ascending)
  {
    return CompareLt (ha, hb);
  }
  else
  {
    return ! CompareLt (ha, hb);
  }
}


// ----------------------------------------------------------------
/** Bounded event sort methods
 *
 *
 */
bool BoundedEventSort::
operator() (ItemId_t const& a, ItemId_t const& b)
{
  EventDef::handle_t ha = GetModel()->m_eventMap[a];
  EventDef::handle_t hb = GetModel()->m_eventMap[b];

  BoundedEventDef * bea = ha->GetBoundedEvent();
  BoundedEventDef * beb = hb->GetBoundedEvent();

  if ( (bea != 0) && (beb != 0) )
  {
    // both are bounded events
    if (m_ascending)
    {
      return CompareLt (bea, beb);
    }
    else
    {
      return ! CompareLt (bea, beb);
    }
  }
  else
  {
    // one or both are not bounded events
    // sort bounded events to the top
    return (bea < beb);
  }
}
