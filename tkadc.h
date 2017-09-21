/**
* @file	tkadc.h
* @brief ADCをコントロールするためのクラスを宣言しています。
*	ADCの制御に関して、モデルに依存する部分はここに記述してください。
* @author Kobayashi Takahiko
* @date 2017
*/

#include <string>
#include <iostream>
#include "tmctlqt.h"

#ifndef __TKADC__
#define __TKADC__

//#define DEVICE_MAX 2
#define TKADC_ADC_MODEL_DL750 TKADCCONTROL::ADCMODEL::DL750
#define TKADC_ADC_MODEL_DL850 TKADCCONTROL::ADCMODEL::DL850
#define TKADC_ADC_CHANNEL_MAX 18

/**
* @class TKADCCONTROL tkadc.h
* @blief ADCコントロールクラス
*
*	ADCコントロールクラスはADCをコントロールするための基底クラスです。<br>
*	ADCとのコネクションは排他的であるため、あるADCのコントロールには1対1に対応するADCコントロールクラスオブジェクトを作成する必要があります。<br>
* @par この基底クラスが作られた背景
*	ADCモデルによっては機能が制限されることがあります。例えばDL750はVXI11に対応していません。DL1740のコントロールにはEthernetを用いることができません。<br>
*	また、DL750とDL850のステータスビットの違いのように、ADCモデルによっては同じ機能でも実装が異なることがあります。<br>
*	このような実装の違いもメソッドをオーバーロードさせることで同様に扱うことができます。<br>
* @note
*	もし新たな計測器を追加するようなことがあれば、可能な限りADCコントロールクラスを派生させて多態性を持たせてください。
*/
class TKADCCONTROL {
public:
	/**
	* @enum ADCMODEL
	* @blief ADCモデル列挙型
	*
	*	ADCのモデルを区別するための型です。<br>
	*	ADCモデルは多くの場合ADCの型番を指します(あるいは商品名と呼べるかもしれません)。<br>
	*	ADCモデルはあるADCについての固有な名前を意味しません。従って複数の同じADCがシステムに組み込まれている場合は、
	*	それら複数のADCが同じADCモデル名を持ちます。<br>
	*	ADCモデルという呼称は横河電機の流儀に従ったものです。
	*/
	enum class ADCMODEL {
		DL750,
		DL850
	};

	/**
	* @enum CONDITIONFLAG
	* @blief コンディションビットマスク列挙型
	*
	*	ビットマスクはモデルによって異なるため、実際にビットマスクとして使用する為には継承されたクラスで再定義する必要があります。
	*/
	enum class CONDITIONFLAG {
		ALL = 0xffff
	};

protected:

private:
	ADCMODEL adc_model;
	int device_id;
	int wire_type;
	char adress[256];
	char name[256];
	char file_name_header[32];
	char file_path[32];
	int next_local_shot_number;
	int local_shot_number_max = 99999999;


public:
	class Exception {
	public:
		int adc_id;	//TKADCINFO::ADCID
		int error_number;
	public:
		Exception(int adc_id_, int error_number_) : adc_id(adc_id_), error_number(error_number_)
		{
		}
	};

public:
	TKADCCONTROL(ADCMODEL adcmodel);
	~TKADCCONTROL();
	ADCMODEL Model();


	/**
	* @fn Open
	* @blief ADCとの通信を開きます。
	*
	*	開いた通信はClose()関数によって必ず閉じる必要があります。
	* @param[in] wire_type_
	*	通信プロトコルを選択します。
	* @param[in] adress_
	*	アドレスを指定します。
	* @return 0:
	*	正常終了
	* @return 0以外:
	*	tmctl.hで定義されたエラーコードを返します
	* @note
	*	Close()に失敗すると通信が成立しないので、通信が開かない場合は一度ADCを再起動させてください。
	*	電源の再投入が許されるまでの時間は各ADCのマニュアルを参照してください。
	*/
	int Open(int wire_type_, const char* adress_);


	/**
	* @fn Close
	* @blief ADCとの通信を閉じます。
	*
	*	デストラクタから呼ばれます。
	* @return 0:
	*	正常終了
	* @return 0以外:
	*	tmctl.hで定義されたエラーコードを返します
	* @note
	*	Close()に失敗すると次の通信が成立しないので、Close()できなかった場合はADCを再起動させてください。<br>
	*	電源の再投入が許されるまでの時間は各ADCのマニュアルを参照してください。
	*/
	int Close();


	/**
	* @fn SendMessage
	* @blief ADCにメッセージを送信します。
	*
	*	ADCをコントロールする手段としては最も低水準な関数です。
	* @param[in] message
	*	ADCに送信するメッセージを指定します。
	* @return 0:
	*	正常終了
	* @return 0以外:
	*	tmctl.hで定義されたエラーコードを返します
	*/
	int SendMessage(const char* message);


	/**
	* @fn Start
	* @blief ADCの計測を開始します。
	* @return 0:
	*	正常終了
	* @return 0以外:
	*	tmctl.hで定義されたエラーコードを返します
	* @note
	*	Singleでの使用を前提としています。
	* @note
	*	Start()が呼ばれた事とADCの波形取り込みの準備が整った事（実際に"Start"している事）は同義ではありません。<br>
	*	例えばDL850はStart()が呼ばれた後、高い頻度でキャリブレーションを実行します。この間波形を取り込むことはできません。<br>
	*	波形が取り込める状態かどうかはGetStatusCondition(TKADCCONTROL::CONDITIONFLAG::TRG)等で調べる必要があります。
	*/
	int Start();


	/**
	* @fn Stop
	* @blief ADCの計測を終了します。
	* @return 0:
	*	正常終了
	* @return 0以外:
	*	tmctl.hで定義されたエラーコードを返します
	*/
	int Stop();


	/**
	* @fn WaitADC
	* @blief ADCがビジー状態の間待ち続けます。
	* @param[in] flag
	*	コンディションのビットマスク条件を指定します。省略した場合はCONDITIONFLAG::ALLになります。
	* @return 0:
	*	正常終了
	* @return 0以外:
	*	tmctl.hで定義されたエラーコードを返します
	*/
	int WaitADC(TKADCCONTROL::CONDITIONFLAG flag = CONDITIONFLAG::ALL);


	/**
	* @fn SaveShot
	* @blief ADCが取り込んだ波形を指定したファイル名でADCの内部ストレージに保存します。
	*
	*	バイナリファイルです。既存のファイル名が指定された場合の挙動はADCの仕様によります。
	* @param[in] file_name
	*	保存ファイル名を指定します。拡張子は不要です。
	* @return 0:
	*	正常終了
	* @return 0以外:
	*	tmctl.hで定義されたエラーコードを返します
	* @warning
	*	DL750, DL850 ではファイルは上書き保存されず36進数で自動インクリメントされた名前が採用されます。<br>
	*	指定した名前と異なるファイルに保存される可能性があることに注意してください。
	* @todo
	*	保存されたファイル名の取得
	* @todo
	*	許されないファイル名を拒否する
	*/
	int SaveShot(std::string file_name);


	/**
	* @fn GetDeviceID
	* @blief ADCとの通信によって生成されるデバイスIDを取得します。
	*
	*	通常このデバイスIDを知る必要はありません。
	* @return
	*	デバイスIDを返します
	*/
	int GetDeviceID();


	/**
	* @fn GetStatusCondition
	* @blief ADCのビジー状態を取得します。
	* @param[in] flag
	*	ビジー要因のビットマスク条件を指定します。省略した場合はCONDITIONFLAG::ALLになります。
	* @return 0:
	*	指定したビットマスク条件のうちビジー状態のものはありません。
	* @return 0以外:
	*	指定したビットマスク条件のうち少なくとも1つはビジー状態です。
	*/
	virtual int GetStatusCondition(TKADCCONTROL::CONDITIONFLAG flag = CONDITIONFLAG::ALL);


	/**
	* @fn Delete
	* @blief ADCの内部ストレージから指定したファイル名のバイナリデータを削除します。
	*
	*	存在しないファイル名が指定された場合の挙動はADCの仕様によります。
	* @param[in] file_name
	*	削除ファイル名を指定します。拡張子は不要です。
	* @return 0:
	*	正常終了
	* @return 0以外:
	*	tmctl.hで定義されたエラーコードを返します
	* @note
	*	DL750, DL850 では存在しないファイル名が指定された場合、ADCの画面に警告が出るに留まり、コントロールは継続されます。<br>
	*	削除が実行されるとディスクアクセスが発生するためWaitADC()等で待つ必要があります。
	* @todo
	*	許されないファイル名を拒否する
	*/
	virtual int Delete(std::string file_name);

public:
	//local shot number 
	/**
	* @todo
	*	ADCコントロールクラスに存在するのは不適当なので別のクラスに移動させる
	*/
	int GetLastLocalShotNumber();
	int GetNextLocalShotNumber();
	int SetLastLocalShotNumber(int new_local_shot_number);
	int IncrementLocalShotNumber();
	int SetLocalShotNumberMax(int new_local_shot_number_max);
	int GetLocalShotNumberMax();
};




/**
* @class TKADCCONTROL_DL750
* @blief DL750ADCコントロールクラス
*
*	DL750をコントロールするためのクラスです。
*/
class TKADCCONTROL_DL750 : public TKADCCONTROL {
public:

	//The condition flag is depending on the ADC model.
	//Details are described in the YOKOGAWA IM701210-18 document.

	enum class CONDITIONFLAG {
		ALL = 0xffff,	//16bit
		RUN = 1 << 0,	//Running
		TRG = 1 << 2,	//Awaiting trigger
		TRGINV = ALL - TRG,
		CAL = 1 << 3,	//Calibration
		TST = 1 << 4,	//Testing
		PRN = 1 << 5,	//Printing
		ACS = 1 << 6,	//Accessing
		MES = 1 << 7,	//Measuring
		HST = 1 << 8,	//History Search
		SUP = 1 << 9,	//Setup
		NGO = 1 << 10,	//Go/No-Go
		SCH = 1 << 11,	//Search
		NSS = 1 << 12,	//N-Single
		INI = 1 << 13,	//Initializing
		FFT = 1 << 14,	//FFT
	};

public:
	TKADCCONTROL_DL750();
	int GetStatusCondition(TKADCCONTROL_DL750::CONDITIONFLAG flag = CONDITIONFLAG::ALL);
	int Delete(std::string file_name);
};

/**
* @class TKADCCONTROL_DL850
* @blief DL850ADCコントロールクラス
*
*	DL850をコントロールするためのクラスです。
*/
class TKADCCONTROL_DL850 : public TKADCCONTROL {
public:

	//The condition flag is depending on the ADC model.
	//Details are described in the YOKOGAWA IMDL850E-17 document.

	enum class CONDITIONFLAG {
		ALL = 0xffff,	//16bit
		CAP = 1 << 0,	//Capture
		REC = 1 << 1,	//Record
		TRG = 1 << 2,	//Awaiting trigger
		TRGINV = ALL - TRG,
		CAL = 1 << 3,	//Calibration
		TST = 1 << 4,	//Testing
		PRN = 1 << 5,	//Printing
		ACS = 1 << 6,	//Accessing
		MES = 1 << 7,	//Measuring
		HST = 1 << 8,	//History Search
		CNT = 1 << 9,	//CONection
		NGO = 1 << 10,	//Go/No-Go
		SCH = 1 << 11,	//Search
		RUN = 1 << 12,	//Running
		KLK = 1 << 13,	//Key lock
		AN = 1 << 14	//Analysis
	};

public:
	TKADCCONTROL_DL850();
	int GetStatusCondition(TKADCCONTROL_DL850::CONDITIONFLAG flag = CONDITIONFLAG::ALL);
	int Delete(std::string file_name);
};

#endif
