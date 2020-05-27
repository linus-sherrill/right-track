//
// File: Observer.h
//
// This file contains the interface for the classes used for the
// subject/observer pattern.
//

#if !defined (observer__HH_)
#define observer__HH_

#include "Subject.h"
#include <iostream>
#include <unordered_set>

//
// Partial definitions
//
class Subject;

// ----------------------------------------------------------------------------
/** Observer base class.
*
* This class represents an observer in the subject/observer pattern.
*
* Refer to the literature for a detailed description of how to
* use this pattern.
*
* In general, an observer registers with one or more subjects using the
* Observer::Observe(subj) method.
* All registered observers are called each time the subject invokes the Notify()
* method.  The implementation of the Update() method in the derived class
* must decide what to do with
* each event type code (handle or ignore).
*
* In this implementation of the subject-observer pattern, both the subject
* and observer are linked together. The subject has a list of those that are
* observing and the observer has a list of subjects that are being watched.
* The lists are updated when either a subject or an observer has been deleted.
* This is done to keep the subjects list of observers up to date. We do not
* want to Notify an observer that has been deleted.
*
* This implementation is *not* thread safe.
*
* Dynamics:
*
\msc
App,Subject,Observer,Observer_n;
--- [label="Start observing the subject"];
App=>Observer [label="Observe()", URL="\ref Observer::Observe()"];
Observer=>Subject [label="Attach()"];
Subject=>Observer [label="Bind()"];

--- [label="Subject notifies observers"];
App=>Subject [label="Notify", URL="\ref Subject::Notify()"];
Subject=>Observer [label="Notified()"];
Observer=>Observer [label="Update()", URL="\ref Observer::Update()"];
Observer=>Observer [label="PostUpdateHook()", URL="\ref Observer::PostUpdateHook()"];
... [label="For all observers"];
Subject=>Observer_n [label="Notified()"];
Observer_n=>Observer_n [label="Update()", URL="\ref Observer::Update()"];
Observer_n=>Observer_n [label="PostUpdateHook()", URL="\ref Observer::PostUpdateHook()"];
Subject=>Subject [label="PostNotifyHook()", URL="\ref Subject::PostNotifyHook()"];

--- [label="Observer is deleted"];
App=>Observer [label="delete"];
Observer=>Observer [label="Detach()"];
Observer=>Subject [label="Detach()"];
... [label="For all subjects being observed"];

--- [label="Subjects is deleted"];
App=>Subject [label="delete"];
Subject=>Subject [label="Notify(NotifySubjectDelete)"];
Subject=>Observer [label="Notified(NotifySubjectDelete)"];
Observer=>Observer [label="Update(NotifySubjectDelete)", URL="\ref Observer::Update()"];
Observer=>Observer [label="PostUpdateHook(NotifySubjectDelete)", URL="\ref Observer::PostUpdateHook()"];
... [label="For all observers"];
Subject=>Observer_n [label="Notified(NotifySubjectDelete)"];
Observer_n=>Observer_n [label="Update(NotifySubjectDelete)", URL="\ref Observer::Update()"];
Observer_n=>Observer_n [label="PostUpdateHook(NotifySubjectDelete)", URL="\ref Observer::PostUpdateHook()"];
\endmsc
*
* Example:
*
\code
//+ TBD
\endcode
*
*/

class Observer
{
public:
    // -- CONSTRUCTORS --
  Observer() = default;
    virtual ~Observer();

    // -- ACCESSORS --
    virtual std::ostream & ToStream (std::ostream & str) const;

protected:
    void Observe (Subject & s);
    virtual void Detach ();

    /** Hook for post update operation.  This method is called after the
    * Notify() method returns. This is useful for classes that are Subjects
    * and Observers where the notify event must be propogated to another
    * set of Observers.
    * @param[in] subj - reference to subject that generated event
    * @param[in] type - event type code
    */
    virtual void PostUpdateHook (Subject & subj, Subject::NotifyType_t type) { }

    /** Update action method.  This method is called when the subject being
    * observed generates an event with the Notify method.
    * All derived classes must implement this
    * method since it is pure virtual.
    *
    * Note that this observer is automatically detached from the subject
    * when a Subject::NotifySubjectDelete notify type code is received.
    *
    * @param[in] subj - reference to subject that generated event
    * @param[in] type - event type code
    * @retval currently ignored
    */
    virtual int Update (Subject & subj, Subject::NotifyType_t type) = 0;

private:
    // -- TYPES --
    using list_t = std::unordered_set< Subject * >;
    using list_iterator_t = list_t::iterator;
    using list_const_iterator_t = list_t::const_iterator;

    // Allow access by Subject class
    friend class Subject;

    int Notified (Subject & s, Subject::NotifyType_t t);

    /// Bind this observer to a subject.  This method binds this observer
    /// to the specified subject.  This is done to make sure that the
    ///observer is notified when the observer is destroyed.
    /// @param[in] s - subject to be bound to
    void Bind (Subject * s) { m_subjectList.insert(s); }

    /// Subjects that we are observing.
    list_t m_subjectList;
};



/** Output operator.
*
* This is the standard output operator for the Observer
* class.
*
* @param str - stream to format on
* @param obj - object to format
* @returns The same output stream passed in
*/

inline std::ostream & operator<< (std::ostream & str, const Observer & obj)
{ return obj.ToStream (str); }

#endif
