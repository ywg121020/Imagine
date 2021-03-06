/*-------------------------------------------------------------------------
** Copyright (C) 2005-2010 Timothy E. Holy and Zhongsheng Guo
**    All rights reserved.
** Author: All code authored by Zhongsheng Guo.
** License: This file may be used under the terms of the GNU General Public
**    License version 2.0 as published by the Free Software Foundation
**    and appearing at http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
**
**    If this license is not appropriate for your use, please
**    contact holy@wustl.edu or zsguo@pcg.wustl.edu for other license(s).
**
** This file is provided WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**-------------------------------------------------------------------------*/

#ifndef DATA_ACQ_THREAD_HPP
#define DATA_ACQ_THREAD_HPP

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

//#include "andor_g.hpp"
#include "daq.hpp"
#include "camera_g.hpp"
//#include "cooke.hpp"
#include "positioner.hpp"
#include "ai_thread.hpp"
#include "di_thread.hpp"
#include "workerthread.h"
#include "waveform.h"

class QImage;
class NiDaqAi;
class Imagine;
class CurveData;

class DataAcqThread : public WorkerThread
{
    Q_OBJECT

public:
    DataAcqThread(QThread::Priority defaultPriority, Camera *cam = nullptr, Positioner *pos = nullptr, Imagine *parentImag = nullptr, QObject *parent = 0);
    ~DataAcqThread();

    static void genSquareSpike(int duration); //utility function

    void startAcq();
    void stopAcq(); //note: this func call is non-blocking

    bool preparePositioner(bool isForward = true, bool useTrigger = false);
    bool prepareDaqBuffered();

    // setter for the camera and positioner... use this instead of setting the var directly
    // yeah, I could make the var private... so can you!
    void setCamera(Camera *cam);
    void setPositioner(Positioner *pos);

    // de-globalized vars in need of a proper home...
    int curStimIndex = 0;
    vector<pair<int, int> > stimuli; //first: stim (valve), second: time (stack#)
    volatile bool isUpdatingImage;

    // this thread's camera
    Camera* pCamera = nullptr;

    // COMPUTER, ENHANCE: should probably move ownership up a couple levels at some point
    Positioner* pPositioner = nullptr;

    // the Imagine instance that owns this thread
    Imagine *parentImagine = nullptr;

    //the analog input acquisition thread owned by this thread
    AiThread *aiThread = NULL;
    DiThread *diThread = NULL;

    //intended camera params:
    int nStacksUser,
        nFramesPerStackUser;
    int nStacks,
        nFramesPerStack;
    double exposureTime, //in sec
        idleTimeBwtnStacks; //in sec
    double piezoTravelBackTime; //in sec
    int gain;
    int preAmpGainIdx;
    int horShiftSpeedIdx;
    int verShiftSpeedIdx;
    int verClockVolAmp;
    Camera::AcqTriggerMode acqTriggerMode;
	Camera::ExpTriggerMode expTriggerMode;
    int hstart, hend, vstart, vend; //binning params
    int angle;
    double umPerPxlXy;
    double mismatchRotation;
    int mismatchXTranslation;
    int mismatchYTranslation;

    QString preAmpGain, horShiftSpeed, verShiftSpeed; //for the purpose of saving in header only

    //real params used by the camera:
    double cycleTime; //in sec

    //piezo params:
    double piezoStartPosUm, piezoStopPosUm; //NOTE: in um
    bool isBiDirectionalImaging;
    CurveData *conPiezoWavData;
    CurveData *conShutterWavData;
    CurveData *conLaserWavData;
    CurveData *conTTL1WavData;
    CurveData *conTTL2WavData;
    ControlWaveform *conWaveData;
    bool isUsingWav = false;
    int sampleRate;

    //stimulus:
    QString stimFileContent;
    bool applyStim;

    //file saving params:
    QString headerFilename, aiFilename, diFilename;
    QString camFilename, commandFilename, sifFileBasename;

    //comment:
    QString comment;

    bool isLive;
    int idxCurStack;  //the stack we are working on. Since it's 0-based it's also the number of stacks finished so far
    bool ownPos = false;

    volatile int nEmittedSignal = 0;
    QMutex mutex2;
    void incEmittedSignal() {
        mutex2.lock();
        nEmittedSignal++;
        mutex2.unlock();
    }
    void decEmittedSignal() {
        mutex2.lock();
        nEmittedSignal--;
        mutex2.unlock();
    }
    QMutex mutex;
    void setIsUpdatingImage(bool value);

signals:
    void imageDisplayReady(const QImage &image, long idx);
    void newStatusMsgReady(const QString &str);
    void newLogMsgReady(const QString &str);
    void imageDataReady(const QByteArray &data16, long idx, int imageW, int imageH, int progress);
    void resetActuatorPosReady();
    void reapplyLaserSettings();

protected:
    void run();
    void run_acq_and_save();
    void run_live();

private:
    bool saveHeader(QString filename, DaqAi* ai, DaqDi* di = NULL, ControlWaveform *conWaveData = NULL);
    void fireStimulus(int valve);
    bool restart;
    bool abort;
    volatile bool stopRequested;
};

#endif  //DATA_ACQ_THREAD_HPP
