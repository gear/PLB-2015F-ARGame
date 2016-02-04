/*************************************************************************
	UDPClient�N���X
	Date: 2014/06/18 by �����r��@UECISMS
	�g����: 
		�쐬��Asend()�֐��ő��M����B
		send()�֐��̑�1�����͑��M�o�b�t�@�A��2�����͑��M����o�C�g���ł���B
 *************************************************************************/
#ifndef __UDPCLIENT_H__
#define __UDPCLIENT_H__

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

class UDPClient{

public:
	
	/* �R���X�g���N�^/�f�X�g���N�^ */
	UDPClient();
	~UDPClient();

	/* ������ */
	int initialize(char *ipAddress, unsigned int portNo);

	/* ��� */
	void finalize();

	/* ���M */
	int send(void *sendBuffer, unsigned int size);

private:

	SOCKET sock;
	struct sockaddr_in sockAddr;
	int isInitialized;

};

#endif