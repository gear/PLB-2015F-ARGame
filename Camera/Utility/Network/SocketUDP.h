/*************************************************************************
	WinSock2��p����UDP�ʐM�N���X(UDPClient/UDPServer�N���X�ƃZ�b�g�ŗp����)
	Date: 2014/06/18 by �����r��@UECISMS
	�g����:
		WinSock�����������邽�߂ɁA1�xinitializeSocket()���ĂԕK�v������B
		���̌�AcreateClient()�y��createServerClient()���g����
		Client���Ȃ�UDPClient�AServer���Ȃ�UDPServer�N���X�̃C���X�^���X���쐬���A
		���ꂼ���send()/receive()�֐��ő���M���s���B

	���C�u������OS�ˑ����ɂ���:
		���̃N���X�ł�Socket�ʐM��Windows��WinSock2���g���Ă���̂ŁA
		Windows.h(WinSock2.h)��include��ws2_32.lib�̒ǉ����s���Ă���B
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
	
	/* �R���X�g���N�^/�f�X�g���N�^ */
	SocketUDP();
	~SocketUDP();

	/* ������ */
	static int initializeSocket();

	/* ��� */
	static void finalizeSocket();

	/* Server�쐬 */
	static UDPServer *createServer(unsigned int portNo);

	/* Client�쐬 */
	static UDPClient *createClient(char *ipAddress, unsigned int portNo);

};

#endif