/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#ifndef _MAIN_FRAME_APP_H_
#define _MAIN_FRAME_APP_H_

#include <Model.h>
#include <Observer.h>
#include <RT_MainFrame.h>

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
  : public RT_MainFrame, public Observer
{
public:
  MainFrameApp( wxWindow* parent,
                int id,
                const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxDEFAULT_FRAME_STYLE );

  virtual ~MainFrameApp() = default;

  Model*  GetModel() const { return Model::Instance(); }
  void InitializeEventTree();

  int Update( Subject& subj, Subject::NotifyType_t type ) override;

protected:
  void DoModelUpdate();
  void UpdateEventInfo();
  void UpdateTimeline();
  void UpdateCursorTimes();

  bool FileSave();
  bool FileSaveAs();

  // Event handlers
  void OnPaint( wxPaintEvent& event );
  void OnIdle( wxIdleEvent& evt );

  void FileOpenHandler( wxCommandEvent& event );
  void MergeEventsHandler( wxCommandEvent& event );
  void SaveHandler( wxCommandEvent& event );
  void SaveAsHandler( wxCommandEvent& event );
  void QuitHandler( wxCommandEvent& event );
  void AboutHandler( wxCommandEvent& event );

  void ColorPickerHandler( wxCommandEvent& event );
  void ZoomInHandler( wxCommandEvent& event );
  void ZoomOutHandler( wxCommandEvent& event );
  void ZoomFillHandler( wxCommandEvent& event );
  void CursorMenuHandler( wxCommandEvent& event );
  void ResetCursorHandler( wxCommandEvent& event );
  void RefreshHandler( wxCommandEvent& event );
  void EventDataHandler( wxCommandEvent& event );
  void EditDataSetAnnotationHandler( wxCommandEvent& event );

  void handle_move_top( wxCommandEvent& event );
  void handle_move_up( wxCommandEvent& event );
  void handle_move_down( wxCommandEvent& event );
  void handle_move_bottom( wxCommandEvent& event );

  void handle_filter_events( wxCommandEvent& event );
  void handle_enter_timeline( wxCommandEvent& event );

  void Curs1Set( wxCommandEvent& event );
  void Curs1Down( wxSpinEvent& event );
  void Curs1Up( wxSpinEvent& event );
  void Curs2Set( wxCommandEvent& event );
  void Curs2Down( wxSpinEvent& event );
  void Curs2Up( wxSpinEvent& event );

  void SortEventNameHandler( wxCommandEvent& event );
  void SortNumOccurHandler( wxCommandEvent& event );
  void SortAvgHandler( wxCommandEvent& event );
  void SortMaxDurationHandler( wxCommandEvent& event );
  void SortMinDurationHandler( wxCommandEvent& event );
  void SortPctActHandler( wxCommandEvent& event );

  DECLARE_EVENT_TABLE();

private:
  std::set< Subject::NotifyType_t > m_pendingUpdates;
}; // end class MainFrameApp

/**
 * This class represents the context data that is associated with the tree view
 * of the event names.
 */
class AppTreeData
  : public wxTreeItemData
{
public:
  // Event ID associated with this tree node
  ItemId_t m_itemId;
};

#endif /* _MAIN_FRAME_APP_H_ */
