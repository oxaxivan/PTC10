//#pragma once
#include <QString>
#include <QtSerialPort/QSerialPort>
#include <QTime>

//#define MAX_CHANNELS	30
#define MAX_T_USED		8
#define MAX_ERRORS    20
#define MAX_SYMBOLS 256
#define MAX_CHANNELS 30

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
        int nchannels_used;
        PTC10(int BaudRate, QString PortName);   //конструктор-деструктор
        int GetValue(const QString &name, char * buffer);
        int SetValue(const Qstring &name, float value);

        int GetChannelsNames(QString names[MAX_CHANNELS]);
        int ChangeChannelName(const QString & name, const QString & newname);

        int GetDeviceID(Qstring & buffer);

        //int GetSpecialLog(QString name, int mode, QString time); //heavy artillery, not to be used probably //I'll think about "mode" later
        //int GetLog(QString name); // returns all data from log about ch 'name'

        //int SensorType(const QString & name, int mode); //<ch>Units
        //int SetCurrent(const QString & name, float value, int mode);

        int DisableOutputs();        //i mean, ALL outputs
        int EnableOutputs();

        int GetTime();

        int HardReset();
        int GetErrors(QString * list); //Achtung
        int AbortAll();         //kill.all - aborts all running macros
        int PrintOnScreen(const QString & message);

        int Derivative(const QString & name); //Instead of Power, Temperature, etc. I/O sends its derivative
        int Value(const QString & name); //vice versa

        int Dither(const QString & name); // PTC430 DC
        int NotDither(const QString & name);

        int Lopass(const QString & name, QString mode);            //sets RC-filter time for inputs
        int IncreaseLopass(const QString & name);
        int DecreaseLopass(const QString & name);

        int LowLimit(const QString & name, float value);
        int HighLimit(const QString & name, float value);

        int GetAverage(const QString & name, int points);   //Prints Average /on Screen??
        int GetStdDeviation(const QString & name, int points);  //Prints Std Deviation /on Screen??
        int Stats(const QString & name); //Starts Gathering Stats
        int NoStats(const QString & name);

        int PIDSetRamp(const QString & name, float value);
        int PIDSetP(const QString & name, float value);
        int PIDSetI(const QString & name, float value);
        int PIDSetD(const QString & name, float value);
        int PIDMode(const QString & name, int mode);
        int PIDInput(const QString & output_name, const QString & input_name);
        int PIDRampRate(const QString & name, float value); //value=0 - ASAP
        //int PIDAutoTune(QString name, float lag, float step, int mode, int type);

        private slots:
        void getResponse(const QString &s);

        signals:
        void response(const QString &s) const;

};
 // also: <ch>Plot, <ch>cal!!, <ch>alarm !!!
