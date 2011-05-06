/*ckwg +5
 * Copyright 2010 by Kitware, Inc. All Rights Reserved. Please refer to
 * KITWARE_LICENSE.TXT for licensing information, or contact General Counsel,
 * Kitware, Inc., 28 Corporate Drive, Clifton Park, NY 12065.
 */



#ifndef _RIGHT_TRACK_SCOPED_CONTEXT_H_
#define _RIGHT_TRACK_SCOPED_CONTEXT_H_

#include "EventContext.h"

namespace RightTrack {

// ----------------------------------------------------------------
/** Scoped context.
 *
 * This class uses initialization is acquisition idiom to generate a
 * bounded event around a lexical scope.
 */
class ScopedContext
  : private boost::noncopyable
{
public:
  ScopedContext( EventContext & cont)
  :m_context(cont)
  { this->Push(); }

  virtual ~ScopedContext() { this->Pop(); }


  void Pop() { m_context.Pop(); }


private:
    void Push() { m_context.Push(); }

  EventContext & m_context;


}; // end class ScopedEventContext


// ----------------------------------------------------------------
/** Scoped Context helper.
 *
 * This macro defines a static event context and an automatic scoped
 * context control all in one line. Add this macro as the first line
 * in all methods that are to be tracked and the duration of the
 * method will be tracked automatically.
 *
 * This macro expansion is controlled by the SCOPED_CONTEXT_ENABLE
 * symbol. If it is defined, then the scoped tracking is enabled. If
 * it is disabled, then no code is generated.
 */
if SCOPED_CONTEXT_ENABLE

#define SCOPED_CONTEXT(N)                                               \
static ::RightTrack::EventContext __local_context__ (N);                \
::RightTrack::ScopedContext __scoped_context__(__local_context__)

#else

#define SCOPED_CONTEXT(N) do { } while(0)

#endif

} // end namespace

#endif /* _RIGHT_TRACK_SCOPED_CONTEXT_H_ */

// Local Variables:
// mode: c++
// fill-column: 70
// c-tab-width: 2
// c-basic-offset: 2
// c-basic-indent: 2
// c-indent-tabs-mode: nil
// end:

