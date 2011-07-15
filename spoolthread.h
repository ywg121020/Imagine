#ifndef SPOOLTHREAD_H
#define SPOOLTHREAD_H

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;

#include <assert.h>

#include "lockguard.h"

#include "fast_ofstream.hpp"
#include "circbuf.hpp"


#include <QThread>

#include <QMutex>
#include <QThread>
#include <QWaitCondition>


class SpoolThread: public QThread {
   Q_OBJECT
private:
   FastOfstream *ofsSpooling;
   volatile bool shouldStop; //todo: do we need a lock to guard it?

public:
   SpoolThread(FastOfstream *ofsSpooling, QObject *parent = 0)
      : QThread(parent){
      this->ofsSpooling=ofsSpooling;
      shouldStop=false;
   }
   ~SpoolThread(){}

   void requestStop(){
      shouldStop=true;
   }

   void run(){
#if defined(_DEBUG)
      cerr<<"enter cooke spooling thread run()"<<endl;
#endif

      while(true){
         if(shouldStop) break;

      }//while,

#if defined(_DEBUG)
      cerr<<"leave cooke spooling thread run()"<<endl;
#endif
   }//run(),
};//class, SpoolThread


#endif // SPOOLTHREAD_H
