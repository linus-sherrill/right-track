/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _EVENTTABLEAPP_H_
#define _EVENTTABLEAPP_H_

#include <EventTable.h>

#include <DisplayableIterator.h>


// ----------------------------------------------------------------
/**
 *
 *
 */
class EventTableApp
  : public EventTable
{
public:
  EventTableApp(wxWindow* parent, int id, const wxString& title,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);
  virtual ~EventTableApp();

  void InitGrid(DisplayableIterator & event_it);

protected:
    virtual void handle_done(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void handle_save(wxCommandEvent &event); // wxGlade: <event_handler>
    virtual void handle_cancel(wxCommandEvent &event); // wxGlade: <event_handler>


private:
  DisplayableIterator m_dataSource;


}; // end class EventTableApp


#endif /* _EVENTTABLEAPP_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
