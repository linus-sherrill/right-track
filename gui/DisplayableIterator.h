/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _DISPLAYABLEITERATOR_H_
#define _DISPLAYABLEITERATOR_H_

#include "Model.h"


// ----------------------------------------------------------------
/** Displayable iterator class.
 *
 * This class represents an iterator over the displayable subset of
 * events in the model. The events are iterated over in the order
 * specified by the display list.
 *
 * If the displayable filter is changed while an iterator is active,
 * the results will not be consistent.
 *
 */
class DisplayableIterator
{
public:

// ----------------------------------------------------------------
/** Constructor.
 *
 * Create a new iterator that points to the first displayable event.
 */
  DisplayableIterator();


// ----------------------------------------------------------------
/** Is current iterator position valid.
 *
 *
 */
  bool IsCurrentValid();


// ----------------------------------------------------------------
/** Start/restart iterator from beginning.
 *
 * This method returns the iterator to the first displayable event.
 */
  void Start();


// ----------------------------------------------------------------
/** Go to next displayable event.
 *
 * This method moves the iterator to the next displayable event, if
 * there is one. Attempting to advance the iterator past the end of
 * the list has no terrible side effects. It is still pointing at the
 * end (invalid element).
 *
 * @param count - number of elements to advance.
 *
 * @return true is returned if another displayable event is
 * found. false is returned indicating no more displayable events.
 */
  bool Next(int count = 1);


// ----------------------------------------------------------------
/** Get current event.
 *
 * This method returns the current displayable event.
 */
  EventDef::handle_t CurrentEvent();


// ----------------------------------------------------------------
/** Get current event id.
 *
 * This method returns event id of the current event.
 */
  ItemId_t CurrentItemId();


protected:
  // returns the next displayable, even if it is the current one
  void FindDisplayable();


private:
  std::vector< ItemId_t >::iterator m_currentIndex;
  Model * m_model;
};


#endif /* _DISPLAYABLEITERATOR_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
