/*********************************************
				Timer�N���X			�uTimer.cpp�v
 **********************************************/

#include "Timer.h"

Timer *Timer::timInstance = NULL;

/******************************************
							Singleton���\�b�h
 ******************************************/
/* Singleton�C���X�^���X���� */
Timer *Timer::getInstance(){
	if ( timInstance ){
		return timInstance;
	}else{
		timInstance = new Timer();
		return timInstance;
	}
} // getInstance

/* Singleton�C���X�^���X�j�� */
void Timer::releaseInstance(){
	if ( timInstance ){
		delete timInstance;
		timInstance = NULL;
	}
} // releaseInstance

/***************************************************
											�C���^�t�F�[�X
 ***************************************************/
/* ���Ԃ��Z�b�g����֐��B
	 �V�[���N���X���疈���[�v�Ă΂��K�v������ */
void Timer::update(void){
	prevTime = systemTime;
	systemTime = SDL_GetTicks();
	dt = (float)(systemTime - prevTime) / 1000.0f;
	if (dt < 0.001f){
		dt = 0.001f;
	}
} // update

/******************************************************************
							RDTSC���߂�p�����^�C�}�ɂ��Ă̊֐�
 ******************************************************************/
/* RDTSC���߂�CPU�̃N���b�N�J�E���^���W�X�^�̒l��ǂݏo�� */
ULONGLONG Timer::getRDTSCClockCount(){
	return __rdtsc();
} // getRDTSCClockCount

/* �N���b�N�J�E���g��msec�ɕϊ����� */
double Timer::getRDTSCMSeconds( ULONGLONG clockCount ){
	return ( (double)clockCount / (double)cpuClock ) * 1000.0;	// msec�ɕϊ�
} // getRDTSCMSeconds

/***************************************************
						�R���X�g���N�^ / �f�X�g���N�^
 ***************************************************/
/* �R���X�g���N�^ */
Timer::Timer(){} // �R���X�g���N�^

/* �f�X�g���N�^ */
Timer::~Timer(){} // �f�X�g���N�^

/* �^�C�}�[�������� */
void Timer::initialize(void){
	prevTime = SDL_GetTicks();
	SDL_Delay(50);		// �����҂�
	systemTime = SDL_GetTicks();
	calcCPUClock();
} // initialize

/******************************************************************
					RDTSC���߂�p�����^�C�}�����ɂ��Ă̕⏕�֐�
 ******************************************************************/
/* �v���C�I���e�B��ݒ肷�� */
void Timer::hiPriority( BOOL enable ){

	static DWORD oldProcessClass;
	static DWORD oldThreadPriority;

	if( enable ){
		oldProcessClass   = GetPriorityClass( GetCurrentProcess() );
		oldThreadPriority = GetThreadPriority( GetCurrentThread() );
	} else {
		SetPriorityClass( GetCurrentProcess() , oldProcessClass );
		SetThreadPriority( GetCurrentThread() , oldThreadPriority );
	}
	Sleep(0);

	return;
} // hiPriority

/* CPU�̓���N���b�N�̑���(1000msec������) */
void Timer::calcCPUClock(){
	hiPriority( TRUE );
	ULONGLONG result1, result2;
	//SetThreadAffinityMask(GetCurrentThread(), 1);		// �X���b�h�Ŏg�p����CPU�R�A��1�Ɍ���
	{
		DWORD t = timeGetTime() + 100;
		while( timeGetTime() < t );
		t += 1000;
		result1 = __rdtsc();
		while( timeGetTime() < t );
		result2 = __rdtsc();
	}
	hiPriority( FALSE );
	cpuClock = result2 - result1;
	printf("RDTSC Timer initialized\n");
	//printf("CPU Clock: %I64 (measured for RDTSC Timer)\n", cpuClock);			// %llu���g���Ȃ������ꍇ
} // calcCPUClock
