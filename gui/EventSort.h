/*ckwg +5
 * Copyright 2011, 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef _RIGHT_TRACK_EVENT_SORT_H_
#define _RIGHT_TRACK_EVENT_SORT_H_

#include "Model.h"

// ----------------------------------------------------------------
/** Base class for all event sorters.
 *
 *
 */
class EventSort
{
public:
  EventSort();
  virtual ~EventSort() = default;

  virtual bool operator() (ItemId_t const& a, ItemId_t const& b);

  void Ascending() { m_ascending = true; }
  void Descending() { m_ascending = false; }

protected:
  Model * GetModel() const { return Model::Instance(); }

  virtual bool CompareLt( EventDef::handle_t ha, EventDef::handle_t hb) = 0;

protected:
  bool m_ascending;

}; // end class EventSort


// ----------------------------------------------------------------
/** Base class for bounded event specific sorts
 *
 *
 */
class BoundedEventSort
  : public EventSort
{
public:
  virtual bool operator() (ItemId_t const& a, ItemId_t const& b);

protected:
  virtual bool CompareLt( BoundedEventDef const* bea, BoundedEventDef const* beb) = 0;

private:
  virtual bool CompareLt( EventDef::handle_t ha, EventDef::handle_t hb) { return 0; }
}; // end class BoundedEventSort


// ----------------------------------------------------------------
/** Sort by - implementation.
 *
 *
 */
#define SORT_BY(N, F)                                                   \
class SortBy ## N : public EventSort                                   \
{                                                                       \
protected:                                                              \
  virtual bool CompareLt( EventDef::handle_t ha, EventDef::handle_t hb) { return (ha->F < hb->F); } \
} // end class


SORT_BY(Name, EventName() );
SORT_BY(Count, NumOccurrences() );

#undef SORT_BY


// ================================================================
// ----------------------------------------------------------------
/** Sort by  - implementation for bounded event fields
 *
 * This is only valid for bouned events
 */
#define SORT_BOUNDED_BY(N, F)                                           \
class SortBy ## N : public BoundedEventSort                             \
{                                                                       \
protected:                                                              \
  virtual bool CompareLt( BoundedEventDef const* bea, BoundedEventDef const* beb) \
   { return (bea->F < beb->F); }                                        \
} // end class


SORT_BOUNDED_BY(AverageTime, m_stats.m_avgDuration);
SORT_BOUNDED_BY(MaxDuration, m_stats.m_maxDuration);
SORT_BOUNDED_BY(MinDuration, m_stats.m_minDuration);
SORT_BOUNDED_BY(Percentage,  m_stats.m_activePct);

#undef SORT_BOUNDED_BY

#endif /* _RIGHT_TRACK_EVENT_SORT_H_ */
