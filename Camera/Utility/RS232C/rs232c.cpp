#include "rs232c.h"

#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>

int RS232c::connect( const char* portName, int baudRate, DWORD sendBufferSize, DWORD receiveBufferSize, int readTimeOut, int writeTimeOut, int byteSize, int parity, int stopBits, int rts, int dtr ){

 if ( !isConnected ){

		m_hComm = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

		if( m_hComm == INVALID_HANDLE_VALUE ){
			printf("�ڑ��Ɏ��s���܂���: %u\n", GetLastError());
			return -1;
		}

		/*�@�C�x���g���g�p���Ȃ��悤�ɃZ�b�g���� */
		SetCommMask( m_hComm, 0 );

		/*�@���o�̓o�b�t�@�̋��e�ʐݒ� */
		SetupComm( m_hComm, sendBufferSize, receiveBufferSize );

		/*�@�^�C���A�E�g���̃Z�b�g */
		COMMTIMEOUTS	m_CommTime;
		//m_CommTime.ReadIntervalTimeout = 0;
		//m_CommTime.ReadIntervalTimeout = 0xFFFFFFFF;						// �Ⴆ�w��T�C�Y��菭�Ȃ��Ă���M�ł��邾����M���Ă����߂�
		m_CommTime.ReadIntervalTimeout = 100;						// �Ⴆ�w��T�C�Y��菭�Ȃ��Ă���M�ł��邾����M���Ă����߂�
		m_CommTime.ReadTotalTimeoutMultiplier = 0;
		m_CommTime.ReadTotalTimeoutConstant = readTimeOut;
		m_CommTime.WriteTotalTimeoutMultiplier = 0;
		m_CommTime.WriteTotalTimeoutConstant = writeTimeOut;
                                               
		/*�@�^�C���A�E�g�̐ݒ� */
		SetCommTimeouts( m_hComm, &m_CommTime);

		/*�@�ʐM�f�o�C�X���̎擾 */
		GetCommState( m_hComm, &m_Dcb );

		/*�@�ʐM�f�o�C�X���̏C�� */
		m_Dcb.DCBlength = sizeof( DCB );
		m_Dcb.fBinary = FALSE;
		m_Dcb.BaudRate = baudRate;
		m_Dcb.ByteSize = byteSize;
		m_Dcb.Parity = parity;
		m_Dcb.StopBits = stopBits;
		m_Dcb.fRtsControl = rts;
		m_Dcb.fDtrControl = dtr;
		m_Dcb.fDsrSensitivity = FALSE;
		m_Dcb.fAbortOnError = FALSE;
		m_Dcb.fNull = FALSE;
		m_Dcb.fParity = NOPARITY;
		m_Dcb.ErrorChar = 0x00;
		m_Dcb.fErrorChar = FALSE;


		SetCommState( m_hComm, &m_Dcb ) ;
		if( !SetCommState( m_hComm, &m_Dcb ) ){
			printf("%s�ւ̐ڑ��Ɏ��s���܂���: %u\n", portName, GetLastError());
			return FALSE;
		}

		isConnected = 1;
		printf("%s��%dbps�Őڑ����܂����B\n", portName, baudRate);

		return 0;

 }else{

	 disconnect();
	 return connect(portName, baudRate, byteSize, parity, stopBits, rts, dtr, readTimeOut, writeTimeOut);
 }
} // connect

/* �ؒf */
void RS232c::disconnect(){
	if ( isConnected ){
		CloseHandle(m_hComm);				//�|�[�g����܂�
		isConnected = 0;
		printf("�ؒf���܂����B\n");
	}
} // disconnect

/* ������Ƃ��Ď�M�B��M�����T�C�Y��Ԃ� */
DWORD RS232c::receiveWords(char* receiveBuffer, DWORD size){

	if( ReadFile(m_hComm, receiveBuffer, size, &this->size, NULL) == 0 ){

		printf("��M�Ɏ��s���܂���: %u\n", GetLastError());
		return 0;

	}else{
		receiveBuffer[size] = '\0';
		return this->size;
	}
} // receiveWords

/* �w�肳�ꂽ�T�C�Y����M�B��M�����T�C�Y��Ԃ� */
DWORD RS232c::receive(void* receiveBuffer, DWORD size){

	if( ReadFile(m_hComm, receiveBuffer, size, &this->size, NULL) == 0 ){

		printf("��M�Ɏ��s���܂���: %u\n", GetLastError());
		return 0;

	}else{
		return this->size;
	}
}

/* NULL�I�[������̑��M�B���M�����T�C�Y��Ԃ� */
DWORD RS232c::sendWords(const char* word){

	if( WriteFile( m_hComm, word, (DWORD)strlen(word), &size, NULL ) == 0 ){

		printf("���M�Ɏ��s���܂���: %u\n", GetLastError());
		return 0;

	}else{
		return size;
	}
} // sendWords

/* �w�肳�ꂽ�o�b�t�@���T�C�Y���������M�B���M�����T�C�Y��Ԃ� */
DWORD RS232c::send(void* sendBuffer, DWORD size){

	if( WriteFile( m_hComm, sendBuffer, size, &this->size, NULL ) == 0 ){

		printf("���M�Ɏ��s���܂���: %u\n", GetLastError());
		return 0;

	}else{
		return this->size;
	}
} // send

/* RS232C�̑���M�o�b�t�@���N���A */
void RS232c::clearBuffer(){

	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

} // clearBuffer

/* �ڑ����Ă��邩�ǂ��� */
int RS232c::isLinked(){

	if( m_Dcb.fDtrControl == DTR_CONTROL_ENABLE ){									// DTR���L���̏ꍇ�̂ݗL��

		DWORD   dwSts;
		BOOL    fSuccess = GetCommModemStatus( m_hComm, &dwSts );			// �M���X�e�[�^�X�̎擾

		if( !fSuccess ){

		}else{

			if(!(dwSts & MS_DSR_ON)){
				return 0;								// DSR�M�� ON
			}

			//if(dwSts & MS_CTS_ON)				//CTSON
			//if(dwSts & MS_RING_ON)			//RINGON

			if(!(dwSts & MS_RLSD_ON)){
				return 0;								//RLSDON
			}
		}
	}

	return 1;
} // isLinked

/* �R���X�g���N�^ */
RS232c::RS232c(){
	isConnected = 0;
} // �R���X�g���N�^
 
/* �f�X�g���N�^ */
RS232c::~RS232c(){
} // �f�X�g���N�^
