//
// File: Subject.h
//
// This file contains the interface for the classes used for the
// subject/observer pattern.
//

#if !defined (subject_HH_)
#define subject_HH_

#include <iostream>
#include <unordered_set>

//
// Partial definitions
//
class Observer;

// ----------------------------------------------------------------------------
/** Subject base class.
*
* This class represents the subject of the subject/observer pattern.
* Refer to the literature for a detailed description of how to
* use this pattern.
*
* In general, one or more observers registers with a subject
* using the Observer::Observe(subj) method. All observers
* are notified when the subject issues a Notify() call.
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
*
* Dynamics:
*
\msc
App,Subject,Observer,Observer_n;
--- [label="Start observing the subject"];
App=>Observer [label="Observe(Subject)", URL="\ref Observer::Observe()"];
Observer=>Subject [label="Attach()"];
Subject=>Observer [label="Bind()"];

--- [label="Subject notifies observers"];
App=>Subject [label="Notify()", URL="\ref Subject::Notify()"];
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
*
*/

class Subject
{
public:
    // -- TYPES --
    /** Define events causing the update.  These symbols define a basic
    * set of type codes. Your application will, most likely, require more
    * and different codes.  Use the NotifyType() method to create unique
    * notify type codes for your application.
    */
    enum NotifyType_t {
        NotifyGeneral = 1,
        NotifyReset,
        NotifyError,

        NotifySubjectDelete = 99,   // subject being deleted
        NotifyAppFirst = 100 // start of application specific code range
    };

    // -- CONSTRUCTOR --
    Subject () = default;
    virtual ~Subject ();


    // -- ACCESSORS --
    virtual std::ostream & ToStream (std::ostream & str) const;

  /** Create a user specified type code.
   *
   * This method returns a unique
   * notify type code based on the value supplied and with the
   * correct data type.
   *
   * Example usage:
   \code
   // define application specific Notify Type codes
   #define NT_foo Subject::NotifyType(1)
   #define NT_bar Subject::NotifyType(2)
   \endcode
   * @param val - user notify type value (from 1)
   * @returns A properly typed and encoded notify type code
   */
    static NotifyType_t NotifyType (unsigned int val)
    { return (NotifyType_t) (val + Subject::NotifyAppFirst); }

    // -- MODIFIERS --
    virtual int Notify (NotifyType_t n);


protected:
    /// Post notify hook method.  This method is called after all
    /// observers are notified.
    /// @param[in] type - event type code
    virtual void PostNotifyHook (Subject::NotifyType_t type) { }


private:
    friend class Observer;

    // -- TYPES --
    using list_t = std::unordered_set< Observer * >;
    using list_iterator_t = list_t::iterator;
    using list_const_iterator_t = list_t::const_iterator;

    void Attach (Observer * obs);
    void Detach (Observer * obs);

    /// List of observers.
    list_t m_observerList;
};


/** Output operator.
*
* This is the standard output operator for the Subject
* class.
*
* @param[in] str - stream to format on
* @param[in] obj - object to format
* @returns The same output stream passed in
*/

inline std::ostream & operator<< (std::ostream & str, const Subject & obj)
{ return obj.ToStream (str); }

#endif
