#ifndef TKINSTRUMENTSTAT_H
#define TKINSTRUMENTSTAT_H

#include <vector>
#include <string>
#include <ctime>
#include "tkadcconst.h"

class TKINSTRUMENTSTAT
{

};

class TKADCSTAT : public TKINSTRUMENTSTAT
{
private:
	std::string device_name;
	std::string model_name;
	bool is_enable;

public:
	class ACQUIRE
	{
	public:
		enum MODE
		{

		};
	private:
		std::string file_name;
			//ACQUIRE:RTOUT:FILENAME
	};

	class CHANNEL
	{
	public:
	private:
		float v_offset;
		unsigned int serial_number;
			//SYSTEM:CHANNEL<x>:NUMBER?

	};

	class SYSTEM
	{
	public:
	private:
		std::time_t time;
			//SYSTEM:CLOCK:DATE
			//SYSTEM:CLOCK:TIME
	};

	class TIMEBASE
	{
	public:
	private:
		SAMPLINGRATE sampling_rate;
			//TIMEBASE:SRATE
			//TDIVもあるがどちらかでよいだろう

	};

	class TRIGGER
	{
	public:
	private:
		unsigned int position;
			//:TRIGger:POSition
			//floatで受けintで設定する

	};

	std::vector<CHANNEL> Channel;
	std::vector<SYSTEM> System;
	std::vector<TIMEBASE> TimeBase;

public:
	TKADCSTAT(unsigned int channnel_number = 0)
	{
		Channel.reserve(channnel_number);
	}
}

#endif // TKINSTRUMENTSTAT_H
