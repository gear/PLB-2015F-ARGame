#pragma once

#include <stdlib.h>
#include <vector>
using namespace std;

/* Window size */
static const int WINDOW_WIDTH = 752;				// must be same as the camera resolution
static const int WINDOW_HEIGHT = 480;
//static const int WINDOW_WIDTH = 1920;
//static const int WINDOW_HEIGHT = 1080;

typedef struct{
	unsigned int id;
	unsigned int lastUpdatedTime;
	float x, y;
	float size;
} TrackingObject;

/* ��� - State */
typedef enum {
	STATE_PROCESS_IMAGE,
} FLASHLIGHT_DETECTION_STATE;

/* �}�[�J�R�}���h - Marker Commands */
typedef enum{
	MARKER_COMMAND_LIGHT_ON = 1,											// ���C�g��On
	MARKER_COMMAND_LIGHT_OFF = 2,											// ���C�g��Off
	MARKER_COMMAND_SWITCH_TO_NAVIGATION_MODE = 3,			// �i�r�Q�[�V�������[�h��
	MARKER_COMMAND_SWITCH_TO_AUGMENTATION_MODE = 4,		// ���d�􃂁[�h��
} MARKER_COMMAND;

/* �V���b�^�X�s�[�h���[�h */
typedef enum{
	IMAGE_PROCESSING_MODE_FAR = 1,			// ���邭����
	IMAGE_PROCESSING_MODE_NEAR = 2,			// �Â�����
	IMAGE_PROCESSING_MODE_MARKER_DETECTION_ONLY = 3,			// �}�[�J���o�̂�
} IMAGE_PROCESSING_MODE;

/* ���ʕ\���^�C�v */
typedef enum{
	RESULT_MODE_DEBUG_SRC_IMAGE,
	RESULT_MODE_DEBUG_BINARIZED_IMAGE,
	RESULT_MODE_DEBUG_RESULT_IMAGE,
	RESULT_MODE_DEBUG_NO_IMAGE,
	RESULT_MODE_SENSOR_DATA,					// �Z���T�f�[�^
	RESULT_MODE_APPLICATION,					// �A�v����ʕ\��
} RESULT_MODE;

/* 2�����x�N�g�� */
typedef struct{
	float x;
	float y;
} Vector2f;

/* 3�����x�N�g�� */
typedef struct{
	float x;
	float y;
	float z;
} Vector3f;

/* �ܓx�o�x */
typedef struct{
	double latitude;
	double longitude;
} Location;

static const int MAX_TRACKING_OBJECTS = 10;

/* �����x�Z���T�L�����u���[�V�����f�[�^ */
typedef struct{
	Vector3f accelerationOffset;	// �e���̖����׎��̃Z���T�l
	Vector3f gyroOffset;					// �������W���C��
	float gravityAccelerationOffset;		// �d�͉����x�̑��茋��
} SensorOffsetData;

/*******************************************************
											���ʃf�[�^
 *******************************************************/

/* �摜�������ʃf�[�^ */
typedef struct{
	TrackingObject trackingObjectArray[MAX_TRACKING_OBJECTS];
	IMAGE_PROCESSING_MODE imageProcessingMode;
	char detectedMarkerID;		// �����o����-1
	float detectedMarkerPositionX;
	float detectedMarkerPositionY;
} ImageProcessingResult;

/* �Z���T�f�[�^ */
typedef struct{
	Vector3f acceleration;		// 16bit x, y and z
	Vector3f gyro;						// 16bit x, y and z
	Vector3f compass;
	Location location;
	unsigned char numSatellites;							// �q���̐�
} RawSensorData;

/* ���������Z���T�f�[�^ */
typedef struct{
	float accelerationXYZ;
	float gyroXYZ;
	float orientationXY;
} ProcessedSensorData;

/* �l�b�g���[�N�f�[�^ */
typedef struct{
	ImageProcessingResult imageProcessingResult;
	RawSensorData rawSensorData;
	ProcessedSensorData processedSensorData;
} NetworkData;

/* RS232C�p�P�b�g�T�C�Y */
static const int RS232C_PACKET_SIZE = 27;		// 2byte x 3axis x 2sensors + 4byte * 3axis + 4byte(CompassXY) + 4byte(Latitude) + 4byte(Longitude) = 36byte
static const int PACKET_WITH_HEADER_SIZE = (RS232C_PACKET_SIZE + 2);					// ���E�I�[�������܂�1�p�P�b�g�T�C�Y
static const int PACKET_SIZE_WITHOUT_HEADER = (PACKET_WITH_HEADER_SIZE - 1);			// �w�b�_�������������p�P�b�g�T�C�Y
