//#include "stdafx.h"
#include "PTC10.h"

PTC10::PTC10(int BaudRate, QString PortName)
{
        nchannels_used = 0;
        serial = new QSerialPort();
        serial->setParity(QSerialPort::NoParity);
        serial->setStopBits(QSerialPort::OneStop);
        serial->setDataBits(QSerialPort::Data8);
        serial->setPortName(PortName);
        serial->setFlowControl(QSerialPort::HardwareControl);

        switch(BaudRate){
        case 600:
            serial->setBaudRate(600);
            break;
        case 1200:
            serial->setBaudRate(1200);
            break;
        case 2400:
            serial->setBaudRate(2400);
            break;
        case 4800:
            serial->setBaudRate(4800);
            break;
        case 9600:
            serial->setBaudRate(9600);
            break;
        case 19200:
            serial->setBaudRate(19200);
            break;
        case 38400:
            serial->setBaudRate(38400);
            break;
        case 57600:
            serial->setBaudRate(57600);
            break;
        case 115200:
            serial->setBaudRate(115200);
            break;
        default:
            emit response("Wrong Baudrate");
        }

        connect(&serial, SIGNAL(response(QString)),this, SLOT(get_response(QString)));
        if (!serial->open(QIODevice::ReadWrite))
            return 1;
}

int PTC10::GetValue(const QString &name, char * buffer)
{
        Qstring tmp = name;
        tmp.append("? \n");
        //int nread;
	try
	{
                serial->write(tmp.toLocal8Bit());
	}
	catch(...)
	{
                buffer = 0;
		return 1;
	};
	try
	{
            serial->waitForReadyRead(30)
            *buffer = serial->readAll();
	}
	catch(...)
	{
                *buffer = 0;
		return 2;
	}
        /*if(nread == 0)
                return 2;*/
        tmp.truncate(tmp.lastIndexOf("\n"));
        *buffer = tmp;
	return 0;
}

int PTC10::SetValue(const QString & name, float value)
{
        QString tmp = name;
        tmp.append(".value = ");
        tmp.append(QString::number(value));
        tmp.append("\n");
	try
	{
                serial->write(tmp.toLocal8Bit());
	}
	catch(...)
	{
		return 1;
	};
	return 0;
}

int GetChannelValue(const QString & name)
{
    QString tmp = name;
    tmp.append(".value?");
    char value[10];  //= "0", "0", "0", "0", "0", "0", "0", "0", "0", "0"
    if(GetValue(tmp,value))
        return 1;
    return atof(value);
}

int PTC10::GetDeviceID(QString & buffer)
{
    char t[MAX_SYMBOLS];
    int j = 0;
    return GetValue("*IDN",t);
    while(t[j]!="\n")
    {
        buffer.append(t[j]);
        j++;
    }
}

int PTC10::GetChannelsNames(QString names[MAX_CHANNELS])
{
        char [MAX_SYMBOLS] b0;
        int nres = GetValue("getOutputNames",b0);
	if(nres)
		return nres;
        int i = 0, j=0;
        while(b0[j]!="\n")
	{
            while(b0[j]!=",")
                names[i].append(b0[j]);
            i++;
            j++;
	}
        nchannels_used = i;
	return 0;
}
	
int ChangeChannelName(const QString & name, const QString & newname)
{
        Qstring tmp = name;
        tmp.append(".Name ");
        tmp.append(newname);
        tmp.append("\n");
        try
        {
                serial->write(tmp.toLocal8Bit());
        }
        catch(...)
        {
            return 1;
        }
        return 0;
}

int DisableOutputs()
{
    QString tmp = "outputEnable = off \n";
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int EnableOutputs()
{
    QString tmp = "outputEnable = on \n";
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int GetTime()
{
        QString tmp = "Systemtime.ms?\n";
        int t;
        bool ok;
        try
        {
            serial->write(tmp.toLocal8Bit());
        }
        catch(...)
        {
            return -1;
        }
        try
        {
            serial->waitForReadyRead(30);
            QByteArray responseData = serial->readAll();
            tmp = QString(responseData);
        }
        catch(...)
        {
            return -2;
        }
        if(tmp == 0) return -2;
        tmp.remove("\n");
        t = tmp.toInt(&ok,10);
        if(ok == false) return -2;
        return t;
}

int GetErrors(QString * list)
{
    QString t;
    QString tt = "geterror";
    int i;
    while (t != "no errors")
    {
        try
        {
            serial->write(tt.toLocal8Bit());
        }
        catch(...)
        {
            return -1;
        }
        try
        {
            serial->waitForReadyRead(30);
            QByteArray responseData = serial->readAll();
            t = QString(responseData);
        }
        catch(...)
        {
            return -1;
        }
        *(list+i) = t.remove("\n");
    }
    return 0;
}

int AbortAll()
{
    QString tmp = "kill.all";
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int HardReset()
{
    QString tmp = "*RST\n";
    try
    {
        serial->write(tmp.toLocal8Bit)
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int PrintOnScreen(const QString & message)
{
    QString tmp = "print ";
    tmp.append(message);
    tmp.append("\n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int Derivative(const QString & name)
{
    QString tmp = name;
    tmp.append(".d/dt = on \n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int Value(const QString & name)
{
    QString tmp = name;
    tmp.append(".d/dt = off \n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int Dither(const QString & name)
{
    QString tmp = name;
    tmp.append(".Dither = on \n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int NotDither(const QString & name)
{
    QString tmp = name;
    tmp.append(".Dither = off \n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int Lopass(const QString & name, QString mode)
{
    QString tmp = name;
    tmp.append(".Lopass = ");
    tmp.append(mode);
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int IncreaseLopass(const QString & name)
{
    QString tmp = name;
    tmp.append(".Lopass += 1 \n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int DecreaseLopass(const QString & name)
{
    QString tmp = name;
    tmp.append(".Lopass += -1 \n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int LowLimit(const QString & name, float value)
{
    QString tmp = name;
    tmp.append(".Low lmt =");
    tmp.append(Qstring::number(value));
    tmp.append("\n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int HighLimit(const QString & name, float value)
{
    QString tmp = name;
    tmp.append(".Hi lmt =");
    tmp.append(QString::number(value));
    tmp.append("\n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int GetAverage(const QString & name, int points)
{
    if(Stats(name) == 1) return 3;
    QString tmp = name;
    tmp.append(".Points = ");
    tmp.append(QString::number(points));
    tmp.append("\n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    tmp = name;
    tmp.append(".Average \n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 2;
    }
    return 0;
}

int GetStdDeviation(const QString & name, int points)
{
    if(Stats(name) == 1) return 3;
    QString tmp = name;
    tmp.append(".Points = ");
    tmp.append(QString::number(points));
    tmp.append("\n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    tmp = name;
    tmp.append(".SD \n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 2;
    }
    return 0;
}

int Stats(const QString & name)
{
    QString tmp = name;
    tmp.append(".Stats = on \n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int NoStats(const QString & name)
{
    QString tmp = name;
    tmp.append(".Stats = off \n");
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int PIDSetP(const QString & name, float value)
{
    QString tmp = name;
    tmp.append("PID.P = ");
    tmp.append(QString::number(value));
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int PIDSetI(const QString & name, float value)
{
    QString tmp = name;
    tmp.append("PID.I = ");
    tmp.append(QString::number(value));
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int PIDSetD(const QString & name, float value)
{
    QString tmp = name;
    tmp.append("PID.D = ");
    tmp.append(QString::number(value));
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int PIDMode(const QString & name, int mode)
{
    QString tmp = name;
    tmp.append(".PID.Mode = ");
    switch (mode) {
    case PID_MODE_ON:
        tmp.append("On");
        break;
    case PID_MODE_OFF:
        tmp.append("Off");
        break;
    case PID_MODE_FOLLOW:
        tmp.append("Follow");
        break;
    default:
        return 2;
        break;
    }
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;

}

int PIDInput(const QString & output_name, const QString & input_name)
{
    QString tmp = output_name;
    tmp.append(".Pid.Input =");
    tmp.append(input_name);
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int PIDSetRamp(const QString & name, float value)
{
    QString tmp = name;
    tmp.append(".PID.PampT = ");
    tmp.append(QString::number(value));
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int PIDSetRamp(const QString & name, float value)
{
    QString tmp = name;
    tmp.append(".PID.Pamp = ");
    tmp.append(QString::number(value));
    try
    {
        serial->write(tmp.toLocal8Bit());
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

void MainWindow::getResponse(const QString &s)
{
    ui->lineEditResponse->setText(s);
}
