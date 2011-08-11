/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _EVENT_DATABASE_H_
#define _EVENT_DATABASE_H_

#include <BoundedEvent.h>
#include <DiscreteEvent.h>
#include <EventRecords.h>

#include <wx/wx.h>
#include <vcl_vector.h>
#include <boost/shared_ptr.hpp>


using namespace ::RightTrack;
using namespace ::RightTrack::Internal;

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


class BoundedEventDef;
class DiscreteEventDef;
class BoundedOccurrence;
class DiscreteOccurrence;


// ----------------------------------------------------------------
/** Base class for event occurrences.
 *
 *
 */
class BaseOccurrence
{
public:
  typedef boost::shared_ptr < BaseOccurrence > handle_t;
  typedef vcl_vector < handle_t >::iterator iterator_t;
  typedef vcl_vector < handle_t >::const_iterator const_iterator_t;
  typedef vcl_vector < handle_t >::reference occurrence_ref_t;

  wxString const& GetUserComment() const { return (m_userComment); }
  void SetUserComment(wxString const& v) { m_userComment = v; }
  bool IsCommentActive() const { return ! m_userComment.empty(); }
  virtual wxString GetInfo() = 0;
  virtual bool ContainsTime (double time, double delta) const = 0;

  virtual BoundedOccurrence *  GetBoundedOccurrence() { return 0; }
  virtual DiscreteOccurrence *  GetDiscreteOccurrence() { return 0; }

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
  virtual bool ContainsTime (double time, double delta) const;
  virtual wxString GetInfo();
  virtual BoundedOccurrence *  GetBoundedOccurrence() { return this; }

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
  virtual bool ContainsTime (double time, double delta) const;
  virtual wxString GetInfo();
  virtual DiscreteOccurrence *  GetDiscreteOccurrence() { return this; }

  double m_eventTime; // in seconds
  EventData_t m_eventData;

  wxPen m_eventMarkerPen; // discrete event
  wxBrush m_eventMarkerBrush;
};



// ----------------------------------------------------------------
/** Base class for event definitions.
 *
 *
 */
class EventDef
{
public:
  typedef boost::shared_ptr < EventDef > handle_t;
  typedef vcl_vector < BaseOccurrence::handle_t >::iterator iterator_t;
  typedef vcl_vector < BaseOccurrence::handle_t >::const_iterator const_iterator_t;
  typedef vcl_vector < BaseOccurrence::handle_t >::reference occurrence_ref_t;

  EventDef();
  virtual ~EventDef();

  /** Returns event type ET_BOUNDED_EVENT, ET_DISCRETE_EVENT
   */
  virtual Event::EventType_t EventType() const = 0;
  wxString const& EventName() const { return m_eventName; }
  wxString const& EventGroup() const { return m_groupName; }

  virtual BoundedEventDef * GetBoundedEvent ();
  virtual DiscreteEventDef * GetDiscreteEvent ();

  virtual wxString GetEventInfo() = 0;
  virtual size_t NumOccurrences() const = 0;

  BaseOccurrence * FindByTime (double time, double delta);
  ItemId_t GetEventId() const { return m_eventId; }
  wxString const& GetUserComment() const { return (m_userComment); }
  void SetUserComment(wxString const& v) { m_userComment = v; }
  bool IsCommentActive() const { return ! m_userComment.empty(); }


  wxPen m_eventBaselinePen;

  double m_time;
  ItemId_t m_eventId;
  int m_color; // eventually convert to wxColor()

  bool m_enabled; // controls drawing (not drawn if not enabled)

  wxString m_eventName;
  wxString m_groupName;

  // vector of occurrences
  vcl_vector < BaseOccurrence::handle_t > m_list;

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
  BoundedEventDef() { }
  virtual ~BoundedEventDef() { }

  virtual Event::EventType_t EventType() const { return Event::ET_BOUNDED_EVENT; }
  virtual BoundedEventDef * GetBoundedEvent () { return this; }
  virtual wxString GetEventInfo();
  virtual size_t NumOccurrences() const { return m_list.size(); }

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
  DiscreteEventDef() { }
  virtual ~DiscreteEventDef() { }

  virtual Event::EventType_t EventType() const { return Event::ET_DISCRETE_EVENT; }
  virtual DiscreteEventDef * GetDiscreteEvent () { return this; }
  virtual wxString GetEventInfo();
  virtual size_t NumOccurrences() const { return m_list.size(); }
};


// ----------------------------------------------------------------
// Context data type
//
class ContextHistoryElement
{
public:
  typedef boost::shared_ptr < ContextHistoryElement > handle_t;

  double m_startTime;
  double m_endTime;

};


class ContextDef
{
public:
  ContextDefinition ctxt_def;

  // list of context history elements (by handle
};


#endif /* _EVENT_DATABASE_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
