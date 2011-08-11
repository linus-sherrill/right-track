/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#include "TextEditDialogApp.h"


TextEditDialogApp::
TextEditDialogApp(wxWindow* parent,
                  int id,
                  const wxString& title,
                  const wxPoint& pos,
                  const wxSize& size,
                  long style)
  : TextEditDialog(parent, id, title, pos, size, style)
{

}


TextEditDialogApp::
~TextEditDialogApp()
{ }


void TextEditDialogApp::
SetText ( wxString const& txt)
{
  this->TextField->SetValue(txt);
}


wxString TextEditDialogApp::
GetText ( )
{
  return this->TextField->GetValue();
}


// ----------------------------------------------------------------
/** Handle clear button
 *
 *
 */
void TextEditDialogApp::
handle_clear(wxCommandEvent &event)
{
  this->TextField->Clear();
}
