/*-------------------------------------------------------------------------
** Copyright (C) 2017-2022 Dae Woo Kim
**    All rights reserved.
** Author: Dae Woo Kim
** License: This file may be used under the terms of the GNU General Public
**    License version 2.0 as published by the Free Software Foundation
**    and appearing at http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
**
**    If this license is not appropriate for your use, please
**    contact kdw503@wustl.edu for other license(s).
**
** This file is provided WITHOUT ANY WARRANTY; without even the implied
** warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**-------------------------------------------------------------------------*/

#ifndef SCRIPT_H
#define SCRIPT_H

#include <QtWidgets/QApplication>
#include <QTextStream>
#include <QFile>
#include <QScriptEngine>
#include <QScriptProgram>
#include <QScriptable>
#include <QThread>
#include <string>

class ImagineScript : public QObject
{
    Q_OBJECT
private:
    QString rig;
    QScriptEngine *se;
    bool print(const QString &str);
    bool validityCheck(const QString &file1, const QString &file2);
    bool record(const QString &file1, const QString &file2, long int timeout);
    bool loadConfig(const QString &file1, const QString &file2);
    bool loadWaveform(const QString &file);
    bool sleep(long int time);
    bool setFilename(const QString &filename);

public:
    static void *instance;
    QString errorMsg = "";
    QScriptProgram *scriptProgram = NULL;
    bool shouldWait = false;    // wait until Imagine function is done
    bool retVal;                // return value of Imagine function

    ImagineScript(QString rig);
    ~ImagineScript();
    QString getErrorMsg(void) {
        return errorMsg;
    };

    void loadImagineScript(QString code);
    bool scriptProgramEvaluate();

    // print("Hello world!")
    static QScriptValue printWrapper(QScriptContext *context, QScriptEngine *se);
    // validityCheck("OCPI_cfg1.txt", "OCPI_cfg2.txt");
    static QScriptValue validityCheckWrapper(QScriptContext *context, QScriptEngine *se);
    // record("OCPI_cfg1.txt", "OCPI_cfg2.txt", timeout);
    static QScriptValue recordWrapper(QScriptContext *context, QScriptEngine *se);
    // loadConfig("OCPI_cfg1.txt", "OCPI_cfg2.txt");
    static QScriptValue loadConfigWrapper(QScriptContext *context, QScriptEngine *se);
    // loadWaveform("OCPI_waveform.json");
    static QScriptValue loadWaveformWrapper(QScriptContext *context, QScriptEngine *se);
    // sleep(1000); // sleep 1000msec
    static QScriptValue sleepWrapper(QScriptContext *context, QScriptEngine *se);
    // setOutputFilename("t1.imagine");
    static QScriptValue setFilenameWrapper(QScriptContext *context, QScriptEngine *se);

signals:
    void newMsgReady(const QString &str);
    void requestValidityCheck(const QString &file1, const QString &file2);
    void requestRecord();
    void requestLoadConfig(const QString &file1, const QString &file2);
    void requestLoadWaveform(const QString &file);
    void requestSetFilename(const QString &file);
};

#endif // SCRIPT_H

