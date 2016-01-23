#include "UDPClient.h"
	
/* �R���X�g���N�^/�f�X�g���N�^ */
UDPClient::UDPClient(){
	isInitialized = 0;
} // UDPServer

UDPClient::~UDPClient(){	
	finalize();
} // ~UDPServer

/* ������ */
int UDPClient::initialize(char *ipAddress, unsigned int portNo){
	sock = socket(AF_INET, SOCK_DGRAM, 0);		// UDP�Ƃ��ď�����
	if (sock == INVALID_SOCKET) {
		printf("Socket Error: %d\n", WSAGetLastError());
		return -1;
	}
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons((u_short)portNo);
	sockAddr.sin_addr.S_un.S_addr = inet_addr(ipAddress);		
	if (sockAddr.sin_addr.S_un.S_addr == 0xffffffff) {
		struct hostent *host;
		host = gethostbyname(ipAddress);
		if (host == NULL) {
			return 1;
		}
		sockAddr.sin_addr.S_un.S_addr =	*(unsigned int *)host->h_addr_list[0];
	}

	isInitialized = 1;
	return 0;
} // initialize

/* ��� */
void UDPClient::finalize(){
	if (isInitialized){
		closesocket(sock);
		isInitialized = 0;
	}
} // finalize

/* ���M */
int UDPClient::send(void *sendBuffer, unsigned int size){
	return sendto(sock, (char*)sendBuffer, size, 0, (struct sockaddr *)&sockAddr, sizeof(sockAddr));
} // send
