/*ckwg +29
 * Copyright 2010, 2020 by Kitware, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 *  * Neither name of Kitware, Inc. nor the names of any contributors may be used
 *    to endorse or promote products derived from this software without specific
 *    prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _MODEL_H_
#define _MODEL_H_

#include <BoundedEvent.h>
#include <DiscreteEvent.h>
#include <EventRecords.h>
#include <Subject.h>

#include <map>
#include <list>
#include <vector>
#include <algorithm>

#include <wx/pen.h>
#include <wx/brush.h>
#include <wx/colordlg.h>

#include "event_database.h"

using namespace ::RightTrack;
using namespace ::RightTrack::Internal;

class MainFrameApp;

// ================================================================
// ----------------------------------------------------------------
/** Model part of MVC
 *
 * The model 
 */
class Model
  : public Subject
{
public:
  // -- TYPES --
  // Event data types
  using event_map_t = std::map < ItemId_t, EventDef::handle_t >;
  using event_iterator_t = event_map_t::iterator;
  using const_event_iterator_t = event_map_t::const_iterator;

  // Context data types
  using context_map_t = std::map < ItemId_t, ContextDef >;
  using context_iterator_t = context_map_t::iterator;
  using context_history_t = std::vector < ContextHistoryElement >;
  using pp_iterator_t = context_history_t::iterator;

  static const Subject::NotifyType_t UPDATE_cursor_info; // Cursor locations have changed
  static const Subject::NotifyType_t UPDATE_event_info; // event info sidebar (for selected event)
  static const Subject::NotifyType_t UPDATE_time_line; // Updates display of start and end time of view
  static const Subject::NotifyType_t UPDATE_event_frame; // redraw event frame

  // -- CONSTRUCTORS --
  Model();  // CTOR
  void Reset();
  static Model * Instance();

  // -- ACCESSORS --
  double EventTimeRange() const;

  /** Get time offset for events.  This method returns the earliest
   * time in the data set which is used as the start time for the
   * display.
   */
  double TimeOffset() const { return m_timingOffset; }

  /** Convert global time of offset time.
   */
  double TimeOffset(double time) const { return time - m_timingOffset; }

  /**  Number of events in data base.
   */
  int EventCount() const;
  int DisplayableEventCount() const;

  // provide access to the data set annotation
  wxString& DataSetAnnotation() { return m_modelAnnotation; }

  // -- MANIPULATORS --
  void ReadFromFile( const char * file); // read new events
  void LoadDataBaseFile( const char * file);
  void SaveToFile(); // use existing file name
  void SaveAsToFile( const char* file );
  

  void MoveSelectedEventTop();
  void MoveSelectedEventUp();
  void MoveSelectedEventDown();
  void MoveSelectedEventBottom();

  template <class SORT>
  void SortEvents ();

  // FindEventByTime (ItemId_t id, double ots);

  // Event data areas -- proposal
  struct drawing_attributes_t
  {
    ItemId_t event_id;
    bool selected;
    bool display_enable;
  };

  template<class Archive>
  void serialize(Archive & archive)
  {
    archive(CEREAL_NVP(m_drawOrder), CEREAL_NVP(m_eventMap),
            CEREAL_NVP(m_contextMap), CEREAL_NVP(m_contextHistory),
            CEREAL_NVP(m_defaultBaselineColor),
            CEREAL_NVP(m_defaultLineColor),
            CEREAL_NVP(m_defaultEventColor),
            CEREAL_NVP(m_startEventColor), CEREAL_NVP(m_endEventColor),
            CEREAL_NVP(m_selectColor), CEREAL_NVP(m_commentMarkerColor),
            CEREAL_NVP(m_timingOffset),
            CEREAL_NVP(m_maxTime), CEREAL_NVP(m_maxItemNumber),
            CEREAL_NVP(m_cursor_1_time), CEREAL_NVP(m_cursor_2_time),
            CEREAL_NVP(m_viewTimeStart), CEREAL_NVP(m_viewTimeEnd),

            CEREAL_NVP(m_modelAnnotation),
            CEREAL_NVP(m_eventFilter));
  }

  
  std::vector < ItemId_t > m_drawOrder;
  event_map_t m_eventMap; // list of events

  EventDef::handle_t GetEventHistory( ItemId_t ev) { return m_eventMap[ev]; }

  // Context data areas
  context_map_t m_contextMap;
  std::vector < ContextHistoryElement > m_contextHistory;

  // cursor time bounds
  void SetCursorTimes (double t1, double t2);
  void GetCursorTimes (double& t1, double& t2);

  // x-axis timeline bounds
  void SetTimeBounds (double start, double end);
  void GetTimeBounds (double& start, double& end);

  // Event selection
  void SelectEvent (ItemId_t event);
  bool IsEventSelected (ItemId_t event) const;
  ItemId_t GetSelectedEvent() const;

  // Occurrences selection
  void SelectOccurrence (BaseOccurrence * oc);
  bool IsOccurrenceSelected(BaseOccurrence * oc) const;
  BaseOccurrence * GetSelectedOccurrence() const;

  void SetEventFilter( bool v );
  bool IsEventDisplayable(ItemId_t event) const;
  
  // This method returns true if the model has been
  // modified since the last save.
  bool ModelNeedsSave() const;

  
  // Colors to use
  wxColour m_defaultBaselineColor;
  wxColour m_defaultLineColor;
  wxColour m_defaultEventColor;
  wxColour m_startEventColor;
  wxColour m_endEventColor;
  wxColour m_selectColor;
  wxColour m_commentMarkerColor;

  wxColourData m_persistentColourData;

  // Name where the model was loaded from or stored.
  std::string m_modelFileName;
  
private:
  void ScanEvents();

  double m_timingOffset; // time of first event
  double m_maxTime;

  // Last item number in data base. Used for appending items.
  ItemId_t m_maxItemNumber;

  double m_cursor_1_time;
  double m_cursor_2_time;

  double m_viewTimeStart;
  double m_viewTimeEnd;

  wxString m_modelAnnotation;

  // ---- non persistent state ----
  // Event click info
  BoundedEventStatistics m_evc_stats;
  double m_evc_data;

  // selected items
  ItemId_t m_selectedEvent;
  BaseOccurrence * m_selectedOccurrence;

  bool m_eventFilter;
  bool m_modelDirty;
  
  // Singleton support
  static Model * s_instance;
  
  // Make non-copyable
  Model( const Model& ) = delete;
  Model& operator=( const Model& ) = delete;
};


// ----------------------------------------------------------------
/** Sort event displayable order.
 *
 *
 */
template <class SORT>
void Model::
SortEvents ()
{
  std::sort (m_drawOrder.begin(), m_drawOrder.end(), SORT());

  // Need to redraw events
  Notify(UPDATE_event_frame);
}

#endif /* _MODEL_H_ */
