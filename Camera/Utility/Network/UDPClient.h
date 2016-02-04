/*************************************************************************
	UDPClientクラス
	Date: 2014/06/18 by 佐藤俊樹@UECISMS
	使い方: 
		作成後、send()関数で送信する。
		send()関数の第1引数は送信バッファ、第2引数は送信するバイト数である。
 *************************************************************************/
#ifndef __UDPCLIENT_H__
#define __UDPCLIENT_H__

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

class UDPClient{

public:
	
	/* コンストラクタ/デストラクタ */
	UDPClient();
	~UDPClient();

	/* 初期化 */
	int initialize(char *ipAddress, unsigned int portNo);

	/* 解放 */
	void finalize();

	/* 送信 */
	int send(void *sendBuffer, unsigned int size);

private:

	SOCKET sock;
	struct sockaddr_in sockAddr;
	int isInitialized;

};

#endif