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
        PTC10(int BaudRate, QString PortName);   //конструктор. вызывается сразу
        int GetValue(const QString &name, char * buffer);  //не нужна
        int SetValue(const Qstring &name, float value);  //задает мощность (или еще что-то). в 1 очередь

        int GetChannelValue(const QString & name);  //спрашивает температуру (или еще что-то). в 1 очередь

        int GetChannelsNames(QString names[MAX_CHANNELS]);  //записывает имена каналов в names
        int ChangeChannelName(const QString & name, const QString & newname);  //не нужна

        int GetDeviceID(Qstring & buffer);  //не нужна

        //int GetSpecialLog(QString name, int mode, QString time); //heavy artillery, not to be used probably //I'll think about "mode" later
        //int GetLog(QString name); // returns all data from log about ch 'name'

        //int SensorType(const QString & name, int mode); //<ch>Units
        //int SetCurrent(const QString & name, float value, int mode);

        int DisableOutputs();        //i mean, ALL outputs //пока не нужна, но иметь в виду
        int EnableOutputs();    //пока не нужна

        int GetTime();  //пока не нужна

        int HardReset();    //пока не нужна
        int GetErrors(QString * list);  //пригодится при отладке
        int AbortAll();         //kill.all - aborts all running macros //пока не нужна
        int PrintOnScreen(const QString & message); //не нужна

        int Derivative(const QString & name); //Instead of Power, Temperature, etc. I/O sends its derivative //в 2 очередь
        int Value(const QString & name); //vice versa  //в 2 очередь

        int Dither(const QString & name); // PTC430 DC  //хз
        int NotDither(const QString & name);    //хз

        int Lopass(const QString & name, QString mode);            //sets RC-filter time for inputs //не нужна
        int IncreaseLopass(const QString & name);   //не нужна
        int DecreaseLopass(const QString & name);   //не нужна

        int LowLimit(const QString & name, float value);    //скорее всего, не нужна
        int HighLimit(const QString & name, float value);   //скорее всего, не нужна

        int GetAverage(const QString & name, int points);   //Prints Average /on Screen??
        int GetStdDeviation(const QString & name, int points);  //Prints Std Deviation /on Screen??
        int Stats(const QString & name); //Starts Gathering Stats
        int NoStats(const QString & name);

        int PIDSetRamp(const QString & name, float value);  //весь PID - в 2 очередь
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
