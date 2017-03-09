//#include "stdafx.h"
#include "PTC10.h"

PTC10::PTC10()
{
        //TTY::TTY();
	nchannels_used = 4;
	nchannels = MAX_CHANNELS;
	for(int q = 0; q < MAX_T_USED; q++)
		channels_used[q] = q;
}

int PTC10::GetValue(const char * name, char * buffer)
{
	if(m_Handle == 0)
		return -1;
	strcpy(buffer,name);
	strcat(buffer,"?\n"); //добавить
	int nread;
	try
	{
		Write(buffer,(int)strlen(buffer)); 
	}
	catch(...)
	{
		buffer[0] = 0;
		return 1;
	};
	try
	{
		nread = Read(buffer, 256);
	}
	catch(...)
	{
		buffer[0] = 0;
		return 2;
	}
	if(nread == 0)
		return 2;
	if(buffer[nread-2] == '\n')
		buffer[nread-2] = 0;  //'\0'!!!!
	else
		buffer[nread-1] = 0;
	return 0;
}

int PTC10::SetValue(const char * name, char * value)
{
	if(m_Handle == 0)
		return -1;
	char b0[256];
	strcpy(b0,name); // strcat(b0,"=");?
	strcat(b0,value);
	strcat(b0,"\n");
	try
	{
		Write(b0,(int)strlen(b0));
	}
	catch(...)
	{
		return 1;
	};
	return 0;
}

int PTC10::GetDeviceID(char * buffer)
{
	return GetValue("*IDN",buffer);
}

int PTC10::GetChannels(char * names[MAX_CHANNELS], int *nchan)
{
	char b0[1000];
	int nres = GetValue("getOutputNames",b0);
	if(nres)
		return nres;
	int i = 0, p = 0, q = 0;
        // q - index of b0
        // p - index of names
	while((i < MAX_CHANNELS)&&(b0[q]))
	{
		if((b0[q] == ',')||(b0[q] == 0)) // (b0[q] == 0)?
		{
			names[i][p] = 0;
			p = 0;
			i++;
			q++;
		}
		else
			names[i][p++] = b0[q];
		q++;
	}
	*nchan = i;
	nchannels = i;
	return 0;
}
	
int PTC10::GetValues(Ar1<double> * ar)
{
	char b0[1000];
	int nres = GetValue("getOutput",b0);
	if(nres)
		return nres;
	double vals[MAX_CHANNELS];
	char b1[100];
	int i = 0, p = 0, q = 0;
	while((i < MAX_CHANNELS)&&(b0[q]))
	{
		if((b0[q] == ',')||(b0[q] == 0))
		{
			b1[p] = 0;
			vals[i] = strtod(b1,NULL);
			p = 0;
			i++;
			q++;
		}
		else
			b1[p++] = b0[q];
		q++;
	}
	ar->setn(nchannels_used);
	for(q = 0; q < nchannels_used; q++)              //?
		(*ar)[q] = vals[channels_used[q]];       //?
	return 0;
}
