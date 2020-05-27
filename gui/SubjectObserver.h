//
// File: subject_observer.h
//
// This file contains the interface for the classes used for the
// subject/observer pattern.
//

#if !defined (subject_observer__HH_)
#define subject_observer__HH_

#include "Subject.h"
#include "Observer.h"

// ------------------------------------------------------------------------
/** Combined subject and observer class.
*
* This class represents something that is both an observer and
* a subject. The main use for this class is as the base class in
* a system where the notification structure is a tree. When this object
* (observer) is notified from the subject it is watching, it forwards the
* notification to all the objects that are watching this object
* after the event is handled.
* A depth-first tree traversal is the result.
*
* If a breadth-first traversal is needed, you must use something else.
*
* Dynamics:
*
\msc
App,Subject,SubjectObserver,Observer;
--- [label="Subject notifies observers"];
App=>Subject [label="Notify", URL="\ref Subject::Notify()"];
Subject=>SubjectObserver [label="Notified()"];
SubjectObserver=>SubjectObserver [label="Update()", URL="\ref Observer::Update()"];
SubjectObserver=>SubjectObserver [label="PostUpdateHook()", URL="\ref Observer::PostUpdateHook()"];
SubjectObserver=>SubjectObserver [label="Notify()", URL="\ref Subject::Notify()"];
SubjectObserver=>Observer [label="Notified()"];
Observer=>Observer [label="Update()", URL="\ref Observer::Update()"];
Observer=>Observer [label="PostUpdateHook()", URL="\ref Observer::PostUpdateHook()"];
\endmsc
*
*/

class SubjectObserver
    : public Subject, public Observer
{
public:
    SubjectObserver() { }
    virtual ~SubjectObserver() { }

     std::ostream & ToStream (std::ostream & str) const override
    {
        dynamic_cast <const Subject *> (this)->ToStream(str);
        dynamic_cast <const Observer *> (this)->ToStream(str);
        return (str);
    }

protected:
    /** Forward notification to observers. This method forwards the notification
    * event to all watchers of this subject. This is the key method that
    * creates a notification tree hierarchy.
    * @param subj - subject initiating the notification
    * @param type - notification type code
    */
     void PostUpdateHook (Subject & subj, Subject::NotifyType_t type) override
    { Notify (type);} // forward the notification

private:

};

// ------------------------------------------------------------------------
/** Output operator.
*
* This is the standard output operator for the SubjectObserver
* class.
*
* @param str - stream to format on
* @param obj - object to format
* @returns The same output stream passed in
*/

inline std::ostream & operator<< (std::ostream & str, const SubjectObserver & obj)
{ return obj.ToStream (str); }

#endif
