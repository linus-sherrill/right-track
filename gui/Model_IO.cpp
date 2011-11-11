/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include "Model.h"

#include <vcl_string.h>
#include <vsl/vsl_binary_io.h>
#include <vsl/vsl_vector_io.h>
#include <vsl/vsl_map_io.h>


//
// Version number we are writing
//
#define VERSION 1

namespace {

//
// Local prototypes
//
void vsl_b_write(vsl_b_ostream& s, EventDef const& v);
void vsl_b_write(vsl_b_ostream& s, BoundedEventDef const& v);
void vsl_b_write(vsl_b_ostream& s, DiscreteEventDef const& v);
void vsl_b_write(vsl_b_ostream& s, BoundedEventStatistics const& v);
void vsl_b_write(vsl_b_ostream& s, DiscreteEventStatistics const& v);
void vsl_b_write(vsl_b_ostream& s, BoundedOccurrence const& v);
void vsl_b_write(vsl_b_ostream& s, DiscreteOccurrence const& v);

void vsl_b_write(vsl_b_ostream& s, wxColour const& v);
void vsl_b_write(vsl_b_ostream& s, wxColourData const& v);
void vsl_b_write(vsl_b_ostream& s, wxPen const& v);
void vsl_b_write(vsl_b_ostream& s, wxBrush const& v);
void vsl_b_write(vsl_b_ostream& s, wxString const& v);


void vsl_b_read(vsl_b_istream& s, EventDef & v);
void vsl_b_read(vsl_b_istream& s, BoundedEventDef & v);
void vsl_b_read(vsl_b_istream& s, DiscreteEventDef & v);
void vsl_b_read(vsl_b_istream& s, BoundedEventStatistics & v);
void vsl_b_read(vsl_b_istream& s, DiscreteEventStatistics & v);
void vsl_b_read(vsl_b_istream& s, BoundedOccurrence & v);
void vsl_b_read(vsl_b_istream& s, DiscreteOccurrence & v);

void vsl_b_read(vsl_b_istream& s, wxColour & v);
void vsl_b_read(vsl_b_istream& s, wxColourData & v);
void vsl_b_read(vsl_b_istream& s, wxPen & v);
void vsl_b_read(vsl_b_istream& s, wxBrush & v);
void vsl_b_read(vsl_b_istream& s, wxString & v);

}


// ----------------------------------------------------------------
/** Write model contents to a file.
 *
 *
 */
bool Model::
WriteModel(wxString const& file)
{
  // Write model header info
  // - version number
  // - date saved
  // - model annotation text
  // - color scheme
  // -
  // Event lists
  // - event map
  // - display order

  // open file
  vcl_ofstream * outStream = new vcl_ofstream;
  outStream->open(file.char_str());

  vsl_b_ostream bstream(outStream);

  // Write header information
  int version(VERSION);
  vsl_b_write (bstream, 1);     // version number

  // current date- TBD

  vsl_b_write (bstream, m_modelAnnotation);

  // colour scheme
  vsl_b_write (bstream, m_defaultBaselineColor);
  vsl_b_write (bstream, m_defaultLineColor);
  vsl_b_write (bstream, m_defaultEventColor);
  vsl_b_write (bstream, m_startEventColor);
  vsl_b_write (bstream, m_endEventColor);
  vsl_b_write (bstream, m_selectColor);
  vsl_b_write (bstream, m_commentMarkerColor);

  vsl_b_write (bstream, m_persistentColourData);

  // event drawing order vector
  vsl_b_write (bstream, m_drawOrder);

  // Write number of entries in map
  int map_size = m_eventMap.size();
  vsl_b_write (bstream, map_size);

  // Write event map
  const_event_iterator_t it;
  for (it = m_eventMap.begin(); it != m_eventMap.end(); it++)
  {
    if (it->second->GetBoundedEvent() != 0)
    {
      BoundedEventDef const * ptr = it->second->GetBoundedEvent();

      vsl_b_write (bstream, Event::ET_BOUNDED_EVENT); // write object type
      vsl_b_write (bstream, it->first); // write event id
      vsl_b_write (bstream, *ptr);
    }
    else if (it->second->GetDiscreteEvent() != 0)
    {
      DiscreteEventDef const * ptr = it->second->GetDiscreteEvent();

      vsl_b_write (bstream, Event::ET_DISCRETE_EVENT); // write object type
      vsl_b_write (bstream, it->first); // write event id
      vsl_b_write (bstream, *ptr);
    }
    else
    {
      // unexpected type
      //+ TBD
    }
  }

  // Close file
  if (outStream)
  {
    outStream->close();
  }

} // end WriteModel


// ================================================================
namespace {

//
// Write functions for event data base
//
void vsl_b_write(vsl_b_ostream& s, EventDef const& v)
{
  vsl_b_write (s, v.m_eventBaselinePen);
  vsl_b_write (s, v.m_time);
  vsl_b_write (s, v.m_eventId);
  vsl_b_write (s, v.m_color);
  vsl_b_write (s, v.m_eventName);
  vsl_b_write (s, v.m_groupName);
  vsl_b_write (s, v.GetUserComment() );
}


void vsl_b_write(vsl_b_ostream& s, BoundedEventDef const& v)
{
  // write out base class
  vsl_b_write (s, * (EventDef *)(&v) );
  vsl_b_write (s, v.m_stats);

  // write out number of occurrences
  int count = v.m_list.size();
  vsl_b_write (s, count);

  // Write out list of occurrences
  EventDef::const_iterator_t it;
  for (it = v.m_list.begin(); it != v.m_list.end(); it++)
  {
    vsl_b_write (s, (*it)->GetBoundedOccurrence());
  }

}


void vsl_b_write(vsl_b_ostream& s, DiscreteEventDef const& v)
{
  // write out base class
  vsl_b_write (s, * (EventDef *)(&v) );
  vsl_b_write (s, v.m_stats);

  // write out number of occurrences
  int count = v.m_list.size();
  vsl_b_write (s, count);

  // write list of occurrences
  EventDef::const_iterator_t it;
  for (it = v.m_list.begin(); it != v.m_list.end(); it++)
  {
    vsl_b_write (s, (*it)->GetDiscreteOccurrence());
  }

}


void vsl_b_write(vsl_b_ostream& s, BoundedEventStatistics const& v)
{
  vsl_b_write (s, v.m_count);
  vsl_b_write (s, v.m_minDuration );
  vsl_b_write (s, v.m_maxDuration );
  vsl_b_write (s, v.m_avgDuration );
  vsl_b_write (s, v.m_stdDuration );
  vsl_b_write (s, v.m_activePct );
}


void vsl_b_write(vsl_b_ostream& s, DiscreteEventStatistics const& v)
{
  vsl_b_write (s, v.m_count);
}


void vsl_b_write(vsl_b_ostream& s, BoundedOccurrence const& v)
{
  // Write base class data
  vsl_b_write (s, v.m_eventPid );
  vsl_b_write (s, v.GetUserComment() );

  // write derived class data
  vsl_b_write (s, v.m_startTime );
  vsl_b_write (s, v.m_startData );
  vsl_b_write (s, v.m_endTime );
  vsl_b_write (s, v.m_endData );
  vsl_b_write (s, v.m_startMarkerPen );
  vsl_b_write (s, v.m_startMarkerBrush );
  vsl_b_write (s, v.m_eventDurationPen );
  vsl_b_write (s, v.m_endMarkerPen );
  vsl_b_write (s, v.m_endMarkerBrush );
}


void vsl_b_write(vsl_b_ostream& s, DiscreteOccurrence const& v)
{
  // Write base class data
  vsl_b_write (s, v.m_eventPid );
  vsl_b_write (s, v.GetUserComment() );

  // write derived class data
  vsl_b_write (s, v.m_eventTime );
  vsl_b_write (s, v.m_eventData );
  vsl_b_write (s, v.m_eventMarkerPen );
  vsl_b_write (s, v.m_eventMarkerBrush );
}



// writers for wxWidgets data types
// - wxColour, wxPen, wxBrush, wxString -> vcl_string
//
//
void vsl_b_write(vsl_b_ostream& s, wxColour const& v)
{
  vsl_b_write (s, v.GetAsString(wxC2S_CSS_SYNTAX) );
}


void vsl_b_write(vsl_b_ostream& s, wxColourData const& v)
{
  for (int i = 0; i < 15; i++)
  {
    wxColour colour = const_cast< wxColourData & >(v).GetCustomColour(i);
    vsl_b_write (s, colour);
  }
}


void vsl_b_write(vsl_b_ostream& s, wxPen const& v)
{
  // write color, width, pattern
  vsl_b_write (s, v.GetColour() );
  vsl_b_write (s, v.GetWidth() );
  vsl_b_write (s, v.GetStyle() );
}


void vsl_b_write(vsl_b_ostream& s, wxBrush const& v)
{
  // write color, width, pattern
  vsl_b_write (s, v.GetColour() );
  vsl_b_write (s, v.GetStyle() );
}


void vsl_b_write(vsl_b_ostream& s, wxString const& v)
{
  vcl_string str( v.char_str() );
  vsl_b_write (s, str );
}

} // end namespace

// ================================================================
// ---- Reading functions -----
// ================================================================


// ----------------------------------------------------------------
/** Read model contents from file.
 *
 * Assume that the model has been cleaned of all previous information.
 */
bool Model::
ReadModel(wxString const& file)
{
  // open file
  vcl_ifstream * inStream = new vcl_ifstream;
  inStream->open(file.char_str());

  vsl_b_istream bstream(inStream);

  int version(-1);
  vsl_b_read (bstream, version);

  // Validate version nunber.  If we have to deal with more than one
  // version number, well, then ...
  if (version != VERSION)
  {
    return false;
  }

  // date written - TBD

  vsl_b_read (bstream, m_modelAnnotation);

  // colour scheme
  vsl_b_read (bstream, m_defaultBaselineColor);
  vsl_b_read (bstream, m_defaultLineColor);
  vsl_b_read (bstream, m_defaultEventColor);
  vsl_b_read (bstream, m_startEventColor);
  vsl_b_read (bstream, m_endEventColor);
  vsl_b_read (bstream, m_selectColor);
  vsl_b_read (bstream, m_commentMarkerColor);

  vsl_b_read (bstream, m_persistentColourData);

  // event drawing order vector
  vsl_b_read (bstream, m_drawOrder);

  // Write number of entries in map
  int map_size(-1);
  vsl_b_read (bstream, map_size);

  if (map_size < 0)
  {
    return false; // size error
  }

  // read event map
  for (int i = 0; i < map_size; i++)
  {
    Event::EventType_t event_type;
    vsl_b_read (bstream, * (int *) (&event_type)); // read object type

    ItemId_t item_id;
    vsl_b_read (bstream, item_id); // read event id

    // read specific event type
    if (event_type == Event::ET_BOUNDED_EVENT)
    {
      BoundedEventDef * event_def = new BoundedEventDef;
      vsl_b_read (bstream, *event_def);

      m_eventMap[item_id] = EventDef::handle_t(event_def);
    }
    else if (event_type == Event::ET_DISCRETE_EVENT)
    {
      DiscreteEventDef * event_def = new DiscreteEventDef;
      vsl_b_read (bstream, *event_def);

      m_eventMap[item_id] = EventDef::handle_t(event_def);
    }
    else
    {
      // unexpected type
      // do something
    }

    // update model file name
    m_modelFileName = *file;
  } // end for

}


namespace {


void vsl_b_read(vsl_b_istream& s, EventDef & v)
{
  vsl_b_read (s, v.m_eventBaselinePen);
  vsl_b_read (s, v.m_time);
  vsl_b_read (s, v.m_eventId);
  vsl_b_read (s, v.m_color);
  vsl_b_read (s, v.m_eventName);
  vsl_b_read (s, v.m_groupName);

  wxString comment;
  vsl_b_read (s, comment );
  v.SetUserComment(comment);
}


void vsl_b_read(vsl_b_istream& s, BoundedEventDef & v)
{
  // read out base class
  vsl_b_read (s, * (EventDef *)(&v) );
  vsl_b_read (s, v.m_stats);

  // read number of occurrences
  int count;
  vsl_b_read (s, count);

  // Read out list of events
  for (int i = 0; i < count; i++)
  {
    BoundedOccurrence * occur = new BoundedOccurrence;
    vsl_b_read (s, *occur);

    v.m_list.push_back (BaseOccurrence::handle_t(occur) );
  } // end for

}


void vsl_b_read(vsl_b_istream& s, DiscreteEventDef & v)
{
  // read out base class
  vsl_b_read (s, * (EventDef *)(&v) );
  vsl_b_read (s, v.m_stats);

  // read out number of occurrences
  int count;
  vsl_b_read (s, count);

  // read list of occurrences
  for (int i = 0; i < count; i++)
  {
    DiscreteOccurrence * occur = new DiscreteOccurrence;
    vsl_b_read (s, *occur);

    v.m_list.push_back(BaseOccurrence::handle_t(occur) );
  }

}


void vsl_b_read(vsl_b_istream& s, BoundedEventStatistics & v)
{
  vsl_b_read (s, v.m_count);
  vsl_b_read (s, v.m_minDuration );
  vsl_b_read (s, v.m_maxDuration );
  vsl_b_read (s, v.m_avgDuration );
  vsl_b_read (s, v.m_stdDuration );
  vsl_b_read (s, v.m_activePct );
}


void vsl_b_read(vsl_b_istream& s, DiscreteEventStatistics & v)
{
  vsl_b_read (s, v.m_count);
}


void vsl_b_read(vsl_b_istream& s, BoundedOccurrence & v)
{
  // Read base class data
  vsl_b_read (s, v.m_eventPid );

  wxString comment;
  vsl_b_read (s, comment );
  v.SetUserComment(comment);

  // read derived class data
  vsl_b_read (s, v.m_startTime );
  vsl_b_read (s, v.m_startData );
  vsl_b_read (s, v.m_endTime );
  vsl_b_read (s, v.m_endData );
  vsl_b_read (s, v.m_startMarkerPen );
  vsl_b_read (s, v.m_startMarkerBrush );
  vsl_b_read (s, v.m_eventDurationPen );
  vsl_b_read (s, v.m_endMarkerPen );
  vsl_b_read (s, v.m_endMarkerBrush );
}


void vsl_b_read(vsl_b_istream& s, DiscreteOccurrence & v)
{
  // Read base class data
  vsl_b_read (s, v.m_eventPid );

  wxString comment;
  vsl_b_read (s, comment );
  v.SetUserComment(comment);

  // read derived class data
  vsl_b_read (s, v.m_eventTime );
  vsl_b_read (s, v.m_eventData );
  vsl_b_read (s, v.m_eventMarkerPen );
  vsl_b_read (s, v.m_eventMarkerBrush );
}


// readers for wxWidgets data types
// - wxColour, wxPen, wxBrush, wxString -> vcl_string
//
//
void vsl_b_read(vsl_b_istream& s, wxColour & v)
{
  wxString str;
  vsl_b_read (s, str );
  v = wxColour (str);
}


void vsl_b_read(vsl_b_istream& s, wxColourData & v)
{
  for (int i = 0; i < 15; i++)
  {
    wxColour colour;
    vsl_b_read (s, colour );
    v.SetCustomColour(i, colour);
  }
}


void vsl_b_read(vsl_b_istream& s, wxPen & v)
{
  // read color, width, pattern
  wxColour colour;
  int width;
  int style;

  vsl_b_read (s, colour );
  vsl_b_read (s, width );
  vsl_b_read (s, style );

  v = wxPen (colour, width, style);
}


void vsl_b_read(vsl_b_istream& s, wxBrush & v)
{
  wxColour colour;
  int style;

  vsl_b_read (s, colour );
  vsl_b_read (s, style );

  v = wxBrush (colour, style);
}


void vsl_b_read(vsl_b_istream& s, wxString & v)
{
  vcl_string str;

  vsl_b_read (s, str );
  v = wxString (str.c_str(), wxConvUTF8);
}

} // end namespace
