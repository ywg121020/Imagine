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

#include "andor_g.hpp"
#include "daq.hpp"

extern QString positionerType;

class QImage;
class NiDaqAi;

class DataAcqThread : public QThread
{
    Q_OBJECT

public:
    DataAcqThread(QObject *parent = 0);
    ~DataAcqThread();

    void startAcq();
    void stopAcq(); //note: this func call is non-blocking

    bool preparePositioner(bool isForward=true);

    //intended camera params:
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
    bool isBaselineClamp;
    Camera::TriggerMode triggerMode;
    int hstart, hend, vstart, vend; //binning params
    int angle;

    QString preAmpGain, horShiftSpeed, verShiftSpeed; //for the purpose of saving in header only

    //real params used by the camera:
    double cycleTime; //in sec

    //piezo params:
    double piezoStartPosUm, piezoStopPosUm; //NOTE: in um
    bool isBiDirectionalImaging;

    //stimulus:
    QString stimFileContent;
    bool applyStim;

    //file saving params:
    QString headerFilename, aiFilename, camFilename, sifFileBasename;
    bool isUseSpool;

    //comment:
    QString comment;

    bool isLive;
    int idxCurStack; 

signals:
    void imageDisplayReady(const QImage &image, long idx);
    void newStatusMsgReady(const QString &str);
    void newLogMsgReady(const QString &str);
    void imageDataReady(const QByteArray &data16, long idx, int imageW, int imageH);

protected:
    void run();
    void run_acq_and_save();
    void run_live();

private:
    bool saveHeader(QString filename, DaqAi* ai);
    void fireStimulus(int valve);

    QMutex mutex;
    QWaitCondition condition;
    bool restart;
    bool abort;
    volatile bool stopRequested;
};

#endif  //DATA_ACQ_THREAD_HPP
