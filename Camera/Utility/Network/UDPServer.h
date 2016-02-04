/*************************************************************************
	UDPServerクラス
	Date: 2014/06/18 by 佐藤俊樹@UECISMS
	使い方: 
		作成後、receive()関数で受信する。
		receive()関数の第1引数は受信バッファ、第2引数は受信するバイト数である。
 *************************************************************************/
#ifndef __UDPSERVER_H__
#define __UDPSERVER_H__

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

class UDPServer{

public:
	
	/* コンストラクタ/デストラクタ */
	UDPServer();
	~UDPServer();

	/* 初期化 */
	int initialize(unsigned int portNo);

	/* 解放 */
	void finalize();

	/* 受信 */
	int receive(void *receiveBuffer, unsigned int size);

private:

	SOCKET sock;
	struct sockaddr_in sockAddr;
	int isInitialized;

};

#endif