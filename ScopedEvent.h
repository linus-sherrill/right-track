/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */


#ifndef _RIGHT_TRACK_SCOPED_EVENT_
#define _RIGHT_TRACK_SCOPED_EVENT_


namespace RightTrack {
  namespace Internal {
    class Event;
  } // end namespace

#if defined(_MSC_VER)
#if _MSC_VER >= 1300
      #define __EVENT_NAME__ __FUNCSIG__
#endif
#else
#if defined(__GNUC__)
      #define __EVENT_NAME__ __PRETTY_FUNCTION__
#endif
#endif
#if !defined(__EVENT_NAME__)
#define __EVENT_NAME__ ""
#endif


// ----------------------------------------------------------------
/** Scoped Event.
 *
 * This class uses initialization is acquisition idiom to generate a
 * bounded event around a lexical scope.
 */
class ScopedEvent
  : boost::noncopyable
{
public:
  ScopedEvent ( ::RightTrack::Internal::Event & ev )
    : m_event(ev) { this->Start(); }

  virtual ~ScopedEvent() { this->End(); }

  // for manual control of event
  void End()   { m_event.End(); }


private:
  void Start() { m_event.Start(); }

  ::RightTrack::Internal::Event & m_event;
}; // end class ScopedBoundedEvent

} // end name space


// ----------------------------------------------------------------
/** Scoped event helper.
 *
 * This macro defines a static bounded event and an automatic scoped
 * event control all in one line. Add this macro as the first line in
 * all methods that are to be tracked and the duration of the method
 * will be tracked automatically.
 *
 * This macro expansion is controlled by the SCOPED_EVENT_ENABLE
 * symbol. If it is defined, then the scoped tracking is enabled. If
 * it is disabled, then no code is generated.
 */
#if 1 // SCOPED_EVENT_ENABLE

#define SCOPED_EVENT                                                 \
  static ::RightTrack::BoundedEvent __local_event__ (__EVENT_NAME__);  \
  ::RightTrack::ScopedEvent __scoped_event__ (__local_event__)

#else

#define SCOPED_EVENT do { } while (0)

#endif

#endif /* _RIGHT_TRACK_SCOPED_EVENT_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:
