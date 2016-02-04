#include "UDPServer.h"
	
/* コンストラクタ/デストラクタ */
UDPServer::UDPServer(){
	isInitialized = 0;
} // UDPServer

UDPServer::~UDPServer(){
	finalize();
} // ~UDPServer

/* 初期化 */
int UDPServer::initialize(unsigned int portNo){
	sock = socket(AF_INET, SOCK_DGRAM, 0);		// UDPとして初期化
	if (sock == INVALID_SOCKET) {
		printf("Socket Error: %d\n", WSAGetLastError());
		return -1;
	}
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(portNo);
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	/* Bind */
	if ( bind(sock, (struct sockaddr *)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR ){
		printf("Socket Error: %d\n", WSAGetLastError());
		return -1;
	}

	/* ブロッキングしない */
	u_long val=1;
	ioctlsocket(sock, FIONBIO, &val);

	isInitialized = 1;
	return 0;
} // initialize

/* 解放 */
void UDPServer::finalize(){
	if (isInitialized){
		closesocket(sock);
		isInitialized = 0;
	}
} // finalize

/* 受信 */
int UDPServer::receive(void *receiveBuffer, unsigned int size){
	return recv(sock, (char*)receiveBuffer, size, 0);
} // receive
