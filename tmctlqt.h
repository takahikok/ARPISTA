/*STARTM------------------------------------------------------------------
+       Module Name     : tmctl.h                                        +
+       Abstract        : Yokogawa T&M Remote Control functions.         +
+       Revision History:                                                +
+       Rev.    Date.          Notes.                                    +
+       --------------------------------------------------------------   +
+       2.0		20070309	USBTMC2 VXI11 supported. thread safe support +
+       2.1		20080508	RS232C expand                                +
+       2.2		20080704	Add TmcGetInitializeError.                   +
+       2.3		20121121	coped 64 bits                                +
+       3.0		20150820	add VISA USB control                         +
-ENDM-------------------------------------------------------------------*/

#ifndef	_TMCTL_H
#define	_TMCTL_H	1

#include <windows.h>

/* Control setting */
#define	TM_NO_CONTROL	0
#define	TM_CTL_GPIB		1
#define	TM_CTL_RS232	2
#define	TM_CTL_USB		3
#define	TM_CTL_ETHER	4
#define	TM_CTL_USBTMC	5
#define	TM_CTL_ETHERUDP	6
#define	TM_CTL_USBTMC2	7				// 2007/01/19 add
#define	TM_CTL_VXI11	8				// 2007/01/19 add
#define	TM_CTL_USB2		9
#define	TM_CTL_VISAUSB	10

/* GPIB */

/* RS232 */
#define	TM_RS_1200		'0'
#define	TM_RS_2400		'1'
#define	TM_RS_4800		'2'
#define	TM_RS_9600		'3'
#define	TM_RS_19200		'4'
#define	TM_RS_38400		'5'
#define	TM_RS_57600		'6'
#define	TM_RS_115200	'7'			// Ver2.1

#define	TM_RS_8N1		'0'
#define	TM_RS_7E		'1'
#define	TM_RS_7O		'2'
#define	TM_RS_8O		'3'			// Ver2.1 8bit,odd,onestopbit
#define	TM_RS_7N5		'4'			// Ver2.1 7bit,noparity,one5stopbit
#define	TM_RS_8N2		'5'

#define	TM_RS_NO		'0'
#define	TM_RS_XON		'1'
#define	TM_RS_HARD		'2'

/* USB */
#define TM_USB_CHECK_OK				0
#define TM_USB_CHECK_NOTOPEN		1
#define TM_USB_CHECK_NODEVICE		2

#define	TM_USB_READPIPE				0
#define	TM_USB_WRITEPIPE			1
#define	TM_USB_STATUSPIPE			2

/* Error Number */
#define	TMCTL_NO_ERROR				0x00000000		/* No error */
#define	TMCTL_TIMEOUT				0x00000001		/* Timeout */
#define	TMCTL_NO_DEVICE				0x00000002		/* Device Not Found */
#define	TMCTL_FAIL_OPEN				0x00000004		/* Open Port Error */
#define	TMCTL_NOT_OPEN				0x00000008		/* Device Not Open */
#define	TMCTL_DEVICE_ALREADY_OPEN	0x00000010		/* Device Already Open */
#define	TMCTL_NOT_CONTROL			0x00000020		/* Controller Not Found */
#define	TMCTL_ILLEGAL_PARAMETER		0x00000040		/* Parameter is illegal */
#define	TMCTL_SEND_ERROR			0x00000100		/* Send Error */
#define	TMCTL_RECV_ERROR			0x00000200		/* Receive Error */
#define	TMCTL_NOT_BLOCK				0x00000400		/* Data is not Block Data */
#define	TMCTL_SYSTEM_ERROR			0x00001000		/* System Error */
#define	TMCTL_ILLEGAL_ID			0x00002000		/* Device ID is Illegal */
#define	TMCTL_NOT_SUPPORTED			0x00004000		/* this feature not supportred */
#define	TMCTL_INSUFFICIENT_BUFFER	0x00008000		/* unsufficient buffer size */
#define	TMCTL_LIBRARY_ERROR			0x00010000		/* Dll Not Found */

/* L2 Error Number */
#define	TMCTL2_NO_ERROR				(00000)			/* No error */
#define	TMCTL2_TIMEOUT				(10001)			/* Timeout */
#define	TMCTL2_NO_DEVICE			(10002)			/* Device Not Found */
#define	TMCTL2_FAIL_OPEN			(10003)			/* Open Port Error */
#define	TMCTL2_NOT_OPEN				(10004)			/* Device Not Open */
#define	TMCTL2_DEVICE_ALREADY_OPEN	(10005)			/* Device Already Open */
#define	TMCTL2_NOT_CONTROL			(10006)			/* Controller Not Found */
#define	TMCTL2_ILLEGAL_PARAMETER	(10007)			/* Parameter is illegal */
#define	TMCTL2_SEND_ERROR			(10008)			/* Send Error */
#define	TMCTL2_RECV_ERROR			(10009)			/* Receive Error */
#define	TMCTL2_NOT_BLOCK			(10010)			/* Data is not Block Data */
#define	TMCTL2_SYSTEM_ERROR			(10011)			/* System Error */
#define	TMCTL2_ILLEGAL_ID			(10012)			/* Device ID is Illegal */
#define	TMCTL2_NOT_SUPPORTED		(10013)			/* this feature not supportred */
#define	TMCTL2_INSUFFICIENT_BUFFER	(10014)			/* unsufficient buffer size */
#define	TMCTL2_LIBRARY_ERROR		(10015)			/* Dll Not Found */

#define	ADRMAXLEN			(64)

typedef	struct _Devicelist
{
	char	adr[ADRMAXLEN] ;
} DEVICELIST ;

typedef	struct _DevicelistEx
{
	char		adr[ADRMAXLEN] ;
unsigned short	vendorID ;
unsigned short	productID ;
	char		dummy[188] ;
} DEVICELISTEX ;

// type define for "TmcSetCallback"
typedef void(__stdcall *Hndlr)(int, UCHAR, ULONG, ULONG) ;

/* Functions */
#ifndef	_TMCTL_DEFINES

#include <QLibrary>
// アプリケーションと同じフォルダに SharedLib.dll があると仮定する。
//static QLibrary tmctlLib("tmctl"); // 読み込み

// 関数シンボルの定義。定義のフォーマットは以下のとおり。
//   typedef 戻り値の型 (*定義型)( 入力値の型1, 入力値の型2, ... );
// 今回、読み込む関数は int addNumber( int, int ) なので次のようになる。
//typedef int (*MyPrototype)(int,int);

// 関数シンボルのアドレスを取得する（失敗の場合は0）。
//MyPrototype myFunction = (MyPrototype) tmctlLib->resolve("addNumbers");

// 呼び出し関数を実行。
//int result = myFunction(3,2);
//qDebug() << result; // "5"

// アンロード。loadを呼び出してない場合、resolveでloadされているため。
//myLib.unload();

#ifdef	__cplusplus
extern	"C" {
#endif
typedef int (*pTmcInitialize)(int, const char*, int*);
typedef	int		__stdcall TmcSetIFC( int id, int tm ) ;
typedef	int		__stdcall TmcDeviceClear( int id ) ;
typedef	int		__stdcall TmcDeviceTrigger( int id ) ;
typedef int (*pTmcSend)( int , const char*  ) ;
typedef	int		__stdcall TmcSendByLength( int id, const char* msg, int len ) ;
typedef	int		__stdcall TmcSendSetup( int id ) ;
typedef	int		__stdcall TmcSendOnly( int id, const char* msg, int len, int end ) ;
typedef int (*pTmcReceive)( int , const char* , int , int*  ) ;
typedef	int		__stdcall TmcReceiveSetup( int id ) ;
typedef	int		__stdcall TmcReceiveOnly( int id, const char* buff, int blen, int* rlen ) ;
typedef	int		__stdcall TmcReceiveBlockHeader( int id, int* len ) ;
typedef	int		__stdcall TmcReceiveBlockData( int id, const char* buff, int blen, int* rlen, int* end ) ;
typedef	int		__stdcall TmcCheckEnd( int id ) ;
typedef	int		__stdcall TmcSetCmd( int id, const char* cmd ) ;
typedef	int		__stdcall TmcSetRen( int id, int flag ) ;
typedef	int (*pTmcGetLastError)( int) ;
typedef	int		__stdcall TmcGetDetailLastError( int id ) ;
typedef	int		__stdcall TmcCheckError( int id, int sts, const char* msg, const char* err ) ;
typedef	int		__stdcall TmcSetTerm( int id, int eos, int eot ) ;
typedef	int		__stdcall TmcSetEos( int id, unsigned char eos ) ;
typedef	int		__stdcall TmcSetTimeout( int id, int tmo ) ;
typedef	int		__stdcall TmcSetDma( int id, int flg ) ;
typedef	int		__stdcall TmcGetStatusByte( int id, unsigned const char* sts ) ;
typedef int (*pTmcFinish)( int  ) ;
typedef	int		__stdcall TmcSearchDevices(int wire, DEVICELIST* list, int max, int* num,const char* option) ;
typedef	int		__stdcall TmcSearchDevicesEx(int wire, DEVICELISTEX* list, int max, int* num,const char* option) ;
typedef	int		__stdcall TmcWaitSRQ(int id, const char* stsbyte, int tout) ;
typedef	int		__stdcall TmcAbortWaitSRQ(int id) ;
typedef	int		__stdcall TmcSetCallback(int id,Hndlr func, ULONG p1, ULONG p2) ;
typedef	int		__stdcall TmcResetCallback(int id) ;
typedef	int		__stdcall TmcSendTestData(int id, const char* msg, int len ) ;
typedef	int		__stdcall TmcReceiveTestData( int id, const char* buff, int blen, int* rlen ) ;
typedef	int		__stdcall TmcInitiateAbortBulkIn(int id, UCHAR tagNo) ;
typedef	int		__stdcall TmcInitiateAbortBulkOut(int id, UCHAR tagNo) ;
typedef	int		__stdcall TmcCheckAbortBulkInStatus(int id) ;
typedef	int		__stdcall TmcCheckAbortBulkOutStatus(int id) ;
typedef	int		__stdcall TmcEncodeSerialNumber(const char* encode,size_t len,const char* src) ;
typedef	int		__stdcall TmcDecodeSerialNumber(const char* decode,size_t len,const char* src) ;
typedef	int		__stdcall TmcGotoLocal( int id ) ;
typedef	int		__stdcall TmcLocalLockout(int id) ;
typedef  int		__stdcall TmcAbortPipe(int id,long pipeNo) ;
typedef  int		__stdcall TmcResetPipe(int id,long pipeNo) ;
typedef	int		__stdcall TmcWriteHeader(int id, int blen) ;
typedef	int		__stdcall TmcReceiveWithoutWriteHeader(int id, const char* buff, int blen, int* rlen, int* end ) ;
typedef	int		__stdcall TmcGetTagNo(int id, UCHAR* tag) ;
typedef	int		__stdcall TmcSendByLength2( int id, const char* msg, int msgSize, int len, CHAR eof) ;
typedef	int		__stdcall TmcDeviceChangeNotify(HWND hWnd, BOOL bStart) ;
typedef	int		__stdcall TmcCheckUSB(int id) ;
typedef	int		__stdcall TmcGetPipeNo(int id,int type,int* pipeNo) ;
typedef	int		__stdcall TmcCheckGUID(void *lParam) ;
typedef	ULONG	__stdcall TmcGetInitializeError() ;
typedef	int		__stdcall TmcGetWireInfo( int id, int* wire );

// L2 Functions
typedef	int	__stdcall xTmcSearchDevices(int wire, DEVICELIST* adrlist, int max, int *num,const char* option) ;
typedef	int	__stdcall xTmcSearchDevicesEx(int wire, DEVICELISTEX* adrlist, int max, int *num,const char* option) ;
typedef	int	__stdcall xTmcInitialize( int wire, const char* adr, int* id ) ;
typedef	int	__stdcall xTmcInitializeEx( int wire, const char* adr, int* id, int timeout ) ;
typedef	int	__stdcall xTmcFinish( int id ) ;
typedef	int	__stdcall xTmcSend( int id, const char* msg ) ;
typedef	int	__stdcall xTmcSendByLength( int id, const char* msg, int len ) ;
typedef	int	__stdcall xTmcSendSetup( int id ) ;
typedef	int	__stdcall xTmcSendOnly( int id, const char* msg, int len, int end ) ;
typedef	int	__stdcall xTmcReceive( int id, const char* buff, int blen, int* rlen ) ;
typedef	int	__stdcall xTmcReceiveSetup( int id ) ;
typedef	int	__stdcall xTmcReceiveOnly( int id, const char* buff, int blen, int* rlen ) ;
typedef	int	__stdcall xTmcReceiveBlockHeader( int id, int* length ) ;
typedef	int	__stdcall xTmcReceiveBlockData( int id, const char* buff, int blen, int* rlen, int* end ) ;
typedef	int	__stdcall xTmcCheckEnd( int id ) ;
typedef	int	__stdcall xTmcDeviceClear( int id ) ;
typedef	int	__stdcall xTmcSetRen( int id, int flag ) ;
typedef	int	__stdcall xTmcGetLastError( int id ) ;
typedef	int	__stdcall xTmcSetTerm( int id, int eos, int eot ) ;
typedef	int	__stdcall xTmcSetTimeout( int id, int tmo ) ;
typedef	int	__stdcall xTmcCreateSRQHandler( int id, UINT msgId, HWND hWnd) ;
typedef	int	__stdcall xTmcDleateSRQHandler( int id ) ;
typedef	int	__stdcall xTmcSetCallback(int id,Hndlr func, ULONG p1, ULONG p2) ;
typedef	int	__stdcall xTmcResetCallback(int id) ;
typedef	int	__stdcall xTmcSet( int id, char *msg) ;
typedef	int	__stdcall xTmcSetBinary( int id, const char* msg, char *buf, int size ) ;
typedef	int	__stdcall xTmcGet( int id, char *msg, const char* buf, int blen, int* rlen ) ;
typedef	int __stdcall xTmcGetBinary( int id, char *msg, const char* buf, int blen, int* rlen ) ;
typedef	int	__stdcall xTmcGetErrorQueue(int id) ;
typedef	int	__stdcall xTmcSetNE( int id, char *msg) ;
typedef	int	__stdcall xTmcSetBinaryNE( int id, const char* msg, char *buf, int size ) ;
typedef	int	__stdcall xTmcGetNE( int id, char *msg, const char* buf, int blen, int* rlen ) ;
typedef	int __stdcall xTmcGetBinaryNE( int id, char *msg, const char* buf, int blen, int* rlen ) ;
typedef	int	__stdcall xTmcSetNES( int id, char *msg) ;
typedef	int	__stdcall xTmcSetBinaryNES( int id, const char* msg, char *buf, int size ) ;
typedef	int	__stdcall xTmcLock( int id ) ;
typedef	int	__stdcall xTmcUnlock( int id ) ;
typedef	int	__stdcall xTmcGetWireInfo( int id, int* wire );

#ifdef	__cplusplus
}
#endif
#endif

class TmctlQt
{
public:
	TmctlQt();
	~TmctlQt();
	static int TmcInitialize( int wire, const char* adr, int* id );
	static int TmcSend( int id, const char* msg );
	static int TmcGetLastError( int id ) ;
	static int TmcReceive( int id, const char* buff, int blen, int* rlen ) ;
	static int TmcFinish( int id ) ;


private:
//	QLibrary* tmctlLib;

};
#endif
