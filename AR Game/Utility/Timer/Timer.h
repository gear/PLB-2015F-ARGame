/*********************************************************************
	Timer�N���X			�uTimer.h�v

	�T�v:
		SDL���g����1msec�P�ʂ̃^�C�}�ƁACPU�̃N���b�N�J�E���^���g����������\�^�C�}�B

	Date:
		Created: 2009/01/16	By �����r��
		Updated: 2010/07/10	By �����r��	�֐����𕪂���₷������
		Updated: 2013/03/07	By �����r��	x64�p��RDTSC�^�C�}�[������(�C�����C���A�Z���u���p�~)
 *********************************************************************/
#ifndef __Timer_H__
#define __Timer_H__

#pragma warning(disable: 4293)		// �V�t�g�����ł������I�Ȍx���𖳎�

#include <SDL.h>

#ifndef STRICT
  #define STRICT
#endif

#ifndef WIN32_LEAN_AND_MEAN
  #define WIN32_LEAN_AND_MEAN
#endif

#ifndef _WINDOWS_
  #include <windows.h>
#endif

#ifndef _INC_MMSYSTEM
  #include <mmsystem.h>
  #pragma comment(lib,"winmm.lib")
#endif

#include <intrin.h>

class Timer{

public:

	/* ���ݎ����ƑO�̃t���[���̎��� */
	Uint32 systemTime, prevTime;

	/* 1�X�e�b�v�̒���(msec) */
	float dt;

	/* �^�C�}�[�������� */
	void initialize(void);

	/******************************************
								Singleton���\�b�h
	 ******************************************/
	/* Singleton�C���X�^���X���� */
	static Timer *getInstance();

	/* Singleton�C���X�^���X�j�� */
	static void releaseInstance();

	/***************************************************
												�C���^�t�F�[�X
	 ***************************************************/
	/* ���Ԃ��Z�b�g����֐��B
		 �V�[���N���X���疈���[�v�Ă΂��K�v������ */
	void update(void);

	/******************************************************************
								RDTSC���߂�p�����^�C�}�ɂ��Ă̊֐�
	 ******************************************************************/
	/* RDTSC���߂�CPU�̃N���b�N�J�E���^���W�X�^�̒l��ǂݏo�� */
	ULONGLONG getRDTSCClockCount();

	/* �N���b�N�J�E���g��msec�ɕϊ����� */
	double getRDTSCMSeconds( ULONGLONG clockCount );

private:

	ULONGLONG cpuClock;			// CPU�N���b�N���g���l

	/***************************
					Timer�C���X�^���X
	 ***************************/
	static Timer *timInstance;
	static int timInitialized;		// ����������Ă��邩���Ȃ���(1: ���łɏ���������Ă���, 0: ����������Ă��Ȃ�)

	/***************************************************
							�R���X�g���N�^ / �f�X�g���N�^
	 ***************************************************/
	/* �R���X�g���N�^ */
	Timer();

	/* �f�X�g���N�^ */
	virtual ~Timer();

	/******************************************************************
						RDTSC���߂�p�����^�C�}�����ɂ��Ă̕⏕�֐�
	 ******************************************************************/
	/* �v���C�I���e�B��ݒ肷�� */
	void hiPriority( BOOL enable );

	/* CPU�̓���N���b�N�̑���(1000msec������) */
	void calcCPUClock();

};
#endif