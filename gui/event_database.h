/*ckwg +5
 * Copyright 2011, 2020 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

/**
 * \file This file contains the data structures that are used for
 * the internal representation of the event data and related
 * statistics.
 */
#ifndef _EVENT_DATABASE_H_
#define _EVENT_DATABASE_H_

#include <BoundedEvent.h>
#include <DiscreteEvent.h>
#include <EventRecords.h>

#include <wx/wx.h>

#include <Serialize.h>

#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/portable_binary.hpp>

#include <cereal/cereal.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>
#include <cereal/types/utility.hpp>
#include <cereal/types/vector.hpp>

#include <memory>
#include <vector>

using namespace ::RightTrack;
using namespace ::RightTrack::Internal;

class BaseOccurrence;
class EventDef;
class BoundedEventDef;
class DiscreteEventDef;
class BoundedOccurrence;
class DiscreteOccurrence;

// ----------------------------------------------------------------

/** Bounded event statistics
 *
 *
 */
struct BoundedEventStatistics
{
  int m_count;
  double m_minDuration;
  double m_maxDuration;
  double m_avgDuration;
  double m_stdDuration;
  double m_activePct;  // (total run time) / (total elapsed tome)
};

// ----------------------------------------------------------------

/** Discrete event statistics
 *
 *
 */
struct DiscreteEventStatistics
{
  int m_count;
};

// ----------------------------------------------------------------

/** Base class for event occurrences.
 *
 *
 */
class BaseOccurrence
{
public:
  using handle_t = std::shared_ptr< BaseOccurrence >;
  using iterator_t = std::vector< handle_t >::iterator;
  using const_iterator_t = std::vector< handle_t >::const_iterator;
  using occurrence_ref_t = std::vector< handle_t >::reference;

  wxString const& GetUserComment() const { return ( m_userComment ); }
  void SetUserComment( wxString const& v ) { m_userComment = v; }
  bool IsCommentActive() const { return !m_userComment.empty(); }
  virtual wxString GetInfo() = 0;
  virtual bool ContainsTime( double time, double delta ) const = 0;

  virtual BoundedOccurrence*  GetBoundedOccurrence() { return 0; }
  virtual DiscreteOccurrence*  GetDiscreteOccurrence() { return 0; }

  template < class Archive >
  void
  serialize( Archive& archive )
  {
    archive( CEREAL_NVP( m_eventPid ),
             CEREAL_NVP( m_selected ),
             CEREAL_NVP( m_userComment ) );
  }

  // -- memeber data --
  EventPid_t m_eventPid;

  bool m_selected; // set if the occurrence is selected

protected:
  wxString m_userComment;
};

// ----------------------------------------------------------------

/** Bounded event occurence.
 *
 *
 */
class BoundedOccurrence
  : public BaseOccurrence
{
public:
  bool ContainsTime( double time, double delta ) const override;
  wxString GetInfo() override;

  BoundedOccurrence* GetBoundedOccurrence() override { return this; }

  template < class Archive >
  void
  serialize( Archive& archive )
  {
    archive( cereal::base_class< BaseOccurrence >( this ),
             CEREAL_NVP( m_startTime ), CEREAL_NVP( m_startData ),
             CEREAL_NVP( m_endTime ), CEREAL_NVP( m_endData ),
             CEREAL_NVP( m_startMarkerPen ), CEREAL_NVP( m_startMarkerBrush ),
             CEREAL_NVP( m_eventDurationPen ), CEREAL_NVP( m_endMarkerPen ),
             CEREAL_NVP( m_endMarkerBrush ) );
  }

  // -- memeber data --
  double m_startTime; // in seconds
  EventData_t m_startData;

  double m_endTime; // in seconds
  EventData_t m_endData;

  wxPen m_startMarkerPen;
  wxBrush m_startMarkerBrush;

  wxPen m_eventDurationPen;

  wxPen m_endMarkerPen;
  wxBrush m_endMarkerBrush;
};

// ----------------------------------------------------------------

/** Discrete event occurrence.
 *
 *
 */
class DiscreteOccurrence
  : public BaseOccurrence
{
public:
  bool ContainsTime( double time, double delta ) const override;
  wxString GetInfo() override;

  DiscreteOccurrence* GetDiscreteOccurrence() override { return this; }

  template < class Archive >
  void
  serialize( Archive& archive )
  {
    archive( cereal::base_class< BaseOccurrence >( this ),
             CEREAL_NVP( m_eventTime ), CEREAL_NVP( m_eventData ),
             CEREAL_NVP( m_eventMarkerPen ), CEREAL_NVP( m_eventMarkerBrush ) );
  }

  // -- memeber data --
  double m_eventTime; // in seconds
  EventData_t m_eventData;

  wxPen m_eventMarkerPen; // discrete event
  wxBrush m_eventMarkerBrush;
};

// ----------------------------------------------------------------

/** Base class for event definitions.
 *
 * There is one of these objects for each unique event. Each
 * occurrence of an event is carried in a vector.
 */
class EventDef
{
public:
  using handle_t = std::shared_ptr< EventDef >;
  using iterator_t = std::vector< BaseOccurrence::handle_t >::iterator;
  using const_iterator_t = std::vector< BaseOccurrence::handle_t >::const_iterator;
  using occurrence_ref_t = std::vector< BaseOccurrence::handle_t >::reference;

  EventDef() = default;
  virtual ~EventDef() = default;

  /** Returns event type ET_BOUNDED_EVENT, ET_DISCRETE_EVENT
   */
  virtual Event::EventType_t EventType() const = 0;

  wxString const& EventName() const { return m_eventName; }
  wxString const& EventGroup() const { return m_groupName; }

  virtual BoundedEventDef* GetBoundedEvent();
  virtual DiscreteEventDef* GetDiscreteEvent();

  virtual wxString GetEventInfo() = 0;
  virtual size_t NumOccurrences() const = 0;

  BaseOccurrence* FindByTime( double time, double delta );

  ItemId_t GetEventId() const { return m_eventId; }
  wxString const& GetUserComment() const { return ( m_userComment ); }
  void SetUserComment( wxString const& v ) { m_userComment = v; }
  bool IsCommentActive() const { return !m_userComment.empty(); }

  template < class Archive >
  void
  serialize( Archive& archive )
  {
    archive( CEREAL_NVP( m_eventBaselinePen ),
             CEREAL_NVP( m_time ),
             CEREAL_NVP( m_eventId ),
             CEREAL_NVP( m_color ),
             CEREAL_NVP( m_enabled ),
             CEREAL_NVP( m_eventName ),
             CEREAL_NVP( m_groupName ),
             CEREAL_NVP( m_originName ),
             CEREAL_NVP( m_userComment ),
             CEREAL_NVP( m_list )
             );
  }

  // -- member data --
  wxPen m_eventBaselinePen;

  double m_time;
  ItemId_t m_eventId;
  wxColour m_color;

  bool m_enabled; // controls drawing (not drawn if not enabled)

  wxString m_eventName;
  wxString m_groupName;

  // This name is intended to uniquely identify a single source
  // for events. This is used when multiple event files are merged
  // into one database. This is used when analyzing multiple processes
  // in a distributed application. It is possible that these processes
  // do not have a synchronized clock. This starts as the file name
  // and can be modified later by the user, as long is it is unique
  // over this database.
  wxString m_originName;

  // vector of occurrences
  std::vector< BaseOccurrence::handle_t > m_list;

protected:
  wxString m_userComment;
};

// ----------------------------------------------------------------

/** Event definition for bounded events.
 *
 *
 */
class BoundedEventDef
  : public EventDef
{
public:
  BoundedEventDef() = default;
  virtual ~BoundedEventDef() = default;

  Event::EventType_t EventType() const override { return Event::ET_BOUNDED_EVENT; }
  BoundedEventDef* GetBoundedEvent() override { return this; }
  wxString GetEventInfo() override;

  size_t NumOccurrences() const override { return m_list.size(); }

  template < class Archive >
  void
  serialize( Archive& archive )
  {
    // Save the base class
    archive( cereal::base_class< EventDef >( this ) );
    // m_stats not serialized
  }

  // -- member data --
  BoundedEventStatistics m_stats; // calculated when data is loaded
};

// ----------------------------------------------------------------

/** Event definition for discrete events.
 *
 *
 */
class DiscreteEventDef
  : public EventDef
{
public:
  DiscreteEventDef() = default;
  virtual ~DiscreteEventDef() = default;

  Event::EventType_t EventType() const override { return Event::ET_DISCRETE_EVENT; }
  DiscreteEventDef* GetDiscreteEvent() override { return this; }
  wxString GetEventInfo() override;

  size_t NumOccurrences() const override { return m_list.size(); }

  template < class Archive >
  void
  serialize( Archive& archive )
  {
    // Save the base class
    archive( cereal::base_class< EventDef >( this ) );
    // m_stats not serialized
  }

  // -- member data --
  DiscreteEventStatistics m_stats;
};

// ----------------------------------------------------------------
// Context data type
//
class ContextHistoryElement
{
public:
  template < class Archive >
  void
  serialize( Archive& archive )
  {
    archive( CEREAL_NVP( m_startTime ),
             CEREAL_NVP( m_endTime ) );
  }

  double m_startTime;
  double m_endTime;
};

// ----------------------------------------------------------------
class ContextDef
{
public:
  template < class Archive >
  void
  serialize( Archive& archive )
  {
    archive( cereal::make_nvp( "context_name", m_ctxtDef.context_name ),
             cereal::make_nvp( "context_id", m_ctxtDef.context_id ) );
  }

  ContextDefinition m_ctxtDef;

  // list of context history elements (by handle)
};

CEREAL_REGISTER_TYPE( BoundedEventDef );
CEREAL_REGISTER_TYPE( DiscreteEventDef );
CEREAL_REGISTER_TYPE( BoundedOccurrence );
CEREAL_REGISTER_TYPE( DiscreteOccurrence );

CEREAL_REGISTER_POLYMORPHIC_RELATION( BaseOccurrence, BoundedOccurrence )
CEREAL_REGISTER_POLYMORPHIC_RELATION( BaseOccurrence, DiscreteOccurrence )
CEREAL_REGISTER_POLYMORPHIC_RELATION( EventDef, BoundedEventDef )
CEREAL_REGISTER_POLYMORPHIC_RELATION( EventDef, DiscreteEventDef )

#endif /* _EVENT_DATABASE_H_ */
