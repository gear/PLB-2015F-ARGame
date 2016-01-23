/****************************************************************
		RS232C�ʐM�N���X	rs232c.h
		���̃N���X�͉��L�̃y�[�W���Q�l�ɂ��č쐬���܂��� :			http://7ujm.net/C++/Rs232c.h.html
		Create:		2008/06/22	by	�d�ʑ�IS�����ȏ��r������ �����r��
		Updated:	2013/07/21	by	�����r��@UECISMS
			����M�o�b�t�@�T�C�Y��ύX�ł���悤�ɂ���(����ɔ����A�R���X�g���N�^�̈����̏��ԁE�����ς����)�B
			��M���Ɏw���M�T�C�Y��菭�Ȃ��T�C�Y�ł�����M���Ă��Ȃ������̂�
			�^�C���A�E�g�ݒ��ReadIntervalTimeout��0Xffffffff����0�ɕύX�����B
 ****************************************************************/

#ifndef _RS232C_H_
#define _RS232C_H_

#if _MSC_VER > 1000
#pragma once
#endif

#include <windows.h>

class RS232c{

public:

	/* �R���X�g���N�^ */
	RS232c();

	/* �f�X�g���N�^ */
	~RS232c();

	/* ������ */
	int connect(const char* portName,							// �|�[�g�����w�肵�܂��@COM1 COM2�ȂǁA�����l��COM1
		int baudRate = 9600,												// �{�[���[�g���w�肵�܂��B�����l�� 9600
		DWORD sendBufferSize = 1024,								// ���M�o�b�t�@�T�C�Y
		DWORD receiveBufferSize = 1024,							// ��M�o�b�t�@�T�C�Y
		int readTimeOut = 5000,											// ��M���̃^�C���A�E�g�@ms�Ŏw��@�����l�́@5000ms
		int writeTimeOut = 20000,										// ���M���̃^�C���A�E�g�@ms�Ŏw��@�����l�́@20000ms
		int byteSize = 8,														// 1�o�C�g�̃r�b�g�����w�肵�܂��B�����l�� 8
		int parity = NOPARITY,											// �p���e�B���w�肵�܂��B�p���e�B�Ȃ�: NOPARITY
																								// �����p���e�B: EVENPARITY
																								// ��p���e�B: ODDPARITY
																								// �����l�́A�p���e�B�Ȃ�: NOPARITY
		int stopBits = ONESTOPBIT,										// �X�g�b�v�r�b�g�����w�肵�܂��B
																								// 1�r�b�g: ONESTOPBIT
																								// 1.5�r�b�g: ONE5STOPBITS
																								// 2�r�b�g: TWOSTOPBITS 
																								// �����l�́@1�r�b�g: ONESTOPBIT
		int rts = RTS_CONTROL_DISABLE,							// RTS��ON=RTS_CONTROL_ENABLE �����l�͖����ł�
		int dtr = DTR_CONTROL_DISABLE							// DTR��ON=DTR_CONTROL_ENABLE �����l�͖����ł�
	);

	/* �ؒf */
	void disconnect();

	/* ������Ƃ��Ď�M�B��M�����T�C�Y��Ԃ� */
	DWORD receiveWords(char* receiveBuffer, DWORD size);

	/* �w�肳�ꂽ�T�C�Y����M�B��M�����T�C�Y��Ԃ� */
	DWORD receive(void* receiveBuffer, DWORD size);

	/* ������̑��M�B���M�����T�C�Y��Ԃ� */
	DWORD sendWords(const char* sendBuffer);

	/* �w�肳�ꂽ�o�b�t�@���T�C�Y���������M�B���M�����T�C�Y��Ԃ� */
	DWORD send(void* sendBuffer, DWORD size);

	/* RS232C�̑���M�o�b�t�@���N���A */
	void clearBuffer();

	/* �ڑ����Ă��邩�ǂ��� */
	int isLinked();

private:

	int isConnected;

	HANDLE m_hComm;
	DWORD size;
	DCB	m_Dcb;

};

#endif // _RS232C_H_



