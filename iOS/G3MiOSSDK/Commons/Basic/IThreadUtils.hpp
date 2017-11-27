//
//  IThreadUtils.hpp
//  G3MiOSSDK
//
//  Created by Diego Gomez Deck on 30/08/12.
//
//

#ifndef __G3MiOSSDK__IThreadUtils__
#define __G3MiOSSDK__IThreadUtils__

#include <stdlib.h>
#include "TimeInterval.hpp"

class G3MContext;
class GTask;
class GAsyncTask;


class IThreadUtils {
protected:
#ifdef C_CODE
  const G3MContext* _context;
#endif
#ifdef JAVA_CODE
  protected G3MContext _context;
#endif

public:
  IThreadUtils() :
  _context(NULL)
  {
  }

  virtual void onResume(const G3MContext* context) = 0;

  virtual void onPause(const G3MContext* context) = 0;

  virtual void onDestroy(const G3MContext* context) = 0;

  virtual void initialize(const G3MContext* context);

  virtual ~IThreadUtils() {
  }

  virtual void invokeInRendererThread(GTask* task,
                                      bool autoDelete) const = 0;

  virtual void invokeInBackground(GTask* task,
                                  bool autoDelete) const = 0;

  void invokeAsyncTask(GAsyncTask* task,
                       bool autodelete) const;
  
  virtual void invokeAfterInRendererThread(
                                   const TimeInterval& time,
                                   GTask* task,
                                   bool autoDelete) const = 0;
  
};

#endif
