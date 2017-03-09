//#pragma once
#include <QString>

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

class PTC10
{
public:
        PTC10::PTC10();   //конструктор-деструктор
        int GetValue(QString name, float * buffer);
        int SetValue(Qstring name, float value);

        int GetChannelsNames(QString * names[MAX_CHANNELS]);
        int ChangeChannelName(Qstring name, QString newname);

        //int GetValues(Ar1<double> *);		//array of values with indici channels_used is returned // wtf??

	int channels_used[MAX_T_USED]; //?
        int nchannels;

        int GetDeviceID(Qstring * buffer);

        int GetSpecialLog(QString name, int mode, QString time); //heavy artillery, not to be used probably //I'll think about "mode" later
        int GetLog(QString name); // returns all data from log about ch 'name'

        int SensorType(QString name, int mode); //<ch>Units
        int SetCurrent(QString name, float value, int mode);

        int DisableOutputs();        //i mean, ALL outputs
        int EnableOutputs();

        int GetTime(); //int?

        QString GetError();
        int AbortAll();         //kill.all - aborts all running macros
        int PrintOnScreen(QString value);

        int Derivative(QString name); //Instead of Power, Temperature, etc. I/O sends its derivative
        int Value(QString name); //vice versa

        int Dither(QString name); // PTC430 DC
        int NotDither(QString name);

        int Lopass(QString name, int mode);            //sets RC-filter time
        int LowLimit(QString name, float value);
        int HighLimit(QString name, float value);

        int GetAverage(QString name, int points);   //Prints Average on Screen
        int GetStdDeviation(QString name, int points);  //Prints Std Deviation on Screen
        int Stats(QString name, int numberofpoints); //Prints all statistic data on Screen
        int NoStats(QString name);

        int PIDSetRamp(QString name, float value);
        int PIDSetP(QString name, float value);
        int PIDSetI(QString name, float value);
        int PIDSetD(QString name, float value);
        int PIDMode(QString name, int mode);
        int PIDRampRate(QString name, float value); //value=0 - ASAP
        int PIDAutoTune(QString name, float lag, float step, int mode, int type);

};
 // also: <ch>Plot, <ch>cal!!, <ch>alarm !!!
