#include "CaptureDevice_PGRCamera2_FireflyMV.h"

/* Singleton�C���X�^���X�Ə������t���O */
FireflyMV *FireflyMV::instance = NULL;

/******************************************
							Singleton���\�b�h
 ******************************************/

/* Singleton�C���X�^���X���� */
FireflyMV *FireflyMV::getInstance(){
	if ( instance ){
		return instance;
	}else{
		instance = new FireflyMV();
		return instance;
	}
} // bb2GetInstance

/* Singleton�C���X�^���X�j�� */
void FireflyMV::releaseInstance(){
	if ( instance ){
		delete instance;
		instance = NULL;
	}
} // bb2ReleaseInstance

/* �R���X�g���N�^ */
FireflyMV::FireflyMV() : PGRCamera(){
	cameraIndex = 0;
	isCameraStarted = 0;
	isCustomParameterInitialized = 0;
	tmpImageHeader = NULL;
} // �R���X�g���N�^

/* �f�X�g���N�^ */
FireflyMV::~FireflyMV(){
} // �f�X�g���N�^

int FireflyMV::getMaxCameraWidth(){
	return 752;
} // getMaxCameraWidth
