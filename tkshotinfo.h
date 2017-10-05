/**
* @file	tkshotinfo.h
* @brief ショット情報を取得・管理するクラスを宣言しています。
* @author Kobayashi Takahiko
* @date 2017
*/

#include "tkadc.h"
#include "tkadcinfo.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
//#include "clx/ini.h"
//#include "clx/ftp.h"
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>
#ifndef __TKSHOTINFO__
#define __TKSHOTINFO__

#define NATIVE_ENDIAN TKDATA::BYTEORDER::LITTLE_ENDIAN

#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <limits>
#include "tkadcconst.h"
//#define TKSI_ADC_MAX 

/**
* @class TKDATA
* @blief データクラス
*
*	ADCで取得したデータを管理するクラスです。<br>
*	1つのデータクラスが管理するのは1台のADCで計測した1ショット分のデータです。
*/
class TKDATA
{

private:
	unsigned int adc_id;

	std::string data_file_name;

	unsigned int trace_total_number;

	std::string model_name;
	TKADCCONST::BYTEORDER byte_order;
	TKADCCONST::DATAFORMAT data_format;
	unsigned int data_offset;

	/**
	* @struct CHDATA
	* @blief チャンネル構造体
	*/
	struct CHDATA
	{
		//! チャンネル番号
		TKADCCONST::CHANNEL ch_number;
		//! ラベル名
		std::string trace_name;
		//! 量子化単位 (LSB)
		float v_resolution;
		//! オフセット電圧
		float v_offset;
		//! 
		unsigned int v_max_data;
		//! 
		unsigned int v_min_data;
		//! サンプリング時間 (時間分解能)
		float h_resolution;
		//! オフセット時間
		float h_offset;
		//! ブロックサイズ
		unsigned int block_size;
		//! 時刻 (どの時刻かはADCによって異なる)
		struct std::tm time;
		//! 推測値
		//! V/Div
		TKADCCONST::VDIV v_div;
	};
	//! チャンネルデータ
	std::vector<TKDATA::CHDATA> CHData;
	//	TKDATA::CHDATA CHData[TKADC_ADC_CHANNEL_MAX];
	//! チャンネル番号からトレース番号への変換マトリックス。インターフェース関数ChannnelNumberToTraceNumber()を使用してください。
	int channel_number_to_trace_number[TKADC_ADC_CHANNEL_MAX];

	inline void readHDRLineString(std::string buf, unsigned int trace_number, char* key_name, std::string* odata);
	template<typename T> void readHDRLine(std::string buf, unsigned int trace_number, char* key_name, T* data);
	void readHDRLine(std::string buf, unsigned int trace_number, char* key_name, float* data);
	void readHDRLine(std::string buf, unsigned int trace_number, char* key_name, int* data);
	void readHDRLine(std::string buf, unsigned int trace_number, char* key_name, struct std::tm* data);
	/**
	* @fn traceNameToCHNumber
	* @blief ラベル名からチャンネル番号を推定します。
	* @param[in] trace_name
	*	ラベル名。
	* @return 0:
	*	該当無し
	* @return 0以外:
	*	チャンネル番号
	* @note
	*	ラベル名が"CH"+<チャンネル番号>であることを前提とした実装です。
	* @note
	*	横河電機のADCにおいてラベル名はチャンネル番号を特定する唯一の情報源です。
	* @warning
	*	この仕様によりADCのラベル名を変更することはできません。
	*/
	TKADCCONST::CHANNEL traceNameToCHNumber(std::string trace_name);

	//! point data (ARPISTA)
	//! [0]:time, [1]:CH1, ... データはeveryだけ間引かれている
	std::vector<std::vector<double> > points;
	unsigned int every;
	TKADCCONST::VDIV v_resolutionToV_div(float v_resolution);

public:
	TKDATA();
	/**
	* @fn ParseHDR
	* @blief ヘッダーファイルのパースを行います。
	*
	*	ヘッダーファイルからバイナリーデータフォーマット及び計測設定情報を取得します。
	* @return 0:
	*	正常終了
	*/
	void ParseHDR();
	void SetADCID(unsigned int const iadc_id);
	unsigned int GetADCID();
	void SetDataFileName(std::string idata_file_name);
	std::string GetDataFileName();
	float GetHResolution();
	float GetVOffset(int const trace_index);
	float GetVResolution(int const trace_index);
	unsigned int GetVMaxData(int const trace_index);
	unsigned int GetVMinData(int const trace_index);
	unsigned int GetBlockSize();
	struct std::tm GetTime();
	float GetHOffset();
	std::string GetModelName();
	TKADCCONST::BYTEORDER GetByteOrder();
	TKADCCONST::DATAFORMAT GetDataFormat();
	int GetDataOffset();
	unsigned int GetTraceNumber();

	//! ARPISTA
	std::vector<std::vector<double> >* GetDataPointsPtr();
	unsigned int SetEvery(unsigned int const every_);
	unsigned int GetEvery();
	unsigned int LoadDataPoints(unsigned int every = 0, unsigned int start_position = 0, unsigned int point_number = UINT_MAX);
	std::vector<std::vector<double> >& GetDataPoints();
};

/**
* @class TKSHOT
* @blief ショットクラス
*
*	ADCで取得したデータを管理するクラスです。<br>
*	1つのショットクラスが管理するのは1ショット分のすべてのデータです。
*/
class TKSHOT
{
private:
	int shot_number;
	bool is_current_shot;

	int adc_num = 0;
	std::vector<TKDATA> TKData;
	//	TKDATA TKData[2];

	int plot_num;
	//	std::vector<TKPLOT> TKPlot;
	int total_trace_number = 0;

	int getADCDataIndexByADCID(int adc_id);

public:
	TKSHOT();
	~TKSHOT() {}
//	std::vector<TKDATA>::iterator operator [](unsigned int n); //(*thisShot)[i]
//	std::vector<TKDATA>::iterator operator [](const unsigned int n);

	unsigned int GetADCNumber();

	int NameShotNumber(int ishot_number);
	int Clear();
	int AppendDataFile(std::string data_file_name);
	int ADCIDToADCDataIndex(int adc_id);
	unsigned int GetADCID(int adc_index);

	//! ARPISTA
	std::vector<TKDATA>& Data();
	unsigned int GetTotalTraceNumber();
	TKDATA& Data(unsigned int n);
};


#endif
