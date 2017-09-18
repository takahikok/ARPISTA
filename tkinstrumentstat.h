#ifndef TKINSTRUMENTSTAT_H
#define TKINSTRUMENTSTAT_H

#include <vector>
#include <string>
#include <ctime>

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
		enum RECORDLENGTH : unsigned int
		{
			_1k = 1000,
			_2_5k = 2500,
			_5k = 5000,
			_10k = 10000,
			_25k = 25000,
			_50k = 50000,
			_100k = 100000,
			_250k = 250000,
			_500k = 500000,
			_1M = 1000000,
			_2_5M = 2500000,
			_5M = 5000000,
			_10M = 10000000,
			_25M = 25000000,
			_50M = 50000000,
			_100M = 100000000,
		};
	private:
		std::string file_name;
			//ACQUIRE:RTOUT:FILENAME
	};

	class CHANNEL
	{
	public:
		enum BANDWIDTH
		{
			FULL,
		};
		enum COUPLING
		{
			AC,
			DC1MR,
			DC50R,
		};
		enum BANDWIDTH
		{
			AC,
		};
		enum ATTENUATOR : unsigned int
		{
			_1_1 = 1,
			_1_10 = 10,
			_1_1000 = 1000,
		};
		enum VDIV
		{
			_10V
		};
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
		enum SAMPLINGRATE : unsigned int
		{
			_5 = 5,
			_10 = 10,
			_100 = 100,
		};
	private:
		SAMPLINGRATE sampling_rate;
			//TIMEBASE:SRATE
			//TDIVもあるがどちらかでよいだろう

	};

	class TRIGGER
	{
	public:
		enum SOURCE
		{
			CH1,
			CH2,
			CH3,
			CH4,
			CH5,
			CH6,
			CH7,
			CH8,
			CH9,
			CH10,
			CH11,
			CH12,
			CH13,
			CH14,
			CH15,
			CH16,
			EXTERNAL,
		};
			//:TRIGger:SIMPle:SOURce
		enum SLOPE
		{
			FALL, RISE
		};
			//#:TRIGger[:SIMPle]:EXTernal:SLOPe
			//:TRIGger[:SIMPle]:SLOPeをソースが変わるたびに聞きに行けばよい？
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
