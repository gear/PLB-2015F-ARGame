/*************************************************************************
	WinSock2を用いたUDP通信クラス(UDPClient/UDPServerクラスとセットで用いる)
	Date: 2014/06/18 by 佐藤俊樹@UECISMS
	使い方:
		WinSockを初期化するために、1度initializeSocket()を呼ぶ必要がある。
		その後、createClient()及びcreateServerClient()を使って
		Client側ならUDPClient、Server側ならUDPServerクラスのインスタンスを作成し、
		それぞれのsend()/receive()関数で送受信を行う。

	ライブラリのOS依存性について:
		このクラスではSocket通信にWindowsのWinSock2を使っているので、
		Windows.h(WinSock2.h)のincludeとws2_32.libの追加を行っている。
 *************************************************************************/
#ifndef __SOCKETUDP_H__
#define __SOCKETUDP_H__

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

#include "UDPServer.h"
#include "UDPClient.h"

class SocketUDP{

public:
	
	/* コンストラクタ/デストラクタ */
	SocketUDP();
	~SocketUDP();

	/* 初期化 */
	static int initializeSocket();

	/* 解放 */
	static void finalizeSocket();

	/* Server作成 */
	static UDPServer *createServer(unsigned int portNo);

	/* Client作成 */
	static UDPClient *createClient(char *ipAddress, unsigned int portNo);

};

#endif