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
* @class TKSHOT
* @blief ショットクラス
*
*	ADCで取得したデータを管理するクラスです。<br>
*	1つのショットクラスが管理するのは1ショット分のすべてのデータです。
*/
class TKSHOT
{
private:
	/**
	* @class TKDATA
	* @blief データクラス
	*
	*	ADCで取得したデータを管理するクラスです。<br>
	*	1つのデータクラスが管理するのは1台のADCで計測した1ショット分のデータです。
	*/
	class TKDATA
	{
		friend class TKSHOT;
	private:
		/**
		* @struct CHDATA
		* @blief チャンネル構造体
		*/
		class CHDATA
		{
		private:
			//! チャンネル番号
			TKADCCONST::CHANNEL ch_number;
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
			unsigned int block_size;
			//! 時刻 (どの時刻かはADCによって異なる)
			struct std::tm time;
			//! 推測値
			//! V/Div
			TKADCCONST::VDIV v_div;

		public:
			void SetChNumber(const TKADCCONST::CHANNEL& ch_number);
			void SetTraceName(const std::string& trace_name);
			void SetVResolution(const float& v_resolution);
			void SetVOffset(const float& v_offset);
			void SetVMaxData(const int& v_max_data);
			void SetVMinData(const int& v_min_data);
			void SetHResolution(const float& h_resolution);
			void SetHOffset(const float& h_offset);
			void SetBlockSize(const unsigned int& block_size);
			void SetV_div(const TKADCCONST::VDIV& v_div);
			void SetTime(const std::tm& time);

			const TKADCCONST::CHANNEL& GetChNumber() const;
			const std::string& GetTraceName() const;
			const float& GetVResolution() const;
			const float& GetVOffset() const;
			const int& GetVMaxData() const;
			const int& GetVMinData() const;
			const float& GetHResolution() const;
			const float& GetHOffset() const;
			const unsigned int& GetBlockSize() const;
			const TKADCCONST::VDIV& GetV_div() const;
			const struct std::tm& GetTime() const;
		};

	private:
		unsigned int adc_id;

		std::string data_file_name;

		unsigned int trace_total_number;

		std::string model_name;
		TKADCCONST::BYTEORDER byte_order;
		TKADCCONST::DATAFORMAT data_format;
		unsigned int data_offset;

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
		* @fn traceNameToChNumber
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
		const TKADCCONST::CHANNEL traceNameToChNumber(std::string const& trace_name) const;

		//! point data (ARPISTA)
		//! [0]:time, [1]:CH1, ... データはeveryだけ間引かれている
		std::vector<std::vector<double> > points;
		unsigned int every;
		const TKADCCONST::VDIV v_resolutionToV_div(const float& v_resolution) const;
		void estimateParameters();
		void setDataFileName(std::string const& idata_file_name);
		void parseHDR();

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
		void LoadHDR(const std::string& data_file_name);
		void SetADCID(unsigned int const iadc_id);
		const unsigned int& GetADCID() const;
		const std::string& GetDataFileName() const;
		const float& GetHResolution() const;
		const unsigned int& GetBlockSize() const;
		const struct std::tm& GetTime() const;
		const float& GetHOffset() const;
		const std::string& GetModelName() const;
		const TKADCCONST::BYTEORDER& GetByteOrder() const;
		const TKADCCONST::DATAFORMAT& GetDataFormat() const;
		const unsigned int& GetDataOffset() const;
		const unsigned int& GetTraceNumber() const;

		//! ARPISTA
		std::vector<std::vector<double> >* GetDataPointsPtr();
		void SetEvery(const unsigned int& every);
		const unsigned int& GetEvery() const;
		unsigned int LoadDataPoints(unsigned int const every = 0, unsigned int const start_position = 0, unsigned int point_number = UINT_MAX);
		const std::vector<std::vector<double> >& GetDataPoints() const;
		TKDATA::CHDATA& Ch(unsigned int const index);
		const TKDATA::CHDATA& Ch(unsigned int const index) const;
	};

private:
	int shot_number;
	bool is_current_shot;

	int adc_num = 0;
	std::vector<TKDATA> TKData;
	//	TKDATA TKData[2];

	int plot_num;
	//	std::vector<TKPLOT> TKPlot;
	int total_trace_number = 0;

	const int getADCDataIndexByADCID(const int& adc_id) const;

public:
	TKSHOT();
	~TKSHOT() {}
//	std::vector<TKDATA>::iterator operator [](unsigned int n); //(*thisShot)[i]
//	std::vector<TKDATA>::iterator operator [](const unsigned int n);

	const unsigned int GetADCNumber() const;

	void NameShotNumber(int const ishot_number);
	void Clear();
	const unsigned int AppendDataFile(std::string const& data_file_name);
	const int ADCIDToADCDataIndex(int const& adc_id) const;
	const unsigned int GetADCID(int const& adc_index) const;

	//! ARPISTA
	const unsigned int GetTotalTraceNumber() const;
	TKDATA& Data(unsigned int const& index);
	const TKDATA& Data(unsigned int const& index) const;
	const unsigned int TotalTraceIndexToDataIndex(const unsigned int& total_index);
	const unsigned int TotalTraceIndexToChIndex(const unsigned int& total_index);
	TKSHOT::TKDATA::CHDATA& TotalTraceIndexToCh(const unsigned int& total_index);
};


#endif
