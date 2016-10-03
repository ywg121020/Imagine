/*-------------------------------------------------------------------------
** Copyright (C) 2005-2008 Timothy E. Holy and Zhongsheng Guo
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

#ifndef NI_DAQ_G_HPP
#define NI_DAQ_G_HPP

#include <assert.h>
#include <math.h>

#include <string>
#include <vector>
#include <iostream>

using std::string;
using std::vector;
using std::cout;
using std::endl;

#include "NIDAQmx.h"

#include "misc.hpp"
#include "daq.hpp"


class NiDaq : public virtual Daq {
protected:
   TaskHandle  taskHandle;
   int errorCode;  
public:
   //ctor: create task
   NiDaq(const vector<int> & chs): Daq(chs){
      taskHandle=0;
      errorCode=DAQmxCreateTask("",&taskHandle);
      if(isError()){
         throw EInitDevice("exception when call DAQmxCreateTask()");
      }
   }//ctor,

   //dtor: clear task (i.e. release driver-allocated resources) 
   virtual ~NiDaq(){
      DAQmxClearTask(taskHandle);
   }//dtor,


   //start task
   bool start(){
      errorCode=DAQmxStartTask(taskHandle);
      return !isError();
   }//start(),

   //stop task (i.e. abort if running & reset the task)
   bool stop(){
      errorCode=DAQmxStopTask(taskHandle);
      return !isError();
   }//stop(),

   //check if an error code means error. This func wraps the macro.
   //note: an error code may mean success/warning/error, like compiler's result.
   //return true if errorCode means error,
   //return false if errorCode means warning or success.
   bool isError(){
      return DAQmxFailed(errorCode); 
   }//isError(),

   //similar to isError(). This func is a wrapper to a macro.
   //return true if errorCode means success, 
   //return false if it means warning or error.
   bool isSuc(){
      return DAQmxSuccess==errorCode;
   }//isSuc(),

   //get error msg
   string getErrorMsg(){
      static char errBuf[2048*2]={'\0'};
      string errorMsg;

      if(isSuc()){
         errorMsg="no error/warning";
      }//if, no error & no warning
      else{
         DAQmxGetExtendedErrorInfo(errBuf, sizeof(errBuf));
         if(isError()) errorMsg="Error msg:\n";
         else errorMsg="Warning msg:\n";
         errorMsg+=errBuf;
      }//else, there is error or warning

      return errorMsg;
   }//getErrorMsg(),
};//class, NiDaq

//class: NI DAQ Analog Output
//note: this class manages its output buffer itself.
class NiDaqAo: public NiDaq, public DaqAo {
   sample_t *    dataU16; 

public:
   //create ao channel and add the channel to task
   NiDaqAo(QString devstring, const vector<int> & chs): Daq(chs), NiDaq(chs), DaqAo(chs){
      dataU16=0;
      string dev = devstring.toStdString();
      cout << "About to initialize AO device " << dev << endl;
      //string dev="Dev1/ao";
      string chanList=dev+toString(channels[0]);
      for(unsigned int i=1; i<channels.size(); ++i){
         chanList+=", "+dev+toString(channels[i]);
      }

      errorCode=DAQmxCreateAOVoltageChan(taskHandle,
         chanList.c_str(), //channels to acquire
         "",  //nameToAssignToChannel
         -10.0,   //min value to output
         10.0,//max value to output
         DAQmx_Val_Volts,  //unit
         NULL              //customScaleName
         );
      if(isError()){
         throw EInitDevice("exception when call DAQmxCreateAOVoltageChan()");
      }

      //get the raw sample size, min/max digital values:
      float64 tt;
      errorCode=DAQmxGetAOResolution(taskHandle,(dev+"0").c_str(), &tt);
      if(isError()){
         throw EInitDevice("exception when call DAQmxGetAOResolution()");
      }
      sampleSize=(uInt32)tt;
      maxDigitalValue=pow(2.0,int(sampleSize)-1)-1;
      minDigitalValue=-pow(2.0,int(sampleSize)-1);
      maxPhyValue=10;
      minPhyValue=-10;

   }//ctor,

   //release mem
   virtual ~NiDaqAo(){
      if(dataU16) delete[] dataU16;
   }//dtor,

   //set rate and duration, and allocate output buffer
   //return true if success or warning;
   //return false if error
   //note: you can check for error by checking return value OR call .isError() 
   bool cfgTiming(int scanRate, int nScans){ //nScans: #scans to acquire
      this->scanRate=scanRate;
      this->nScans=nScans;

      errorCode=DAQmxCfgSampClkTiming(taskHandle,"",
         scanRate,
         DAQmx_Val_Rising,
         DAQmx_Val_FiniteSamps,
         nScans
         );

      //allocate output buffer
      if(dataU16)delete[] dataU16;
      dataU16=0;
      dataU16=new uInt16[nScans*channels.size()];
      if(!dataU16){
         throw ENoEnoughMem();
      }

      return !isError();
   }//cfgTiming(),

   //get output buffer address
   //NOTE: data is grouped by channel, i.e. all samples for a channel are close to each other
   uInt16 * getOutputBuf(){
      return dataU16;
   }//getOutputBuf()

   //write waveform to driver's buffer
   bool updateOutputBuf(){
      cout<<"in  updateOutputBuf()"<<endl;
      int32 nScanWritten;
      errorCode=DAQmxWriteBinaryU16(taskHandle,
         nScans, 
         false,  //don't auto start
         DAQmx_Val_WaitInfinitely,
         DAQmx_Val_GroupByChannel, //data layout, all samples for a channel then for another channel
         dataU16,
         &nScanWritten,
         NULL //reserved
         );
      
      cout<<"out updateOutputBuf()"<<endl;
      return !isError();
   }//updateOutputBuf(),

   //wait task until done
   bool wait(double timeToWait){
      errorCode=DAQmxWaitUntilTaskDone(taskHandle, timeToWait);
      return !isError();
   }//wait(),

   //query if task is done
   bool isDone(){
      bool32 result;
      errorCode=DAQmxIsTaskDone(taskHandle, &result);

      return result!=0;
   }//isDone(),

};//class, NiDaqAo


//unlike AO, user need supply read buffer for AI
class NiDaqAi: public NiDaq, public DaqAi {
   //uInt16 *    dataU16; //it is better that user supplies the read buf
public:
   //create AI channels and add the channels to the task
   NiDaqAi(QString devstring,const vector<int> & chs): Daq(chs), NiDaq(chs), DaqAi(chs){
      //todo: next line is unnecessary?
      //DAQmxErrChk(DAQmxCfgInputBuffer(taskHandle, buf_size) ); //jason: this change DEFAULT(?) input buffer size
      string dev = devstring.toStdString();
      cout << "About to initialize AI device " << dev << endl;
      //string dev="Dev1/ai";
      string chanList=dev+toString(channels[0]);
      for(unsigned int i=1; i<channels.size(); ++i){
         chanList+=", "+dev+toString(channels[i]);
      }

      errorCode=DAQmxCreateAIVoltageChan(taskHandle,
         chanList.c_str(),  //channels to acquire
         "",          //nameToAssignToChannel
         DAQmx_Val_RSE , //terminalConfig. todo: should be DAQmx_Val_NRSE? 
         -10.0,   //input range: minVal 
         10.0,    //input range: maxVal
         DAQmx_Val_Volts, //unit
         NULL     //reserved
         );  
      if(isError()){
         throw EInitDevice("exception when call DAQmxCreateAIVoltageChan()");
      }

      //get the raw sample size:
      errorCode=DAQmxGetAIRawSampSize(taskHandle, (dev+"0").c_str(), (uInt32*)&sampleSize);
      if(isError()){
         throw EInitDevice("exception when call DAQmxGetAIRawSampSize()");
      }
	  //TODO: above func set sampleSize to 16, which is incorrect. 
	  //      should be 12. a Temp fix: (or not?)
	  //sampleSize=12;

	  maxDigitalValue = pow(2.0, int(sampleSize) - 1) - 1;
	  minDigitalValue = -pow(2.0, int(sampleSize) - 1);

      maxPhyValue=10;
      minPhyValue=-10;
   }//ctor, NiDaqAi

   //dtor:
   virtual ~NiDaqAi(){
      
   }//dtor, NiDaqAi

   //set scan rate and driver's input buffer size in scans
   bool cfgTiming(int scanRate, int bufSize){
      this->scanRate=scanRate;

      errorCode=DAQmxCfgSampClkTiming(taskHandle,"",
         scanRate,
         DAQmx_Val_Rising,
         DAQmx_Val_ContSamps,
         bufSize //the unit is scan
         ); 

      return !isError();
   }//cfgTiming(),

   //read input from driver
   bool read(int nScans, uInt16 * buf){
      int32 nScansReaded;

      errorCode=DAQmxReadBinaryU16(taskHandle,
         nScans,  //numSampsPerChan
         DAQmx_Val_WaitInfinitely,    //timeToWait
         DAQmx_Val_GroupByScanNumber, //data layout, here save all data in scan by scan
         buf,
         nScans*channels.size(), // #samples to read
         &nScansReaded,
         NULL  //reserved
         );

#if defined(DEBUG_AI)
      if(isError()){
         getErrorMsg();
      }
#endif

      assert(nScans==nScansReaded || isError());

      return !isError();
   }//readInput(),

};//class, NiDaqAi

class NiDaqDo: public NiDaq, public DaqDo {
public:
   //create DO channel and start the task
   NiDaqDo(QString devstring): Daq(vector<int>()), NiDaq(vector<int>()){
      string dev = devstring.toStdString();
      cout << "About to initialize DO device with " << dev << endl;
      errorCode=DAQmxCreateDOChan(taskHandle,dev.c_str(),"",DAQmx_Val_ChanForAllLines);
      if(isError()){
         throw EInitDevice("exception when call DAQmxCreateDOChan()");
      }

      if(!start()){
         throw EInitDevice(string("exception when call DAQmxCreateDOChan():\n")+getErrorMsg());
      }
   }//ctor, NiDaqDo()

   //stop the task
   ~NiDaqDo(){
      stop();
   }//dtor, ~NiDaqDo()


   //output data to hardware
   bool write(){
      errorCode=DAQmxWriteDigitalLines(taskHandle,
         1, //numSampsPerChan
         1, //autoStart
         10.0, //timeout: 10s
         DAQmx_Val_GroupByChannel,//
         data,
         NULL, //sampsPerChanWritten
         NULL);//reserved

      return !isError();
   }//write(),

   bool cfgTiming(int, int){ return true;} //do noting for Dig-out

};//class, NiDaqDo

//read one sample
class NiDaqAiReadOne :public DaqAiReadOne {
   NiDaqAi * ai;
public:
   NiDaqAiReadOne(QString ainame, int channel):DaqAiReadOne(channel){
      vector<int> chs;
      chs.push_back(channel);
      ai=new NiDaqAi(ainame, chs);

      if(!ai->cfgTiming(10000, 10000)){
         throw Daq::EInitDevice("exception when cfgTiming()");
      }
   }//ctor,

   ~NiDaqAiReadOne(){
      delete ai;
   }//dtor,

   //return true if success
   bool readOne(int & reading){
      int16 tReading;
      if(!ai->start()) return false;
      if(!ai->read(1, (uInt16*)&tReading)) return false;
      if(!ai->stop()) return false;
      reading=tReading;

      return true;
   }//readOne()

   double toPhyUnit(double digValue){
      return ai->toPhyUnit(digValue);
   }
};//class, NiDaqAiReadOne

//write one sample
class NiDaqAoWriteOne : public DaqAoWriteOne{
   NiDaqAo* ao;
public:
   NiDaqAoWriteOne(QString aoname,int channel): DaqAoWriteOne(channel){
      vector<int> channels;
      channels.push_back(channel);
      ao=new NiDaqAo(aoname, channels);

      if(!ao->cfgTiming(10000, 2)){ //for buffered writing, 2 samples at least. //SEE: DAQmxWriteBinaryU16()'s online help.
         throw Daq::EInitDevice("exception when cfgTiming()");
      }
   }//ctor,

   ~NiDaqAoWriteOne(){
      delete ao;
   }

   //return true if success
   bool writeOne(int valueToOutput){
      cout<<"in  writeOne()"<<endl;
      uInt16* buf=ao->getOutputBuf();
      buf[0]=valueToOutput;
      buf[1]=buf[0];
      if(!ao->updateOutputBuf()) {
         cout<<ao->getErrorMsg()<<endl;
         return false;
      }
      if(!ao->start()) return false;
      if(!ao->wait(-1)) return false; //wait forever
      if(!ao->stop()) return false;

      cout<<"out writeOne()"<<endl;
      return true;
   }//writeOne(),

   int toDigUnit(double phyValue){
      return ao->toDigUnit(phyValue);
   }//toDigUnit(),
};//class, NiDaqAoWriteOne 

#endif //NI_DAQ_G_HPP