//
//  Serialize.h
//  right_track
//
//  Created by Linus Sherrill on 5/14/20.
//

#ifndef RT_GUI_Serialize_h
#define RT_GUI_Serialize_h

#include <EventRecords.h>

#include <wx/wx.h>

#include <cereal/cereal.hpp>


// ----------------------------------------------------------------
template<class Archive>
void save(Archive& archive,
          wxString const& obj)
{
  std::string str( obj );
  archive( CEREAL_NVP(str) );
}

template<class Archive>
void load(Archive& archive,
          wxString& obj)
{
  std::string str;
  archive( CEREAL_NVP(str) );
  obj = wxString(str);
}


// ----------------------------------------------------------------
template<class Archive>
void save(Archive& archive,
          wxColour const& obj)
{
  unsigned int red = obj.Red();
  unsigned int green = obj.Green();
  unsigned int blue = obj.Blue();
  archive( CEREAL_NVP(red),
          CEREAL_NVP(green),
          CEREAL_NVP(blue) );
}


template<class Archive>
void load(Archive& archive,
          wxColour& obj)
{
  unsigned int red, green, blue;
  archive( CEREAL_NVP(red),
          CEREAL_NVP(green),
          CEREAL_NVP(blue) );
  obj = wxColour( red, green, blue );
}


// ----------------------------------------------------------------
template<class Archive>
void save(Archive& archive,
          wxBrush const& obj)
{
  wxColour color = obj.GetColour();
  wxBrushStyle style = obj.GetStyle();
  
  archive( CEREAL_NVP(color),
          CEREAL_NVP(style) );
}


template<class Archive>
void load(Archive& archive,
          wxBrush& obj)
{
  wxColour color;
  wxBrushStyle style;
  
  archive( CEREAL_NVP(color),
          CEREAL_NVP(style) );
  obj = wxBrush( color, style );
}


// ----------------------------------------------------------------
template<class Archive>
void save(Archive& archive,
          wxPen const& obj)
{
  wxColour color = obj.GetColour();
  int width = obj.GetWidth();
  wxPenStyle style = obj.GetStyle();
  
  archive( CEREAL_NVP(color),
          CEREAL_NVP(width),
          CEREAL_NVP(style) );
}


template<class Archive>
void load(Archive& archive,
          wxPen& obj)
{
  wxColour color;
  int width;
  wxPenStyle style;
  
  archive( CEREAL_NVP(color),
          CEREAL_NVP(width),
          CEREAL_NVP(style) );
  obj = wxPen( color,
              width,
              style );
}

#endif /* RT_GUI_Serialize_h */
