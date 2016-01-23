/****************************************************************
		CaptureDevice_PGRCamera.h.h		FlyCapture�J�����N���X(Flycapture SDK Ver.2�Ή���)
		�T�v:
			FlyCapture�J�����𐧌䂷��N���X�B
			��{�I�ɃJ�X�^���C���[�W���[�h�œ������B
		ToDo: �J���[���������͂܂��������Ȃ̂œ����
		2014/06/11 by �����r��@UECISMS
 ****************************************************************/
#ifndef __CAPTUREDEVICE_FLYCAPTURECAM2_H__
#define __CAPTUREDEVICE_FLYCAPTURECAM2_H__

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "CaptureDevice.h"

#include <FlyCapture2.h>
using namespace FlyCapture2;

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

/* �J�X�^���C���[�W�̃p�����[�^ */
typedef struct{
	unsigned int uiMode;
	unsigned int uiImagePosLeft;		// left
	unsigned int uiImagePosTop;			// top
	unsigned int uiWidth;						// width
	unsigned int uiHeight;					// height
	float fBandWidth;								// speed
	PixelFormat format;							// format
} CUSTOM_IMAGE_PARAMETER;

static const int FLY_DEFAULT_CAMERA_INDEX = 0;

/* �f�t�H���g�J�X�^���C���[�W�p�����[�^ */
static const int FLY_DEFAULT_VEHICLE = 0;
static const int FLY_DEFAULT_LEFT = 0;
static const int FLY_DEFAULT_WIDTH = 640;
static const int FLY_DEFAULT_HEIGHT = 480;
static const float FLY_DEFAULT_SPEED = 100.0f;

/************************************************
					FlyCapture�J�����N���X
 ************************************************/
class PGRCamera : public CaptureDevice{

public:

	/* �R���X�g���N�^ */
	PGRCamera();
	
	/* �f�X�g���N�^ */
	virtual ~PGRCamera();
	
	/* �O���u�����t���[���f�[�^�̃|�C���^�����̂܂�IPL�C���[�W�w�b�_�ɂ������ēn�� */
	virtual Mat *grabImage();

	/* �^����ꂽ�C���[�W�f�[�^��imageData�|�C���^�Ƀt���[���̃|�C���^���������� */
	virtual void grabImage(Mat &imageHeader);
		
	/************************************
		Custom Image (Region of Interest)
	 ************************************/

	/* �J�X�^���C���[�W�p�����[�^��S���ݒ肷��B
		 ROI�̃T�C�Y�͐ݒ�ł���l�ɃJ�������ɐ���������̂�FlyCap.exe��CustomMode(Format7)�̍��ڂ��}�j���A�����Q�� */
	virtual int setCustomImageParameter( int x, int y, int width, int height, PixelFormat format, Mode mode = MODE_0 );

	/**********************************
						������ / �I��
	 **********************************/

	/* ������ */
	virtual int initialize();

	/* ������ */
	virtual int initialize(int x, int y, int width, int height, PixelFormat format);

	/* �I�� */
	virtual void finalize();

	/* �J�����̍ăX�^�[�g */
	virtual void restartCamera();

	/***********************************
						�J�n / ��~
	 ***********************************/

	/* �J�n */
	virtual int startCamera();

	/* ��~ */
	virtual int stopCamera();

	/****************************************
										�ݒ�
	 ****************************************/

	/* �J�����C���f�b�N�X���Z�b�g���� */
	virtual void setCameraIndex(int index);

	/****************************************
						�J�����p�����[�^�ݒ�
		(Flycap.exe��Auto�ݒ���I�t�ɂ��Ă����K�v������)
	 ****************************************/

	/* �Q�C�� */
	void setGain(float gainValue);

	/* �u���C�g�l�X */
	void setBrightness(float brightnessValue);

	/* �G�N�X�|�[�W�� */
	void setExposure(float exposureValue);

	/* �K���} */
	void setGamma(float gammaValue);

	/* �V���b�^�[�X�s�[�h */
	void setShutterSpeed(float shutterValue);

	/* �t���[�����[�g */
	void setFrameRate(float frameRateValue);

	/* ROI���擾 */
	virtual CvRect getROI();

	/* Image Size/Offset Units */
	virtual int getImageSizeUnit();
	virtual int getImageOffsetUnit();

	/* width/height */
	virtual int getMaxCameraWidth();
	virtual int getMaxCameraHeight();

	/****************************************
						�J���[�v���Z�X
	 ****************************************/
	virtual void setColorProcessing();

protected:

	Error error;

	PGRGuid guid;

	BusManager busManager;

	Camera camera;
	CameraInfo camInfo;

	Image rawImage;    
	Image convertedImage;

	Property frmRate;

	unsigned int numCameras;

	CvRect roiRect;

	Mode format7Mode;
	Format7Info format7Info;
	Format7ImageSettings format7ImageSettings;
	Format7PacketInfo format7PacketInfo;
  PixelFormat pixelFormat;

	bool supported;

 	Mat *tmpImageHeader;

	int enableColorProcessing;
	int isCustomParameterInitialized;
  int isCameraStarted;											// �L���v�`�����J�n����Ă��邩
	int cameraIndex;

	CUSTOM_IMAGE_PARAMETER customImageParam;	// Custom Image�̐ݒ�

};
#endif