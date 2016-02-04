/********************************************
			�L���v�`���f�o�C�X���N���X
	Date:
			Create 2014/06/11 by �����r��@UECISMS
 ********************************************/
#ifndef __CVCAPTUREDEVICE_H__
#define __CVCAPTUREDEVICE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <opencv2/opencv.hpp>
using namespace cv;

class CaptureDevice{

public:

	/* �R���X�g���N�^ */
	CaptureDevice();

	/* �f�X�g���N�^ */
	virtual ~CaptureDevice();

	/***********************************
					�t���[���̃L���v�`��
	 ***********************************/
	virtual void grabImage(Mat &imageHeader) = 0;		// �^����ꂽ�C���[�W�w�b�_�Ƀf�[�^��ǉ�����
	virtual Mat *grabImage() = 0;										// �f�[�^�t���̃C���[�W�w�b�_��Ԃ�

	/***********************************
						������ / ���
	 ***********************************/
	virtual int initialize() = 0;
	virtual void finalize() = 0;

	/***********************************
						�J�n / ��~
	 ***********************************/
	virtual int startCamera() = 0;
	virtual int stopCamera() = 0;

protected:

	int isInitialized;
	int imageWidth, imageHeight;

};
#endif