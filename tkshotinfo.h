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

private:
	int adc_id;

	std::string data_file_name;

	int trace_total_number;

	std::string model_name;
	BYTEORDER byte_order;
	DATAFORMAT data_format;
	int data_offset;

	/**
	* @struct CHDATA
	* @blief チャンネル構造体
	*/
	struct CHDATA
	{
		//! チャンネル番号
		int ch_number;
		//! ラベル名
		std::string trace_name;
		//! 量子化単位 (LSB)
		float v_resolution;
		//! オフセット電圧
		float v_offset;
		//! 
		int v_max_data;
		//! 
		int v_min_data;
		//! サンプリング時間 (時間分解能)
		float h_resolution;
		//! オフセット時間
		float h_offset;
		//! ブロックサイズ
		int block_size;
		//! 時刻 (どの時刻かはADCによって異なる)
		struct std::tm time;
	};
	//! チャンネルデータ
	std::vector<TKDATA::CHDATA> CHData;
	//	TKDATA::CHDATA CHData[TKADC_ADC_CHANNEL_MAX];
	//! チャンネル番号からトレース番号への変換マトリックス。インターフェース関数ChannnelNumberToTraceNumber()を使用してください。
	int channel_number_to_trace_number[TKADC_ADC_CHANNEL_MAX];

	inline void readHDRLineString(std::string buf, int trace_number, char* key_name, std::string* odata);
	template<typename T> int readHDRLine(std::string buf, int trace_number, char* key_name, T* data);
	int readHDRLine(std::string buf, int trace_number, char* key_name, float* data);
	int readHDRLine(std::string buf, int trace_number, char* key_name, int* data);
	int readHDRLine(std::string buf, int trace_number, char* key_name, struct std::tm* data);
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
	int traceNameToCHNumber(std::string trace_name);

	//! point data (ARPISTA)
	//! [0]:time, [1]:CH1, ... データはeveryだけ間引かれている
	std::vector<std::vector<double> > points;
	unsigned int every;
//	bool is_empty;

public:
	TKDATA()
	{
		for (int i = 0; i < TKADC_ADC_CHANNEL_MAX; i++)
			channel_number_to_trace_number[i] = -1;
	}
	/**
	* @fn ParseHDR
	* @blief ヘッダーファイルのパースを行います。
	*
	*	ヘッダーファイルからバイナリーデータフォーマット及び計測設定情報を取得します。
	* @return 0:
	*	正常終了
	*/
	int ParseHDR();
	int SetADCID(int iadc_id)
	{
		adc_id = iadc_id;
		return 0;
	}
	int GetADCID()
	{
		return adc_id;
	}
	int SetDataFileName(std::string idata_file_name)
	{
		data_file_name = idata_file_name;
		return 0;
	}
	std::string GetDataFileName()
	{
		return data_file_name;
	}
	float GetHResolution()
	{
		return CHData[0].h_resolution;
	}
	float GetVOffset(int const trace_index)
	{
		return CHData[trace_index].v_offset;
	}
	float GetVResolution(int const trace_index)
	{
		return CHData[trace_index].v_resolution;
	}
	int GetVMaxData(int const trace_index)
	{
		return CHData[trace_index].v_max_data;
	}
	int GetVMinData(int const trace_index)
	{
		return CHData[trace_index].v_min_data;
	}
	unsigned int GetBlockSize()
	{
		return CHData[0].block_size;
	}
	struct std::tm GetTime()
	{
		return CHData[0].time;
	}
	float GetHOffset()
	{
		return CHData[0].h_offset;
	}
	std::string GetModelName()
	{
		return model_name;
	}
	TKDATA::BYTEORDER GetByteOrder()
	{
		return byte_order;
	}
	TKDATA::DATAFORMAT GetDataFormat()
	{
		return data_format;
	}
	int GetDataOffset()
	{
		return data_offset;
	}
	unsigned int GetTraceNumber()
	{
		return trace_total_number;
	}
//	int ChannnelNumberToTraceNumber(int const channel_number)
//	{
//		return channel_number_to_trace_number[channel_number];
//	}
//	int GetChannelNumber(int const trace_index)
//	{
//		return CHData[trace_index].ch_number;
//	}

	//! ARPISTA
	std::vector<std::vector<double> >* GetDataPointsPtr()
	{
		return &points;
	}
	unsigned int SetEvery(unsigned int const every_)
	{
		return every = every_;
	}
	unsigned int GetEvery()
	{
		return every;
	}
	unsigned int LoadDataPoints(unsigned int every = 0, unsigned int start_position = 0, unsigned int point_number = UINT_MAX)
	{
//		points = std::vector<std::vector<double> >(this->GetTraceTotalNumber() + 1, std::vector<double>(point_number, 0.0f));
		if (point_number > this->GetBlockSize() / (every + 1))
			point_number = this->GetBlockSize() / (every + 1);

		points.resize(this->GetTraceNumber() + 1, std::vector<double>(point_number, 0.0f));

		std::ifstream ifsRawBin;
		ifsRawBin.open(this->GetDataFileName() + ".WVF", std::ios::in | std::ios::binary );

		// time axis
		for (unsigned int j = 0; j < point_number; j++)
			points[0][j] = this->GetHOffset() + this->GetHResolution() * (start_position + (every + 1) * j);

		// for IS2
		for (unsigned int i = 0; i < this->GetTraceNumber(); i++) {
			unsigned int sp;
			unsigned char bytes[2];
			unsigned short decoded_integer;
			sp = this->GetDataOffset() + (this->GetBlockSize() * i) * 2;

			for (unsigned int j = 0;
			     j < point_number;
			     j++) {
				ifsRawBin.seekg(sp + (start_position + (every + 1) * j) * 2, std::ios::beg);
				ifsRawBin.read(reinterpret_cast<char*>(bytes), 2);
				switch (this->GetByteOrder()) {
				case TKDATA::BYTEORDER::BIG_ENDIAN:
					decoded_integer = bytes[0] << 8 | bytes[1];
					break;
				case TKDATA::BYTEORDER::LITTLE_ENDIAN:
					decoded_integer = bytes[0] | bytes[1] << 8;
					break;
				}
				points[i + 1][j] = this->GetVOffset(i) + static_cast<signed short>(decoded_integer) * this->GetVResolution(i);
			}
		}

		return point_number;
	}
	std::vector<std::vector<double> >& GetDataPoints()
	{
		return points;
	}
//	bool& IsEmpty()
//	{
//		return TKDATA::is_empty;
//	}



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

	int getADCDataIndexByADCID(int adc_id)
	{
		for (int i = 0; i < adc_num; i++)
			if (TKData[i].GetADCID() == adc_id)
				return i;
		return -1;
	}

public:
	TKSHOT()
	{
		this->Clear();
	}
	~TKSHOT()
	{

	}

	unsigned int GetADCNumber()
	{
		return adc_num;
	}
	std::string GetDataFileName(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetDataFileName();
	}

	int NameShotNumber(int ishot_number)
	{
		shot_number = ishot_number;
		return 0;
	}
	int Clear()
	{
		adc_num = 0;
		TKData.clear();
		total_trace_number = 0;
		return 0;
	}
	int AppendDataFile(std::string data_file_name)
	{

		TKDATA *this_data;
		adc_num++;
		TKData.push_back(TKDATA());
		this_data = &(TKData[adc_num - 1]);
		this_data->SetADCID(adc_num - 1);
		this_data->SetDataFileName(data_file_name);
		this_data->ParseHDR();
//		this_data->IsEmpty() = true;

		total_trace_number += this->GetTraceNumber(adc_num - 1);

		return adc_num-1;
		/*
		adc_num++;
		TKData.push_back(TKDATA::TKDATA());
		TKData[adc_num - 1].SetADCID(adc_id);
		TKData[adc_num - 1].SetDataFileName(data_file_name);
		TKData[adc_num - 1].ParseHDR();
		return 0;
		*/
	}
	float GetHResolution(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetHResolution();
	}
	int GetBlockSize(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetBlockSize();
	}
	float GetVOffset(int adc_id, int trace_index)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetVOffset(trace_index);
	}
	float GetVResolution(int adc_id, int trace_index)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetVResolution(trace_index);
	}
	int GetVMaxData(int adc_id, int trace_index)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetVMaxData(trace_index);
	}
	int GetVMinData(int adc_id, int trace_index)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetVMinData(trace_index);
	}
	float GetHOffset(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetHOffset();
	}
	std::string GetModelName(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetModelName();
	}
	struct std::tm GetTime(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetTime();
	}
	TKDATA::BYTEORDER GetByteOrder(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetByteOrder();
	}
	TKDATA::DATAFORMAT GetDataFormat(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetDataFormat();
	}
	int GetDataOffset(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetDataOffset();
	}
	// Function name changed TKSHOT::GetTraceTotalNumber() to TKSHOT::GetTraceNumber()
	unsigned int GetTraceNumber(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetTraceNumber();
	}
	int ADCIDToADCDataIndex(int adc_id)
	{
		return getADCDataIndexByADCID(adc_id);
	}
	unsigned int GetADCID(int adc_index)
	{
		return TKData[adc_index].GetADCID();
	}
//	int GetChannelNumber(int adc_id, int trace_index)
//	{
//		return TKData[getADCDataIndexByADCID(adc_id)].GetChannelNumber(trace_index);
//	}

	//! ARPISTA
	unsigned int SetEvery(int adc_id, unsigned int const every)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].SetEvery(every);
	}
	unsigned int GetEvery(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetEvery();
	}
	unsigned int LoadDataPoints(int adc_id, unsigned int every = 0, unsigned int start_position = 0, unsigned int point_number = UINT_MAX)
	{
	return TKData[getADCDataIndexByADCID(adc_id)].LoadDataPoints(every, start_position, point_number);
	}
	std::vector<std::vector<double> >* GetDataPointsPtr(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetDataPointsPtr();
	}
	std::vector<std::vector<double> >& GetDataPoints(int adc_id)
	{
		return TKData[getADCDataIndexByADCID(adc_id)].GetDataPoints();
	}
	std::vector<TKDATA>& Data()
	{
		return TKData;
	}
	unsigned int GetTotalTraceNumber()
	{
		return total_trace_number;
	}
};


#endif
