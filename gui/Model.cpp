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

#include "Model.h"

#include <MainFrameApp.h>

#include <wx/wx.h>
#include <wx/app.h>

#include <EventTransportProtoFile.h>
#include <EventTransportReaderGui.h>
#include <EventTransportReaderDebug.h>

#include <cereal/cereal.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/portable_binary.hpp>
#include <cereal/archives/json.hpp>

#include <cstdio>
#include <stdexcept>
#include <sstream>

/**
 * TODO
 *
 * Error reporting is sloppy. We can not interact with the window manager
 * and return codes are so 80's. Throwing an exception with descriptive text
 * seems like the best approach. Which eception?  std::runtime_error?
 * custom application exception?
 */


// Support for singleton
Model * Model::s_instance( nullptr );

const Subject::NotifyType_t Model::UPDATE_cursor_info {Subject::NotifyType(1)};
const Subject::NotifyType_t Model::UPDATE_event_info  {Subject::NotifyType(2)};
const Subject::NotifyType_t Model::UPDATE_time_line   {Subject::NotifyType(3)};
const Subject::NotifyType_t Model::UPDATE_event_frame {Subject::NotifyType(4)};

#if 1 // testing
  using ModelOutputArchive = ::cereal::JSONOutputArchive;
  using ModelInputArchive  = ::cereal::JSONInputArchive;
#else
  using ModelOutputArchive = ::cereal::BinaryOutputArchive;
  using ModelInputArchive  = ::cereal::BinaryInputArchive;
#endif

// ----------------------------------------------------------------
/** Constructor.
 *
 *
 */
Model
::Model()
{
  s_instance = this;
  Reset();
}


Model * Model
::Instance()
{
  return (s_instance);
}


// ----------------------------------------------------------------
/** Reset model.
 *
 *
 */
void Model
::Reset()
{
  m_firstEventTime = 1e300;

  m_drawOrder.clear();
  m_eventMap.clear();

  // These default colors are arbitrary
  m_defaultBaselineColor = wxColour ( 120, 120, 120 );
  m_defaultLineColor = wxColour ( 128, 128, 128 ); // gray
  m_defaultEventColor = wxColour ( 35, 19, 238 );

  m_startEventColor = wxColour ( 10, 167, 6 ); // green
  m_endEventColor = wxColour ( 229, 22, 22 ); // red

  m_selectColor = wxColour (222, 207, 15);

  m_commentMarkerColor = wxColour (166, 98, 203);

  m_selectedEvent = -1;
  m_selectedOccurrence = 0;

  m_maxTime = 0;
  
  m_eventFilter = false;
  m_modelDirty = false;

  m_modelAnnotation.clear();
  m_modelFileName.clear();
}


// ----------------------------------------------------------------
/** Read events from file.
 *
 * This method reads a raw event file and merges it into the
 * current data base.
 */
void Model
::ReadFromFile( const char * file )
{
  // instantiate our reader
  // EventTransportReaderDebug reader;
  wxString fileName(file);
  EventTransportReaderGui reader (this, fileName);
  EventTransportProtoFile file_io;
  int status(0);

  status = file_io.ReadEvents (file, reader);
  if (status != 0)
  {
    throw std::runtime_error( "Error reading raw events file");
  }

  m_modelDirty = true;
  ScanEvents();
}


// ----------------------------------------------------------------
/** Load events database from file.
 *
 * This method loads a saved (serialized) data base into this model.
 * It replaces any exsiting model data.
 */
void Model
::LoadDataBaseFile( const char * file)
{
  //+ validate the file ends in ".rtmdl"
  std::ifstream input( file , std::ios::binary );
  if (!input)
  {
    std::stringstream sstr;
    sstr << "Could not open file \"" << file << "\"";
    throw std::runtime_error( sstr.str());
  }
  // Reset model
  Reset();

  ModelInputArchive input_ar( input );
  input_ar( *this ); // may throw
}


// ----------------------------------------------------------------
/** Save events database from file.
 *
 * This method saves the current model to the same file name
 *  it was loaded from.
 */
void Model
::SaveToFile()
{
  std::string out_filename( m_modelFileName );

  //+ file name ends in ".rtmdl" or add that to it.
  // string backwards until first '.' - that is the file extension
  auto pos = m_modelFileName.find_last_of(".");
  if ( pos != std::string::npos)
  {
    // string off the filename portion
    out_filename = m_modelFileName.substr(0, pos);
  }
  m_modelFileName = out_filename + ".rtmdl";
  std::string temp_file = m_modelFileName + ".temp";
  
  // Write serialized model to temp file
  std::ofstream ostr( temp_file, std::ios::binary );
  if ( ! ostr )
  {
    std::stringstream sstr;
    sstr << "Could not open file \"" << temp_file << "\"";
    throw std::runtime_error( sstr.str());
  }
  
  ModelOutputArchive archive( ostr ); // may throw
  archive( *this );

  ostr.close();
  
  // Delete m_modelFileName
  if( remove( m_modelFileName.c_str()) )
  {
    // Error deleting the file
    std::stringstream sstr;
    sstr << "Could not delete file during save \"" << m_modelFileName << "\"";
    throw std::runtime_error( sstr.str());
  }
  
  // rename out_filename -> model_file
  if( rename(temp_file.c_str(), m_modelFileName.c_str()) )
  {
    // Error renaming the file
    std::stringstream sstr;
    sstr << "Could not rename file during save. "
        << "From \"" << temp_file << " to " << m_modelFileName;
    throw std::runtime_error( sstr.str());
  }

  m_modelDirty = false;
}

// ----------------------------------------------------------------
/** Save events database from file.
 *
 * This method saves the current model to a new file name.
 * The file should have the ".rtmd" extsnsion.
 */
void Model
::SaveAsToFile( const char* file)
{
  // Write serialized model to temp file
  std::ofstream ostr( file, std::ios::binary );
  if ( ! ostr )
  {
    std::stringstream sstr;
    sstr << "Could not open file \"" << file << "\"";
    throw std::runtime_error( sstr.str());
  }

  ModelOutputArchive archive( ostr ); // may throw
  archive( *this );
  ostr.close();

  m_modelFileName = file;
  m_modelDirty = false;
}

// ----------------------------------------------------------------
/** Get timespan of all events.
 *
 * This method returns the length of time covered from the first event
 * to the last event.
 */
double Model
::EventTimeRange() const
{
  return  (m_maxTime - m_firstEventTime);
}


// ----------------------------------------------------------------
/** Scan events
 *
 * Scan the list of events and setup display objects.
 *
 * This is the place to mess with the colouring of the event displays.
 */
void Model
::ScanEvents()
{
  wxPen start_marker_pen = wxPen ( m_startEventColor, 1, wxPENSTYLE_SOLID );
  wxBrush start_marker_brush = wxBrush ( m_startEventColor, wxBRUSHSTYLE_SOLID );

  wxPen end_marker_pen = wxPen ( m_endEventColor, 1, wxPENSTYLE_SOLID );
  wxBrush end_marker_brush = wxBrush ( m_endEventColor, wxBRUSHSTYLE_SOLID );


  event_iterator_t ix;
  for (ix = m_eventMap.begin(); ix != m_eventMap.end(); ix++)
  {
    EventDef::handle_t eh = ix->second;

    eh->m_enabled = true; // all start enabled

    // look for minimum time to use as our base time offset
    // Since the events must be defined before they are used, all we have to do
    // is to look at the eh->m_time
    if (eh->m_time < m_firstEventTime)
    {
      m_firstEventTime = eh->m_time;
    }

    // Setup general colors
    eh->m_eventBaselinePen = wxPen ( m_defaultBaselineColor, 1, wxPENSTYLE_SOLID );

    wxPen event_duration_pen;
    wxPen event_marker_pen;

    // Check for event specific colours
    if (eh->m_color == -1)
    {
      // Use default colors
      event_duration_pen = wxPen ( m_defaultEventColor, 2, wxPENSTYLE_SOLID );
      event_marker_pen   = wxPen ( m_defaultEventColor, 1, wxPENSTYLE_SOLID );
    }
    else
    {
      event_duration_pen = wxPen ( eh->m_color, 2, wxPENSTYLE_SOLID );
      event_marker_pen   = wxPen ( eh->m_color, 1, wxPENSTYLE_SOLID );
    }

    // do specific processing by event type.
    switch (eh->EventType())
    {
    case Event::ET_DISCRETE_EVENT:
    {
      DiscreteEventDef * def = eh->GetDiscreteEvent();
      EventDef::iterator_t it = def->m_list.begin();
      EventDef::iterator_t eit = def->m_list.end();

      int count (0);

      for ( ; it != eit; it++)
      {
        DiscreteOccurrence * dop = (*it)->GetDiscreteOccurrence();
        double ts = dop->m_eventTime;

        if (ts > m_maxTime)
        {
          m_maxTime = ts;
        }

        dop->m_eventMarkerPen = event_marker_pen;
        dop->m_eventMarkerBrush = start_marker_brush;

        count++;
      } // end for it

      def->m_stats.m_count = count;
    }
    break;

    case Event::ET_BOUNDED_EVENT:
    {
      BoundedEventDef * def = eh->GetBoundedEvent();
      EventDef::iterator_t it = def->m_list.begin();
      EventDef::iterator_t eit = def->m_list.end();

      def->m_stats.m_minDuration = 1e300;
      def->m_stats.m_maxDuration = 0.0;
      double sum (0);
      double sum_sq (0);
      double count (0);

      for ( ; it != eit; it++)
      {
        BoundedOccurrence * bop = (*it)->GetBoundedOccurrence();
        double ts = bop->m_endTime;

        if (ts > m_maxTime)
        {
          m_maxTime = ts;
        }

        bop->m_startMarkerPen = start_marker_pen;
        bop->m_startMarkerBrush = start_marker_brush;

        bop->m_eventDurationPen = event_duration_pen;

        bop->m_endMarkerPen = end_marker_pen;
        bop->m_endMarkerBrush = end_marker_brush;

        double duration = bop->m_endTime - bop->m_startTime;
        if (def->m_stats.m_maxDuration < duration) def->m_stats.m_maxDuration = duration;
        if (def->m_stats.m_minDuration > duration) def->m_stats.m_minDuration = duration;
        sum += duration;
        sum_sq += (duration * duration);
        count ++;
      } // end for it

      def->m_stats.m_count = static_cast< int >(count);
      def->m_stats.m_avgDuration = sum / count;
      def->m_stats.m_stdDuration = sqrt((sum_sq - (sum * def->m_stats.m_avgDuration)) / (count - 1) );

      if (def->m_list.size() >= 1) // need some elements for this to work
      {
        def->m_stats.m_activePct = (sum / (def->m_list.back()->GetBoundedOccurrence()->m_endTime
                                           - def->m_list.front()->GetBoundedOccurrence()->m_startTime)) * 100.0;
      }
      else
      {
        def->m_stats.m_activePct = 0;
      }
    }
    break;

    default:
      // display message;
      wxMessageBox( wxT("Internal error - unexpected event type"),
                    wxT("Error"), wxICON_ERROR | wxOK);
      break;
    } // end switch
  } // end for
}

// ================================================================
void Model
::SelectEvent (ItemId_t event)
{
  m_selectedEvent = event;

  // update event info pane, update event frame to get new highlight
  Notify(UPDATE_event_info);
  Notify(UPDATE_event_frame);

  m_modelDirty = true;
}


bool Model
::IsEventSelected (ItemId_t event) const
{
  return (m_selectedEvent == event);
}


ItemId_t Model
::GetSelectedEvent() const
{
  return m_selectedEvent;
}


// ================================================================
void Model
::SelectOccurrence (BaseOccurrence * oc)
{
  m_selectedOccurrence = oc;

  m_modelDirty = true;
}


bool Model
::IsOccurrenceSelected(BaseOccurrence * oc) const
{
  return (m_selectedOccurrence == oc);
}


BaseOccurrence * Model
::GetSelectedOccurrence() const
{
  return m_selectedOccurrence;
}



// ================================================================
int Model
::EventCount() const
{
  return this->m_eventMap.size();
}


// ----------------------------------------------------------------
/** Number of displayable events.
 *
 * Calculate the number of displayable events. Good for determining
 * number of displayable lines.
 */
int Model
::DisplayableEventCount() const
{
  int count(0);
  const size_t limit ( m_drawOrder.size() );

  for (size_t i = 0; i < limit; i++)
  {
    if ( IsEventDisplayable ( m_drawOrder[i] ) )
    {
      count++;
    }
  } // end for

  return count;
}

// ----------------------------------------------------------------
/** Move selected event to top of list.
 *
 *
 */
void Model
::MoveSelectedEventTop()
{
  ItemId_t item = GetSelectedEvent();
  if (item < 0)
  {
    return; // no selected event
  }

  size_t index(0);

  // Scan the drawing order vector

  // Start loop at 1 since if the desired element is at index 0, it
  // can not be moved higher, so don't even try.
  size_t limit = m_drawOrder.size();
  for (size_t i = 1; i < limit; ++i)
  {
    if (m_drawOrder[i] == item)
    {
      index = i;
      break;
    }
  } // end for

  // On exit, index -> selected elements location in draw order,
  // of zero, indicating not found or already at top
  if (0 == index)
  {
    return; // item not found
  }

  // shift list down from [0] .. [index] one slot.
  for (size_t i = index; i > 0; i--)
  {
    m_drawOrder[i] = m_drawOrder[i-1];
  }

  // store item at Top
  m_drawOrder[0] = item;

  // Need to redraw all events
  Notify(UPDATE_event_frame);

  m_modelDirty = true;
}


// ----------------------------------------------------------------
/** Move currently selected event up one row.
 *
 *
 */
void Model
::MoveSelectedEventUp()
{
  ItemId_t item = GetSelectedEvent();
  if (item < 0)
  {
    return; // no selected event
  }

  // Scan the drawing order vector

  // Start loop at 1 since if the desired element is at index 0, it
  // can not be moved higher, so don't even try.
  size_t limit = m_drawOrder.size();
  for (size_t i = 1; i < limit; ++i)
  {
    if (m_drawOrder[i] == item)
    {
      // swap [i] and [i-1]
      ItemId_t temp = m_drawOrder[i];
      m_drawOrder[i] = m_drawOrder[i-1];
      m_drawOrder[i-1] = temp;

      // Need to redraw events
      Notify(UPDATE_event_frame);

      break;
    }
  } // end for

  m_modelDirty = true;
}


// ----------------------------------------------------------------
/** Move currently selected event down one row.
 *
 *
 */
void Model
::MoveSelectedEventDown()
{
  ItemId_t item = GetSelectedEvent();
  if (item < 0)
  {
    return; // no selected event
  }

  // Scan the drawing order vector

  // End loop at next to the last element, since if the one we are
  // looking for is there, it can not be moved.
  size_t limit = m_drawOrder.size() -1;
  for (size_t i = 0; i < limit; ++i)
  {
    if (m_drawOrder[i] == item)
    {
      // swap [i] and [i+1]
      ItemId_t temp = m_drawOrder[i];
      m_drawOrder[i] = m_drawOrder[i+1];
      m_drawOrder[i+1] = temp;

      // Need to redraw events
      Notify(UPDATE_event_frame);

      break;
    }
  } // end for

  m_modelDirty = true;
}


// ----------------------------------------------------------------
/** Move selected event to bottom of list.
 *
 *
 */
void Model
::MoveSelectedEventBottom()
{
  ItemId_t item = GetSelectedEvent();
  if (item < 0)
  {
    return; // no selected event
  }

  int index{-1};

  // Scan the drawing order vector

  // End loop at next to the last element, since if the one we are
  // looking for is there, it can not be moved.
  size_t limit = m_drawOrder.size() -1;
  for (size_t i = 0; i < limit; ++i)
  {
    if (m_drawOrder[i] == item)
    {
      index = i;
      break;
    }
  } // end for

  // On exit, index -> selected elements location in draw order,
  // of zero, indicating not found.
  if (-1 == index)
  {
    return; // item not found - not expected
  }

  // shift list up from [index] .. [limit-1] one slot.
  for (size_t i = index; i <= limit-1; i++)
  {
    m_drawOrder[i] = m_drawOrder[i+1];
  }

  // store item at Bottom
  m_drawOrder[limit] = item;

  // Need to redraw events
  Notify(UPDATE_event_frame);

  m_modelDirty = true;
}

// ----------------------------------------------------------------
/** Set filter mode.
 *
 *
 */
void Model
::SetEventFilter( bool v )
{
  m_eventFilter = v;

  // Need to redraw events
  Notify(UPDATE_event_frame);
}

// ----------------------------------------------------------------
/** Enable all events.
 *
 * This method enables all events to be displayed.
 * The list of events is scanned and the enabled flag is set.
 */
void Model
::EnableAllEvents()
{
  for ( auto event : m_eventMap )
  {
    event.second->m_enabled = true;
  }
}

// ----------------------------------------------------------------
/** Is event filtered out.
 *
 * This method determines if the specified event is filtered out of
 * the display.
 *
 * This interface is designed to support a more general event
 * filtering scheme, but for now the filter only suppresses events
 * with no occurrences.
 *
 * I can see a time when the filter is more script like and can apply
 * a wide range of predicates.
 *
 * @param[in] event - event id to check if filtered
 *
 * @retval true - display event
 * @retval false - do not display event
 */
bool Model
::IsEventDisplayable(ItemId_t event) const
{
  if (m_eventFilter) // is filter enabled
  {
    const_event_iterator_t ix = m_eventMap.find (event);
    size_t count = ix->second->NumOccurrences();
    return (count != 0);
  }

  // display by default
  return true;
}

// ----------------------------------------------------------------
/**
 * Report if model is dirty.
 */
bool Model
::ModelNeedsSave() const
{
  // If the model is empty, then no need to save
  if (m_eventMap.empty())
  {
    return false; // no need to save empty model
  }
  
  return m_modelDirty;
}
