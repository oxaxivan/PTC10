//#pragma once
#include <QString>

//#define MAX_CHANNELS	30
#define MAX_T_USED		8
#define MAX_ERRORS    20

#define LOPASS_OFF "Off  \n"
#define LOPASS_1s "1 s  \n"
#define LOPASS_3s "3 s  \n"
#define LOPASS_10s "10 s \n"
#define LOPASS_30s "30 s \n"
#define LOPASS_100s "100 s\n"
#define LOPASS_300s "300 s\n"

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
        PTC10::PTC10(QString BaudRate, QString PortName);   //конструктор-деструктор
        int GetValue(QString name, QString * buffer);
        int SetValue(Qstring name, float value);

        int GetChannelsNames(QString * names[MAX_CHANNELS]);
        int ChangeChannelName(Qstring name, QString newname);

        //int GetValues(Ar1<double> *);		//array of values with indici channels_used is returned // wtf??

        int nchannels_used;

        int GetDeviceID(Qstring * buffer);

        int GetSpecialLog(QString name, int mode, QString time); //heavy artillery, not to be used probably //I'll think about "mode" later
        int GetLog(QString name); // returns all data from log about ch 'name'

        int SensorType(QString name, int mode); //<ch>Units
        int SetCurrent(QString name, float value, int mode);

        int DisableOutputs();        //i mean, ALL outputs
        int EnableOutputs();

        int GetTime(); //int?

        int GetErrors(QString[MAX_ERRORS] * list);
        int AbortAll();         //kill.all - aborts all running macros
        int PrintOnScreen(QString message);

        int Derivative(QString name); //Instead of Power, Temperature, etc. I/O sends its derivative
        int Value(QString name); //vice versa

        int Dither(QString name); // PTC430 DC
        int NotDither(QString name);

        int Lopass(QString name, char[6] mode);            //sets RC-filter time for inputs
        int IncreaseLopass(QString name);
        int DecreaseLopass(QString name);

        int LowLimit(QString name, float value);
        int HighLimit(QString name, float value);

        int GetAverage(QString name, int points);   //Prints Average on Screen??
        int GetStdDeviation(QString name, int points);  //Prints Std Deviation on Screen??
        int Stats(QString name); //Starts Gathering Stats
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
