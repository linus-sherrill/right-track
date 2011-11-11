/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef _MAIN_FRAME_APP_H_
#define _MAIN_FRAME_APP_H_

#include <MainFrame.h>

#include <Model.h>


//
// Partial types
//
class wxDC;


// ----------------------------------------------------------------
/** Application main frame.
 *
 *
 */
class MainFrameApp
  : public MainFrame
{
public:
  MainFrameApp(wxWindow* parent,
               int id,
               const wxString& title,
               const wxPoint& pos = wxDefaultPosition,
               const wxSize& size = wxDefaultSize,
               long style = wxDEFAULT_FRAME_STYLE);

  virtual ~MainFrameApp();

  Model * GetModel() const { return Model::Instance(); }
  void DrawNames ();

  void ModelUpdate(unsigned code);


protected:
  void DoModelUpdate(unsigned code);
  void UpdateEventInfo();
  void UpdateTimeline();
  void UpdateCursorTimes();

  // Event handlers
  void OnPaint(wxPaintEvent& event);
  void OnIdle(wxIdleEvent& evt);


  virtual void FileOpenHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void SaveHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void SaveAsHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void ColorPickerHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void QuitHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void AboutHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void ZoomInHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void ZoomOutHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void ZoomFillHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void CursorMenuHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void ResetCursorHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void RefreshHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void EventDataHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void EditDataSetAnnotationHandler(wxCommandEvent &event); // wxGlade: <event_handler>

  virtual void handle_move_top(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void handle_move_up(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void handle_move_down(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void handle_move_bottom(wxCommandEvent &event); // wxGlade: <event_handler>

  virtual void handle_filter_events(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void handle_enter_timeline(wxCommandEvent &event); // wxGlade: <event_handler>

  virtual void Curs1Set(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void Curs1Down(wxSpinEvent &event); // wxGlade: <event_handler>
  virtual void Curs1Up(wxSpinEvent &event); // wxGlade: <event_handler>
  virtual void Curs2Set(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void Curs2Down(wxSpinEvent &event); // wxGlade: <event_handler>
  virtual void Curs2Up(wxSpinEvent &event); // wxGlade: <event_handler>

  virtual void SortEventNameHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void SortNumOccurHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void SortAvgHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void SortMaxDurationHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void SortMinDurationHandler(wxCommandEvent &event); // wxGlade: <event_handler>
  virtual void SortPctActHandler(wxCommandEvent &event); // wxGlade: <event_handler>

  DECLARE_EVENT_TABLE();


private:
  unsigned m_pendingUpdate;

}; // end class MainFrameApp


#endif /* _MAIN_FRAME_APP_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
