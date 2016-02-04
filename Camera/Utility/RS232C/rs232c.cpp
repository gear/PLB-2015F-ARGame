#include "rs232c.h"

#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

int RS232c::connect( const char* portName, int baudRate, DWORD sendBufferSize, DWORD receiveBufferSize, int readTimeOut, int writeTimeOut, int byteSize, int parity, int stopBits, int rts, int dtr ){

 if ( !isConnected ){

		m_hComm = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if( m_hComm == INVALID_HANDLE_VALUE ){
			printf("接続に失敗しました: %u\n", GetLastError());
			return -1;
		}

		/*　イベントを使用しないようにセットする */
		SetCommMask( m_hComm, 0 );

		/*　入出力バッファの許容量設定 */
		SetupComm( m_hComm, sendBufferSize, receiveBufferSize );

		/*　タイムアウト情報のセット */
		COMMTIMEOUTS	m_CommTime;
		//m_CommTime.ReadIntervalTimeout = 0;
		//m_CommTime.ReadIntervalTimeout = 0xFFFFFFFF;						// 例え指定サイズより少なくても受信できるだけ受信してすぐ戻る
		m_CommTime.ReadIntervalTimeout = 100;						// 例え指定サイズより少なくても受信できるだけ受信してすぐ戻る
		m_CommTime.ReadTotalTimeoutMultiplier = 0;
		m_CommTime.ReadTotalTimeoutConstant = readTimeOut;
		m_CommTime.WriteTotalTimeoutMultiplier = 0;
		m_CommTime.WriteTotalTimeoutConstant = writeTimeOut;
                                               
		/*　タイムアウトの設定 */
		SetCommTimeouts( m_hComm, &m_CommTime);

		/*　通信デバイス情報の取得 */
		GetCommState( m_hComm, &m_Dcb );

		/*　通信デバイス情報の修正 */
		m_Dcb.DCBlength = sizeof( DCB );
		m_Dcb.fBinary = FALSE;
		m_Dcb.BaudRate = baudRate;
		m_Dcb.ByteSize = byteSize;
		m_Dcb.Parity = parity;
		m_Dcb.StopBits = stopBits;
		m_Dcb.fRtsControl = rts;
		m_Dcb.fDtrControl = dtr;
		m_Dcb.fDsrSensitivity = FALSE;
		m_Dcb.fAbortOnError = FALSE;
		m_Dcb.fNull = FALSE;
		m_Dcb.fParity = NOPARITY;
		m_Dcb.ErrorChar = 0x00;
		m_Dcb.fErrorChar = FALSE;


		SetCommState( m_hComm, &m_Dcb ) ;
		if( !SetCommState( m_hComm, &m_Dcb ) ){
			printf("%sへの接続に失敗しました: %u\n", portName, GetLastError());
			return FALSE;
		}

		isConnected = 1;
		printf("%sに%dbpsで接続しました。\n", portName, baudRate);

		return 0;

 }else{

	 disconnect();
	 return connect(portName, baudRate, byteSize, parity, stopBits, rts, dtr, readTimeOut, writeTimeOut);
 }
} // connect

/* 切断 */
void RS232c::disconnect(){
	if ( isConnected ){
		CloseHandle(m_hComm);				//ポートを閉じます
		isConnected = 0;
		printf("切断しました。\n");
	}
} // disconnect

/* 文字列として受信。受信したサイズを返す */
DWORD RS232c::receiveWords(char* receiveBuffer, DWORD size){

	if( ReadFile(m_hComm, receiveBuffer, size, &this->size, NULL) == 0 ){

		printf("受信に失敗しました: %u\n", GetLastError());
		return 0;

	}else{
		receiveBuffer[size] = '\0';
		return this->size;
	}
} // receiveWords

/* 指定されたサイズ分受信。受信したサイズを返す */
DWORD RS232c::receive(void* receiveBuffer, DWORD size){

	if( ReadFile(m_hComm, receiveBuffer, size, &this->size, NULL) == 0 ){

		printf("受信に失敗しました: %u\n", GetLastError());
		return 0;

	}else{
		return this->size;
	}
}

/* NULL終端文字列の送信。送信したサイズを返す */
DWORD RS232c::sendWords(const char* word){

	if( WriteFile( m_hComm, word, (DWORD)strlen(word), &size, NULL ) == 0 ){

		printf("送信に失敗しました: %u\n", GetLastError());
		return 0;

	}else{
		return size;
	}
} // sendWords

/* 指定されたバッファをサイズ分だけ送信。送信したサイズを返す */
DWORD RS232c::send(void* sendBuffer, DWORD size){

	if( WriteFile( m_hComm, sendBuffer, size, &this->size, NULL ) == 0 ){

		printf("送信に失敗しました: %u\n", GetLastError());
		return 0;

	}else{
		return this->size;
	}
} // send

/* RS232Cの送受信バッファをクリア */
void RS232c::clearBuffer(){

	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

} // clearBuffer

/* 接続しているかどうか */
int RS232c::isLinked(){

	if( m_Dcb.fDtrControl == DTR_CONTROL_ENABLE ){									// DTRが有効の場合のみ有効

		DWORD   dwSts;
		BOOL    fSuccess = GetCommModemStatus( m_hComm, &dwSts );			// 信号ステータスの取得

		if( !fSuccess ){

		}else{

			if(!(dwSts & MS_DSR_ON)){
				return 0;								// DSR信号 ON
			}

			//if(dwSts & MS_CTS_ON)				//CTSON
			//if(dwSts & MS_RING_ON)			//RINGON

			if(!(dwSts & MS_RLSD_ON)){
				return 0;								//RLSDON
			}
		}
	}

	return 1;
} // isLinked

/* コンストラクタ */
RS232c::RS232c(){
	isConnected = 0;
} // コンストラクタ
 
/* デストラクタ */
RS232c::~RS232c(){
} // デストラクタ
