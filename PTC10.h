#pragma once
#include "tty.h"
#include "templfn.h"

#define MAX_CHANNELS	30
#define MAX_T_USED		8

#define LOPASS_OFF 0
#define LOPASS_1s 1
#define LOPASS_3s 2
#define LOPASS_10s 3
#define LOPASS_30s 4
#define LOPASS_100s 5
#define LOPASS_300s 6

#define SENSOR_CURR_1m 1
#define SENSOR_CURR_100u 2
#define SENSOR_CURR_10u 3
#define SENSOR_CURR_AUTO 4

#define PID_MODE_OFF 0
#define PID_MODE_ON 1
#define PID_MODE_FOLLOW 2 //wtf is this?

#define PID_AUTO_MODE_OFF 0
#define PID_AUTO_MODE_AUTO 1
#define PID_AUTO_MODE_STEP 2
#define PID_AUTO_MODE_RELAY 3

#define PID_TYPE_CONS 1
#define PID_TYPE_MOD 2
#define PID_TYPE_AGGR 3
#define PID_TYPE_AUTO 4

class PTC10 : public TTY
{
public:
	PTC10::PTC10();
	int GetValue(const char * name, char * buffer);
	int SetValue(const char * name, char * value);

	int GetChannels(char* names[MAX_CHANNELS], int *nchannels);
        int ChangeName(const char * name, const char * newname);

        int GetValues(Ar1<double> *);		//array of values with indici channels_used is returned / wtf??

	int channels_used[MAX_T_USED]; //?
	int nchannels, nchannels_used;
        //wtf nchannel_used?

	int GetDeviceID(char * buffer);

        int GetSpecialLog(const char * name, const char * mode, const char * time); //heavy artillery, not to be used probably
        int GetLog(const char * name); // returns all data from log about ch 'name'

        int SensorType(const char * name, int mode);
        int SetCurrent(const char * name, float value, int mode);

        int DisableOutputs();        //i mean, ALL outputs
        int EnableOutputs();

        int GetTime(); //int?

        void GetError(char * buffer);
        int AbortAll();         //kill.all - aborts all running macros
        int Print(char * value);

        int GetDerivative(const char * name);

        int Dither(const char * name); // PTC430 DC
        int NotDither(const char * name);

        int Lopass(const char * name, int mode);            //sets RC-filter time
        int LowLimit(const char * name, float value);
        int HighLimit(const char * name, float value);

        int Average(const char * name, int points);
        int StdDeviation(const char * name, int points);
        int Stats(const char * name); //+int points
        int NoStats(const char * name);

        int PIDSetRamp(const char * name, float value);
        int PIDSetP(const char * name, float value);
        int PIDSetI(const char * name, float value);
        int PIDSetD(const char * name, float value);
        int PIDMode(const char * name, int mode);
        int PIDRampRate(const char * name, float value); //value=0 - ASAP
        int PIDAutoTune(const char * name, float lag, float step, int mode, int type);

};
 // also: <ch>Plot, <ch>Units, <ch>cal!!, <ch>alarm !!!
