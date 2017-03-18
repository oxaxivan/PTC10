//#include "stdafx.h"
#include "PTC10.h"

PTC10::PTC10()
{
        nchannels_used = 0;
}

int PTC10::GetValue(QString name, QString * buffer)
{
        Qstring tmp = name;
        tmp.append("? \n");
        bool b;
        int nread;
	try
	{
                Write(tmp,(int)strlen(tmp));
	}
	catch(...)
	{
                buffer = 0;
		return 1;
	};
	try
	{
                nread = Read(&tmp, 256);
	}
	catch(...)
	{
                buffer = 0;
		return 2;
	}
	if(nread == 0)
		return 2;
        tmp.truncate(tmp.lastIndexOf("\n"));
        *buffer = tmp;
	return 0;
}

int PTC10::SetValue(QString name, float value)
{
        QString tmp = name;
        tmp.append(" = ");
        tmp.append(QString::number(value));
        tmp.append("\n");
	try
	{
                Write(tmp,(int)strlen(tmp));
	}
	catch(...)
	{
		return 1;
	};
	return 0;
}

int PTC10::GetDeviceID(QString * buffer)
{
	return GetValue("*IDN",buffer);
}

int PTC10::GetChannelsNames(QString * names[MAX_CHANNELS])
{
        QString b0;
        int nres = GetValue("getOutputNames",b0);
	if(nres)
		return nres;
        int i = 0, n = 0;
        while((i < MAX_CHANNELS)&&)
	{
            n = b0.indexOf("i");
            names[i] = b0.left(n);
            b0.remove(0,n);
            i++;
	}
        nchannels_used = i;
	return 0;
}
	
int ChangeChannelName(Qstring name, QString newname)
{
        Qstring tmp = name;
        tmp.append(".Name ");
        tmp.append(newname);
        tmp.append("\n")
        try
        {
                Write(tmp,(int)strlen(tmp));
        }
        catch(...)
        {
            return 1;
        }
        return 0;
}

int DisableOutputs()
{
    QString tmp = "outputEnable = off \n"
    try
    {
        Write(tmp, ...)
    }
}

int EnableOutputs()
{
    QString tmp = "outputEnable = on \n"
    try
    {
        Write(tmp, ...);
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
            Write(tmp, ...);
        }
        catch(...)
        {
            return -1;
        }
        try
        {
            t = Read(&tmp);
        }
        catch(...)
        {
            return -2;
        }
        if((tmp == 0)&&(t == 0)) return -2;
        tmp.remove("\n");
        t = tmp.toInt(&ok,10);
        if(ok == false) return -2;
        return t;
}

int GetErrors(QString[MAX_ERRORS] * list)
{
    QString t;
    int i;
    while (t != "no errors")
    {
        try
        {
            Write("geterror", ...);
        }
        catch(...)
        {
            return -1;
        }
        try
        {
            Read(&t, 256);
        }
        catch(...)
        {
            return -1;
        }
        *list[i] = t.remove("\n");
        return 0;
    }

}

int AbortAll()
{
    try
    {
        Write("kill.all",...);
    }
    catch
    {
        return 1;
    }
    return 0;
}

int PrintOnScreen(QString message)
{
    QString tmp = "print ";
    tmp.append(message);
    tmp.append("\n");
    try
    {
        Write(tmp,...);
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int Derivative(QString name)
{
    QString tmp = name;
    tmp.append(".d/dt = on \n");
    try
    {
        Write(tmp, ...)
    }
    catch
    {
        return 1;
    }
    return 0;
}

int Value(QString name)
{
    QString tmp = name;
    tmp.append(".d/dt = off \n");
    try
    {
        Write(tmp, ...)
    }
    catch
    {
        return 1;
    }
    return 0;
}

int Dither(QString name)
{
    QString tmp = name;
    tmp.append(".Dither = on \n");
    try
    {
        Write(tmp, ...)
    }
    catch
    {
        return 1;
    }
    return 0;
}

int NotDither(QString name)
{
    QString tmp = name;
    tmp.append(".Dither = off \n");
    try
    {
        Write(tmp, ...)
    }
    catch
    {
        return 1;
    }
    return 0;
}

int Lopass(QString name, char[6] mode)
{
    QString tmp;
    tmp.append(".Lopass = ");
    tmp.append(mode);
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int IncreaseLopass(QString name)
{
    QString tmp = name;
    tmp.append(".Lopass += 1 \n");
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int DecreaseLopass(QString name)
{
    QString tmp = name;
    tmp.append(".Lopass += -1 \n");
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int LowLimit(QString name, float value)
{
    QString tmp = name;
    tmp.append(".Low lmt =");
    tmp.append(floattoQstring(value));
    tmp.append("\n");
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int LowLimit(QString name, float value)
{
    QString tmp = name;
    tmp.append(".Hi lmt =");
    tmp.append(floattoQstring(value));
    tmp.append("\n");
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int GetAverage(QString name, int points)
{
    if(Stats(name) == 1) return 3;
    QString tmp = name;
    tmp.append(".Points = ");
    tmp.append(IntToQString(points));
    tmp.append("\n");
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 1;
    }
    tmp = name;
    tmp.append(".Average \n");
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 2;
    }
    return 0;
}

int GetStdDeviation(QString name, int points)
{
    if(Stats(name) == 1) return 3;
    QString tmp = name;
    tmp.append(".Points = ");
    tmp.append(IntToQString(points));
    tmp.append("\n");
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 1;
    }
    tmp = name;
    tmp.append(".SD \n");
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 2;
    }
    return 0;
}

int Stats(QString name)
{
    QString tmp = name;
    tmp.append(".Stats = on \n");
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}

int Stats(QString name)
{
    QString tmp = name;
    tmp.append(".Stats = off \n");
    try
    {
        Write(tmp, ...)
    }
    catch(...)
    {
        return 1;
    }
    return 0;
}
