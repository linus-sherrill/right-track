/*ckwg +5
 * Copyright 2011 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */

#include "DisplayableIterator.h"

using namespace ::RightTrack;
using namespace ::RightTrack::Internal;

// ----------------------------------------------------------------

/** Constructor.
 *
 * Create a new iterator that points to the first displayable event.
 */
DisplayableIterator
::DisplayableIterator()
  : m_model( Model::Instance() )
{
  Start();
  FindDisplayable();
}

// ----------------------------------------------------------------

/** Is current iterator position valid.
 *
 *
 */
bool
DisplayableIterator
::IsCurrentValid()
{
  return ( m_currentIndex != m_model->m_drawOrder.end() );
}

// ----------------------------------------------------------------

/** Start/restart iterator from beginning.
 *
 * This method returns the iterator to the first displayable event.
 */
void
DisplayableIterator
::Start()
{
  m_currentIndex = m_model->m_drawOrder.begin();
  FindDisplayable();
}

// ----------------------------------------------------------------

/** Go to next displayable event.
 *
 * This method moves the iterator to the next displayable event, if
 * there is one.
 *
 * @return true is returned if another displayable event is
 * found. false is returned indicating no more displayable events.
 */
bool
DisplayableIterator
::Next( int count )
{
  while ( IsCurrentValid() && ( count > 0 ) )
  {
    m_currentIndex++;
    FindDisplayable();
    count--;
  } // end for

  return IsCurrentValid();
}

// ----------------------------------------------------------------

/** Get current event.
 *
 * This method returns the current displayable event.
 */
EventDef::handle_t
DisplayableIterator
::CurrentEvent()
{
  if ( !IsCurrentValid() )
  {
    return EventDef::handle_t(); // return invalid handle
  }

  return m_model->m_eventMap[ *m_currentIndex ];
}

// ----------------------------------------------------------------

/** Get current event id.
 *
 * This method returns event id of the current event.
 */
ItemId_t
DisplayableIterator
::CurrentItemId()
{
  if ( !IsCurrentValid() )
  {
    return 0;
  }

  return *m_currentIndex;
}

// returns the next displayable, even if it is the current one
void
DisplayableIterator
::FindDisplayable()
{
  while ( IsCurrentValid() && ( !m_model->IsEventDisplayable( CurrentItemId() ) ) )
  {
    m_currentIndex++;
  }
}
