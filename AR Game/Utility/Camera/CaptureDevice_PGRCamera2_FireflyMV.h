/****************************************************************
		CaptureDevice_FireflyMV.h.h		FlyCapture�J�����N���X(Flycapture SDK Ver.2�Ή���)
		�J����: Firefly MV(752x480@60fps)
		Date:
			Create	2014/06/11 By �����r��
 ****************************************************************/
#ifndef __CAPTUREDEVICE_FLYCAPTURECAM2_FIREFLYMV_H__
#define __CAPTUREDEVICE_FLYCAPTURECAM2_FIREFLYMV_H__

#include "CaptureDevice_PGRCamera2.h"

/************************************************
					FireflyMV�N���X
 ************************************************/
class FireflyMV : public PGRCamera{

public:

	/******************************************
								Singleton���\�b�h
	 ******************************************/

	/* Singleton�C���X�^���X���� */
	static FireflyMV *getInstance();

	/* Singleton�C���X�^���X�j�� */
	static void releaseInstance();

	/* width/height */
	virtual int getMaxCameraWidth();

private:

	/***************************
	  �V���O���g���C���X�^���X
	***************************/
	static FireflyMV *instance;

	/* �R���X�g���N�^ */
	FireflyMV();
	
	/* �f�X�g���N�^ */
	virtual ~FireflyMV();
	
};
#endif