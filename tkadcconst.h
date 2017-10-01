#ifndef TKADCCONST_H
#define TKADCCONST_H


/**
* @class TKADCCONST tkadcconst.h
* @blief ADC定数
*/
class TKADCCONST {
public:
public:
	/**
	* @enum BYTEORDER
	* @blief バイトオーダー列挙型
	*
	*	バイナリーファイルのバイトオーダーです。<br>
	*	バイナリーファイルから直接データを読み出す際に必要です。
	*/
	enum class BYTEORDER
	{
		//! 680xx
		BIG_ENDIAN,
		//! 80x86
		LITTLE_ENDIAN
	};
	/**
	* @enum DATAFORMAT
	* @blief データフォーマット列挙型
	*
	*	バイナリーファイルのデータフォーマットです。<br>
	*	詳細は横河電機のドキュメントを参照してください。
	*/
	enum class DATAFORMAT
	{
		TRACE,
		BLOCK
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
	enum BANDWIDTH : unsigned int
	{
		_10kHz = 10000,
		_20kHz = 20000,
		_40kHz = 40000,
		_80kHz = 80000,
		_160kHz = 160000,
		_320kHz = 320000,
		_640kHz = 640000,
		_1_28MHz = 1280000,
		_2MHz = 2000000,
		FULL,
	};
	enum COUPLING
	{
		AC,
		DC1MR,
		DC50R,
	};
	enum ATTENUATOR : unsigned int
	{
		_1_1 = 1,
		_1_10 = 10,
		_1_1000 = 1000,
	};
	enum VDIV : unsigned int
	{
		_1mV_div = 1,
		_2mV_div = 2,
		_5mV_div = 5,
		_10mV_div = 10,
		_20mV_div = 20,
		_50mV_div = 50,
		_100mV_div = 100,
		_200mV_div = 200,
		_500mV_div = 500,
		_1V_div = 1000,
		_2V_div = 2000,
		_5V_div = 5000,
		_10V_div = 10000,
		_20V_div = 20000,
		_50V_div = 50000,
		_100V_div = 100000,
		_200V_div = 200000,
	};
	enum SAMPLINGRATE : unsigned int
	{
		_5 = 5,
		_10 = 10,
		_100 = 100,
	};
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

};

#endif // TKADCCONST_H
