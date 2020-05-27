//
// File: Subject.cpp
//

#include "Observer.h"
#include "Subject.h"

#include <algorithm>
#include <functional>

// ----------------------------------------------------------------------------

/** Destructor
 *
 * Cleanup this object before storage is released. We notify all observers
 * that we are being deleted by sending a NotifySubjectDelete code. This
 * approach is taken rather than just calling a method in the object to allow
 * the opportunity application to react to the deletion of a subject.
 */
Subject::
~Subject()
{
  // Let all observers know that subject is being deleted
  Notify( NotifySubjectDelete );
}

// ----------------------------------------------------------------------------

/** Attach observer to this subject.
 *
 * This method registers the
 * specified object as an observer of this subject.
 *
 * @param[in] obs - pointer to observer
 */
void
Subject
::Attach( Observer* obs )
{
  m_observerList.insert( obs );
  obs->Bind( this );

  return;
}

// ----------------------------------------------------------------------------

/** Detach object from subject.
 *
 * This method detaches the specified observer from this subject. The observers
 * object address is removed from our list of observers.
 *
 * @param[in] obs - observer to remove from list.
 */
void
Subject
::Detach( Observer* obs )
{
  // remove specified observer
  m_observerList.erase( obs );

  return;
}

// ----------------------------------------------------------------------------

/** Notifty all observers.
 *
 * This method notifies all attached observers
 * by calling their Update() method with the supplied update code.
 * The event code specifies what action caused this update/notify action.
 * This code is used by the observer to decide what to do.
 *
 * @param[in] n - event code for this notify
 * @returns The number of objects notified.
 */
int
Subject
::Notify( Subject::NotifyType_t n )
{
  list_iterator_t ix;
  int count { 0 };

  for ( auto obs : m_observerList )
  {
    obs->Notified( *this, n );
    count++; // increment count
  }

  // call post notify hook
  PostNotifyHook( n );

  return count;
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
Subject
::ToStream( std::ostream& str ) const
{
  list_const_iterator_t ix;

  str << "Subject @ " << this << " is watched by " <<
    (unsigned) m_observerList.size() << " observers." << std::endl;

  for ( ix = m_observerList.begin(); ix != m_observerList.end(); ix++ )
  {
    Observer* obs( *ix );
    str << "    Observer @ " << obs << std::endl;
  }

  return str;
}
