/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _TEXTEDITDIALOGAPP_H_
#define _TEXTEDITDIALOGAPP_H_

#include "gen_files/TextEditDialog.h"


// ----------------------------------------------------------------
/**
 *
 *
 */
class TextEditDialogApp
: public TextEditDialog
{
public:
  TextEditDialogApp(wxWindow* parent, int id,
                    const wxString& title,
                    const wxPoint& pos=wxDefaultPosition,
                    const wxSize& size=wxDefaultSize,
                    long style=wxDEFAULT_DIALOG_STYLE);
  virtual ~TextEditDialogApp();

  void SetText (wxString const& text);
  wxString GetText();


protected:
    virtual void handle_clear(wxCommandEvent &event); // wxGlade: <event_handler>



private:




}; // end class TextEditDialogApp


#endif /* _TEXTEDITDIALOGAPP_H_ */
