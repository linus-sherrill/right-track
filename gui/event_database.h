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


// ----------------------------------------------------------------
/** Base class for event occurrences.
 *
 *
 */
class BaseOccurrence
{
public:
  wxString GetUserComment() const { return (m_userComment); }
  void SetUserComment(wxString const& v) { m_userComment = v; }
  virtual wxString GetInfo() = 0;

  EventPid_t m_eventPid;

  wxString m_userComment;

  bool m_selected; // set if the occurrence is selected

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
  bool ContainsTime (double time) const { return (time >= m_startTime) && (time <= m_endTime); }
  virtual wxString GetInfo();

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
  virtual wxString GetInfo();

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

  wxString GetUserComment() const { return (m_userComment); }
  void SetUserComment(wxString const& v) { m_userComment = v; }



  wxPen m_eventBaselinePen;

  double m_time;
  ItemId_t m_eventId;
  int m_color; // eventually convert to wxColor()

  bool m_enabled; // controls drawing (not drawn if not enabled)

  wxString m_eventName;
  wxString m_groupName;

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
  typedef vcl_vector < BoundedOccurrence >::iterator iterator_t;
  typedef vcl_vector < BoundedOccurrence >::const_iterator const_iterator_t;
  typedef vcl_vector < BoundedOccurrence >::reference occurrence_ref_t;

  BoundedEventDef() { }
  virtual ~BoundedEventDef() { }

  virtual Event::EventType_t EventType() const { return Event::ET_BOUNDED_EVENT; }
  virtual BoundedEventDef * GetBoundedEvent () { return this; }
  virtual wxString GetEventInfo();
  virtual size_t NumOccurrences() const { return m_list.size(); }

  BoundedOccurrence const* FindByTime (double time);


  // vector of occurrences
  vcl_vector < BoundedOccurrence > m_list;

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
  typedef vcl_vector < DiscreteOccurrence >::iterator iterator_t;
  typedef vcl_vector < DiscreteOccurrence >::const_iterator const_iterator_t;
  typedef vcl_vector < DiscreteOccurrence >::reference occurrence_ref_t;

  DiscreteEventDef() { }
  virtual ~DiscreteEventDef() { }

  virtual Event::EventType_t EventType() const { return Event::ET_DISCRETE_EVENT; }
  virtual DiscreteEventDef * GetDiscreteEvent () { return this; }
  virtual wxString GetEventInfo();
  virtual size_t NumOccurrences() const { return m_list.size(); }


  // vector of occurrences
  vcl_vector < DiscreteOccurrence > m_list;

};


// ----------------------------------------------------------------
// Context data types
//
struct ContextHistoryElement_t
{
  double event_time;

};


struct ContextDef_t
{
  ContextDefinition ctxt_def;

};


#endif /* _EVENT_DATABASE_H_ */
