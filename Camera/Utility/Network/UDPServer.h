/*************************************************************************
	UDPServer�N���X
	Date: 2014/06/18 by �����r��@UECISMS
	�g����: 
		�쐬��Areceive()�֐��Ŏ�M����B
		receive()�֐��̑�1�����͎�M�o�b�t�@�A��2�����͎�M����o�C�g���ł���B
 *************************************************************************/
#ifndef __UDPSERVER_H__
#define __UDPSERVER_H__

#include <stdlib.h>
#include <stdio.h>
#include <Windows.h>

class UDPServer{

public:
	
	/* �R���X�g���N�^/�f�X�g���N�^ */
	UDPServer();
	~UDPServer();

	/* ������ */
	int initialize(unsigned int portNo);

	/* ��� */
	void finalize();

	/* ��M */
	int receive(void *receiveBuffer, unsigned int size);

private:

	SOCKET sock;
	struct sockaddr_in sockAddr;
	int isInitialized;

};

#endif