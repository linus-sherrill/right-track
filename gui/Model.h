/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include <BoundedEvent.h>
#include <DiscreteEvent.h>
#include <EventRecords.h>

#include <vcl_map.h>
#include <vcl_list.h>
#include <vcl_vector.h>

#include <wx/pen.h>
#include <wx/brush.h>
#include <wx/colordlg.h>


using namespace ::RightTrack;
using namespace ::RightTrack::Internal;


//
// This structure defines a single action in the event stream for both
// bounded and discrete events
//
struct EventHistoryElement_t
{
  enum { START, END } ev_action;

  EventTimestamp_t event_time;
  EventPid_t event_pid;
  EventData_t event_data;
};


//
// This struct defines an event and it's history.
//
struct EventHistory_t
{
  EventDefinition ev_def;

  //     ET_BOUNDED_EVENT, ET_DISCRETE_EVENT
  Event::EventType_t EventType() const { return ev_def.event_type; }
  wxString EventName() const { return wxString(ev_def.event_name.c_str(), wxConvUTF8); }
  wxString EventGroup() const { return wxString(ev_def.event_group.c_str(), wxConvUTF8); }

  // Drawing objects
  wxPen eventBaselinePen;
  wxPen eventDurationPen;
  wxPen eventMarkerPen;

  wxPen startMarkerPen;
  wxBrush startMarkerBrush;

  wxPen endMarkerPen;
  wxBrush endMarkerBrush;


  vcl_vector < EventHistoryElement_t > EventHistory;
};


// ----------------------------------------------------------------
// Context data types
//
struct ContextHistoryElement_t
{
  EventTimestamp_t event_time;

};


struct ContextDef_t
{
  ContextDefinition ctxt_def;

};


// ----------------------------------------------------------------
/** Model part of MVC
 *
 *
 */
class Model
{
public:
  // -- TYPES --
  // Event data types
  typedef vcl_map < ItemId_t, EventHistory_t > event_map_t;
  typedef event_map_t::iterator event_iterator_t;
  typedef vcl_vector < EventHistoryElement_t >::iterator time_iterator_t;

  // Context data types
  typedef vcl_map < ItemId_t, ContextDef_t > context_map_t;
  typedef context_map_t::iterator context_iterator_t;
  typedef vcl_vector < ContextHistoryElement_t > context_history_t;
  typedef context_history_t::iterator pp_iterator_t;


  // -- CONSTRUCTORS --
  Model();  // CTOR
  void Reset();
  static Model * Instance();

  // -- ACCESSORS --
  EventTimestamp_t EventTimeRange() const;
  EventTimestamp_t TimeOffset() const { return m_timingOffset; }

  /**  Number of events in data base.
   */
  int EventCount() const { return this->m_eventMap.size(); }


  // -- MANIPULATORS --
  int ReadFromFile( const char * file);
  void Refresh();

  // FindEventByTime (ItemId_t id, EventTimestamp_t ots);

  // Event data areas
  vcl_vector <ItemId_t> m_drawOrder;
  event_map_t m_eventMap;

  // Context data areas
  context_map_t m_contextMap;
  vcl_vector < ContextHistoryElement_t > m_contextHistory;


  // event click info data areas
  wxString m_ei_eventName;
  int m_ei_eventCount;


  // Colors to use
  wxColour m_defaultBaselineColor;
  wxColour m_defaultLineColor;
  wxColour m_defaultEventColor;
  wxColour m_startEventColor;
  wxColour m_endEventColor;

  wxColourData m_persistentColourData;

private:
  void ScanEvents();


  EventTimestamp_t m_timingOffset; // time of first event
  EventTimestamp_t m_maxTime;

  // Last item number in data base. Used for appending items.
  ItemId_t m_maxItemNumber;

  static Model * s_instance;
};

#endif /* _MODEL_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:

