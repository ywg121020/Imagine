#include "ImgApplication.h"
#include "qdebug.h"

const char *ARDUINO_PORT = "COM4";

#pragma region LIFECYCLE

ImgApplication::ImgApplication(int &argc, char **argv) : QApplication(argc, argv){
    // for now, we'll just start the arduino thread immediately
    //startArduinoThread(ARDUINO_PORT);
}

ImgApplication::~ImgApplication() {
    if (imgOne != NULL) delete(imgOne);
    if (imgTwo != NULL) delete(imgTwo);
    //killArduinoThread();
}

#pragma endregion

#pragma region UI

void ImgApplication::showUi() {
    if (imgOne != NULL) imgOne->show();
    if (imgTwo != NULL) imgTwo->show();
}

void ImgApplication::initUI(QString rig, Camera *cam1, Positioner *pos, Laser *laser, Camera *cam2) {
    // init first window
    Imagine *w1p = new Imagine(rig, cam1, pos, laser);
    imgOne = w1p;

    // init second window if needed.  The first window is "master", meaning that it controls the positioner.
    Imagine *w2p = nullptr;
    if (cam2 != NULL) {
        w2p = new Imagine(rig, cam2, pos, laser, w1p);
        imgTwo = w2p;
        imgOne->setSlaveWindow(w2p);
        imgTwo->setSlaveWindow(NULL);
    }
    else
        imgOne->setSlaveWindow(NULL);

    if (rig == "ocpi-2") {
        if (imgOne)
            if (cam1->getCameraID() == 1)
                imgOne->setWindowTitle("Imagine (1)");
            else
                imgOne->setWindowTitle("Imagine (2)");
        if (imgTwo)
            if (cam2->getCameraID() == 1)
                imgTwo->setWindowTitle("Imagine (1)");
            else
                imgTwo->setWindowTitle("Imagine (2)");
    } // else defaut name "Imagine"
    qDebug() << QString("finished initializing UI instance(s)");
}

#pragma endregion

#pragma region Arduino

void ImgApplication::startArduinoThread(const char *port) {
    // make sure we don't have a thread around already
    if (ardThread != NULL) killArduinoThread();
    // the new thread object
    ardThread = new ArduinoThread(port);
    // make the thread clean itself up when it's finished
    connect(ardThread, &ArduinoThread::finished,
        ardThread, &ArduinoThread::deleteLater);
    // make sure we clean up our ref to thread when it goes
    connect(ardThread, &ArduinoThread::finished,
        this, &ImgApplication::ardThreadFinished);
    // we'll use the application object to pass messages to the arduino thread
    connect(this, &ImgApplication::msgForArduino,
        ardThread, &ArduinoThread::sendMessage);
    // and for now we'll handle incoming messages from the arduino at the app level
    connect(ardThread, &ArduinoThread::incomingArduino,
        this, &ImgApplication::incomingArduinoMessage);
    // start the thread running with normal priority
    ardThread->start();
}

void ImgApplication::killArduinoThread() {
    // stop the thread. Thread's finish signal will call our cleanup method.
    if (ardThread == NULL) return;
    ardThread->requestInterruption();
    ardThread->wait(); // this won't lock... right?
}

void ImgApplication::ardThreadFinished() {
    // just make sure we don't have a bum pointer
    ardThread = nullptr;
}

void ImgApplication::sendToArduino(const char *message, int length) {
    if (ardThread == NULL) return;
    // just emit the message... being lazy about letting qt do thread safety for us
    emit msgForArduino(message, length);
}

void ImgApplication::sendToArduino(QString str) {
    sendToArduino(str.toLatin1().data(), str.length());
}

void ImgApplication::incomingArduinoMessage(QString message) {
    // for now, just log it...
    qDebug() << "Arduino says:\n" << message;
}

#pragma endregion