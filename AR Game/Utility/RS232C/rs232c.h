/****************************************************************
		RS232C通信クラス	rs232c.h
		このクラスは下記のページを参考にして作成しました :			http://7ujm.net/C++/Rs232c.h.html
		Create:		2008/06/22	by	電通大IS研究科小池研究室 佐藤俊樹
		Updated:	2013/07/21	by	佐藤俊樹@UECISMS
			送受信バッファサイズを変更できるようにした(これに伴い、コンストラクタの引数の順番・個数が変わった)。
			受信時に指定受信サイズより少ないサイズでしか受信していなかったので
			タイムアウト設定のReadIntervalTimeoutを0Xffffffffから0に変更した。
 ****************************************************************/

#ifndef _RS232C_H_
#define _RS232C_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

class RS232c{

public:

	/* コンストラクタ */
	RS232c();

	/* デストラクタ */
	~RS232c();

	/* 初期化 */
	int connect(const char* portName,							// ポート名を指定します　COM1 COM2など、初期値はCOM1
		int baudRate = 9600,												// ボーレートを指定します。初期値は 9600
		DWORD sendBufferSize = 1024,								// 送信バッファサイズ
		DWORD receiveBufferSize = 1024,							// 受信バッファサイズ
		int readTimeOut = 5000,											// 受信時のタイムアウト　msで指定　初期値は　5000ms
		int writeTimeOut = 20000,										// 送信時のタイムアウト　msで指定　初期値は　20000ms
		int byteSize = 8,														// 1バイトのビット数を指定します。初期値は 8
		int parity = NOPARITY,											// パリティを指定します。パリティなし: NOPARITY
																								// 偶数パリティ: EVENPARITY
																								// 奇数パリティ: ODDPARITY
																								// 初期値は、パリティなし: NOPARITY
		int stopBits = ONESTOPBIT,										// ストップビット数を指定します。
																								// 1ビット: ONESTOPBIT
																								// 1.5ビット: ONE5STOPBITS
																								// 2ビット: TWOSTOPBITS 
																								// 初期値は　1ビット: ONESTOPBIT
		int rts = RTS_CONTROL_DISABLE,							// RTSをON=RTS_CONTROL_ENABLE 初期値は無効です
		int dtr = DTR_CONTROL_DISABLE							// DTRをON=DTR_CONTROL_ENABLE 初期値は無効です
	);

	/* 切断 */
	void disconnect();

	/* 文字列として受信。受信したサイズを返す */
	DWORD receiveWords(char* receiveBuffer, DWORD size);

	/* 指定されたサイズ分受信。受信したサイズを返す */
	DWORD receive(void* receiveBuffer, DWORD size);

	/* 文字列の送信。送信したサイズを返す */
	DWORD sendWords(const char* sendBuffer);

	/* 指定されたバッファをサイズ分だけ送信。送信したサイズを返す */
	DWORD send(void* sendBuffer, DWORD size);

	/* RS232Cの送受信バッファをクリア */
	void clearBuffer();

	/* 接続しているかどうか */
	int isLinked();

private:

	int isConnected;

	HANDLE m_hComm;
	DWORD size;
	DCB	m_Dcb;

};

#endif // _RS232C_H_



