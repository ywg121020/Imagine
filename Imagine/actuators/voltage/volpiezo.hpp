#ifndef VOLPIEZO_HPP
#define VOLPIEZO_HPP

#include "positioner.hpp"

#include "ni_daq_g.hpp"

class VolPiezo : public Positioner {

public:
    VolPiezo(QString aistring, QString aostring){
        ainame = aistring;
        aoname = aostring;
        aoOnce = nullptr;
        ao = nullptr;
        posType = VolPiezoPositioner;
    }
    ~VolPiezo(){ delete aoOnce; delete ao; }

	//TODO: make these configurable elsewhere.  may be better to subclass VolPiezo for individual positioner models
    double minPos(){ return 0; }
    double maxPos(){ return 800; }
	double maxSpeed(){ return 2000; } //microns per second
    bool curPos(double* pos);
    bool moveTo(double to);

    bool addMovement(double from, double to, double duration, int trigger);

    bool prepareCmd();
    bool runCmd();
    bool waitCmd();
    bool abortCmd();

private:
    QString ainame;
    QString aoname;
    NiDaqAoWriteOne * aoOnce;
    NiDaqAo* ao;

    double zpos2voltage(double um);
    void cleanup();
};//class, VolPiezo

#endif