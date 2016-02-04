#include "SocketUDP.h"
	
/* �R���X�g���N�^/�f�X�g���N�^ */
SocketUDP::SocketUDP(){	} // UDP
SocketUDP::~SocketUDP(){	} // ~UDP

/* WinSock������ */
int SocketUDP::initializeSocket(){
	WSADATA wsaData;
	int errorNo;
	if (( errorNo = WSAStartup(MAKEWORD(2,0), &wsaData) ) != 0){
		switch (errorNo) {
			case WSASYSNOTREADY:
				printf("WSASYSNOTREADY\n");
				break;
			case WSAVERNOTSUPPORTED:
				printf("WSAVERNOTSUPPORTED\n");
				break;
			case WSAEINPROGRESS:
				printf("WSAEINPROGRESS\n");
				break;
			case WSAEPROCLIM:
				printf("WSAEPROCLIM\n");
				break;
			case WSAEFAULT:
				printf("WSAEFAULT\n");
				break;
		} // switch
		return -1;
	}
	return 0;
} // initializeSocket

/* WinSock��� */
void SocketUDP::finalizeSocket(){
	WSACleanup();
} // finalizeSocket

/* Server�쐬 */
UDPServer *SocketUDP::createServer(unsigned int portNo){
	UDPServer *udpServer = new UDPServer();
	if ( udpServer->initialize(portNo) < 0 ){
		return NULL;
	}
	return udpServer;
} // createServer

/* Client�쐬 */
UDPClient *SocketUDP::createClient(char *ipAddress, unsigned int portNo){
	UDPClient *udpClient = new UDPClient();
	if ( udpClient->initialize(ipAddress, portNo) < 0 ){
		return NULL;
	}
	return udpClient;
} // createClient
