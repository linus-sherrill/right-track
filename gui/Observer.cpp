//
// File: Observer.cpp
//

#include "Observer.h"
#include "Subject.h"

#include <algorithm>
#include <functional>

// ----------------------------------------------------------------------------

/** Destructor.
 *
 */

Observer::
~Observer()
{
  // disconnect from subject
  Detach();
}

// ----------------------------------------------------------------------------

/** Observe a subject.
 *
 * This method establishes this object as an
 * observer of the specified subject.
 *
 * @param[in] subj - subject to observe
 *
 * @retval 0 - attached o.k.
 * @retval -1 - already attached to a subject
 */
void
Observer
::Observe( Subject& subj )
{
  subj.Attach( this );
}

// ----------------------------------------------------------------------------

/** Detach from all subjects.
 *
 * This method detaches us from all subjects we are currently observing.
 *
 * @retval 0 - detached o.k.
 * @retval -1 - not currently observing
 */
void
Observer
::Detach()
{
  // Send Detach() method to all subjects we are observing.
  std::for_each( m_subjectList.begin(), m_subjectList.end(),
                 std::bind2nd( std::mem_fun( &Subject::Detach ), this ) );

  m_subjectList.clear();    // erase all subjects in the list

  return;
}

// ----------------------------------------------------------------------------

/** Notified by a subject.
 *
 * This method is called when a subject is notifying us.
 * The event is forwarded to the Update() method and PostUpdateHook() handling
 * is done.
 * If an internal event code is detected, it is handled right here before is is
 * passed to the Update() method.
 *
 * @param subj - subject doing the notify
 * @param type - notification code
 */
int
Observer
::Notified( Subject& subj, Subject::NotifyType_t type )
{
  // If this is a subject-delete, we must update our
  // subject list to remove this entry.  Not doing so
  // can leave us open to catastrophic error.
  if ( type == Subject::NotifySubjectDelete )
  {
    m_subjectList.erase( &subj );   // take entry out of list
  }

  // Forward this event to the Update() method
  int status = Update( subj, type );

  // call hook method
  PostUpdateHook( subj, type );

  return status;
}

// ---------------------------------------------------------------------

/** Format this object
 *
 * This method formats this object to the specified stream.
 *
 * @param[in] str - stream to format on
 *
 * @returns Stream passed as input.
 */
std::ostream&
Observer
::ToStream( std::ostream& str ) const
{
  list_const_iterator_t ix;

  str << "Observer @ " << this << " is watching " <<
    (unsigned) m_subjectList.size() << " subjects." << std::endl;

  for ( ix = m_subjectList.begin(); ix != m_subjectList.end(); ix++ )
  {
    Subject* sub( *ix );
    str << "    Subject @ " << sub << std::endl;
  }

  return str;
}
