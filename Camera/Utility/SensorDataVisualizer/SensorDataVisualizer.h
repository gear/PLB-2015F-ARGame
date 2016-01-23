/**************************************************************************
		�O���t�`��N���X				�uSensorDataVisualizer.h�v
		Created 2013.03.03 by �����r��
		�O���t���ȒP�ɕ`���B
 **************************************************************************/

#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <SDL.h>
#include <SDL_ttf.h>
#include <math.h>

#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>

//#include "GraphRenderer/GraphRenderer.h"
#include "../../Definitions.h"

/* �Z���T�L�����u���[�V���� */
#include "../AccelerometerCalibrator/AccelerometerCalibrator.h"
#include "../GyroCalibrator/GyroCalibrator.h"

#include "../../Utility/Inputter/Inputter.h"
#include "../../Utility/Timer/Timer.h"

#include "../Renderer/TextRenderer.h"

#include "../CompassOrientationVisualizer/CompassOrientationVisualizer.h"

/* �O���t�̔{�� */
static const float GRAPH_MAGNIFICATION_RATIO_1000 = (1 / 1000.0f);
static const float GRAPH_MAGNIFICATION_RATIO_500 = (1 / 500.0f);
static const float GRAPH_MAGNIFICATION_RATIO_300 = (1 / 300.0f);
static const float GRAPH_MAGNIFICATION_RATIO_100 = (1 / 100.0f);
static const float GRAPH_MAGNIFICATION_RATIO_50 = (1 / 50.0f);
static const float GRAPH_MAGNIFICATION_RATIO_10 = (1 / 10.0f);
static const float GRAPH_MAGNIFICATION_RATIO_5 = (1 / 5.0f);
static const float GRAPH_MAGNIFICATION_RATIO_1 = 1;

class SensorDataVisualizer{

public:

	SensorDataVisualizer();

	virtual ~SensorDataVisualizer();

	/* ������ */
	virtual int initialize(float windowWidth, float windowHeight, TextRenderer *textRenderer);

	/* ��� */
	virtual void finalize();

	/* �X�V���� */
	virtual void update(RawSensorData &latestRawSensorData, ProcessedSensorData &latestProcessedSensorData);

	/* �`�悷�� */
	virtual void render(RawSensorData &latestRawSensorData, ProcessedSensorData &latestProcessedSensorData);

	/* �I�t�Z�b�g�l��ۑ����� */
	virtual void saveOffsetValue(){

		static Uint32 tick = 0;

		///* �����x�̃I�t�Z�b�g�l�̎擾 */
		//if ( inputter->keyState[SDLK_x].state && tick + 1000 < timer->systemTime ){
		//	tick = timer->systemTime;
		//	float *accelerationXArrayPtr = graphAccelerationX.getDataArrayPtr();		// �O���t�Ɋi�[����Ă���f�[�^�𗘗p����
		//	float averagedXOffset = calcAverage(accelerationXArrayPtr, graphAccelerationX.getDataArraySize());
		//	sensorOffsetData.accelerationOffset.x = averagedXOffset;
		//	printf("Set X Acceleration Offset: %03.02f\n", averagedXOffset);

		//}else if ( inputter->keyState[SDLK_y].state && tick + 1000 < timer->systemTime ){
		//	tick = timer->systemTime;
		//	float *accelerationYArrayPtr = graphAccelerationY.getDataArrayPtr();		// �O���t�Ɋi�[����Ă���f�[�^�𗘗p����
		//	float averagedYOffset = calcAverage(accelerationYArrayPtr, graphAccelerationY.getDataArraySize());
		//	sensorOffsetData.accelerationOffset.y = averagedYOffset;
		//	printf("Set Y Acceleration Offset: %03.02f\n", averagedYOffset);

		//}else if ( inputter->keyState[SDLK_z].state && tick + 1000 < timer->systemTime ){
		//	tick = timer->systemTime;
		//	float *accelerationZArrayPtr = graphAccelerationZ.getDataArrayPtr();		// �O���t�Ɋi�[����Ă���f�[�^�𗘗p����
		//	float averagedZOffset = calcAverage(accelerationZArrayPtr, graphAccelerationZ.getDataArraySize());
		//	sensorOffsetData.accelerationOffset.z = averagedZOffset;
		//	printf("Set Z Acceleration Offset: %03.02f\n", averagedZOffset);

		///* �d�͉����x�萔�̑��� */
		//}else if ( inputter->keyState[SDLK_g].state && tick + 1000 < timer->systemTime ){
		//	tick = timer->systemTime;
		//	float *accelerationXYZ = graphAccelerationXYZ.getDataArrayPtr();		// �O���t�Ɋi�[����Ă���f�[�^�𗘗p����
		//	float averagedGOffset = calcAverage(accelerationXYZ, graphAccelerationXYZ.getDataArraySize());
		//	sensorOffsetData.gravityAccelerationOffset = averagedGOffset;
		//	printf("Set Gravity Acceleration Offset: %03.02f\n", averagedGOffset);
		//}
	
		///* �W���C���̃I�t�Z�b�g�l�̎擾 */
		//if ( inputter->keyState[SDLK_k].state && tick + 1000 < timer->systemTime ){
		//	float *gyroDataArrayPtr = graphGyroX.getDataArrayPtr();		// �O���t�Ɋi�[����Ă���f�[�^�𗘗p����
		//	float averagedXOffset = calcAverage(gyroDataArrayPtr, graphGyroX.getDataArraySize());
		//	sensorOffsetData.gyroOffset.x = averagedXOffset;
		//	gyroDataArrayPtr = graphGyroY.getDataArrayPtr();		// �O���t�Ɋi�[����Ă���f�[�^�𗘗p����
		//	float averagedYOffset = calcAverage(gyroDataArrayPtr, graphGyroY.getDataArraySize());
		//	sensorOffsetData.gyroOffset.y = averagedYOffset;
		//	gyroDataArrayPtr = graphGyroZ.getDataArrayPtr();		// �O���t�Ɋi�[����Ă���f�[�^�𗘗p����
		//	float averagedZOffset = calcAverage(gyroDataArrayPtr, graphGyroZ.getDataArraySize());
		//	sensorOffsetData.gyroOffset.z = averagedZOffset;
		//	printf("Set Gyro Offset: %03.02f %03.02f %03.02f\n", averagedXOffset, averagedYOffset, averagedZOffset);
		//	tick = timer->systemTime;
		//}

		/* �A�b�v�f�[�g */
		if ( inputter->keyState[SDLK_u].state && tick + 1000 < timer->systemTime ){
			gyroCalibrator.setCalibrationData(sensorOffsetData);
			accelerometerCalibrator.setCalibrationData(sensorOffsetData);
			printf("�Z���T�̖����׃I�t�Z�b�g�l���X�V����܂����B\n");
			tick = timer->systemTime;
		}	
	}

protected:
	
	Inputter *inputter;
	Timer *timer;

	float latitude, longitude;
	int numSatellites;

	/* �Z���T�f�[�^�̃O���t */
	//GraphRenderer graphGyroX, graphGyroY, graphGyroZ, graphGyroXYZ;																										// �W���C��
	//GraphRenderer graphAccelerationX, graphAccelerationY, graphAccelerationZ, graphAccelerationXYZ;										// �����x1

	CompassOrientationVisualizer compassOrientationVisualizer;

	/* �����`�� */
	TextRenderer *textRenderer;

	/* ���݈ʒu */
	int currentSensorValueIndex;

	/* �E�C���h�E�T�C�Y */
	float windowWidth, windowHeight;

	/* �Z���T�I�t�Z�b�g�f�[�^ */
	SensorOffsetData sensorOffsetData;
	float compassOffset;

	/* �����x�Z���T�L�����u���[�^ */
	AccelerometerCalibrator accelerometerCalibrator;
	GyroCalibrator gyroCalibrator;

	/* ���όv�Z */
	float calcAverage(float *srcArray, int numData){
		if ( numData == 0 ){
			return 0.0f;
		}
		float result = 0.0f;
		for (int i = 0; i < numData; i++){
			result += srcArray[i];
		}
		return result / numData;
	} // calcAverage

	/* �e�L�X�g���̕\�� */
	void showTextInformation(RawSensorData &rawSensorData, ProcessedSensorData &processedSensorData){
		char tmpBuffer[128];

		///* �����x */
		//_snprintf(tmpBuffer, 128, "AccX: %04.1f", rawSensorData.acceleration.x - sensorOffsetData.accelerationOffset.x);
		//textRenderer->renderASCII(tmpBuffer, 100, windowHeight / 10);

		//_snprintf(tmpBuffer, 128, "AccY: %04.1f", rawSensorData.acceleration.y - sensorOffsetData.accelerationOffset.y);
		//textRenderer->renderASCII(tmpBuffer, 100, windowHeight / 10 + 20);

		//_snprintf(tmpBuffer, 128, "AccZ: %04.1f", rawSensorData.acceleration.z - sensorOffsetData.accelerationOffset.z);
		//textRenderer->renderASCII(tmpBuffer, 100, windowHeight / 10 + 40);

		///* �����x���v */
		//_snprintf(tmpBuffer, 128, "AccXYZ: %04.1f", processedSensorData.accelerationXYZ);
		//textRenderer->renderASCII(tmpBuffer, 100, windowHeight / 10 + 60);

		//_snprintf(tmpBuffer, 128, "AccXYZNoG: %04.1f", processedSensorData.accelerationXYZ - sensorOffsetData.gravityAccelerationOffset);
		//textRenderer->renderASCII(tmpBuffer, 100, windowHeight / 10 + 80);

		///* �W���C�� */
		//_snprintf(tmpBuffer, 128, "GyrX: %04.1f", rawSensorData.gyro.x - sensorOffsetData.gyroOffset.x);
		//textRenderer->renderASCII(tmpBuffer, 250, windowHeight / 10);

		//_snprintf(tmpBuffer, 128, "GyrY: %04.1f", rawSensorData.gyro.y - sensorOffsetData.gyroOffset.y);
		//textRenderer->renderASCII(tmpBuffer, 250, windowHeight / 10 + 20);

		//_snprintf(tmpBuffer, 128, "GyrZ: %04.1f", rawSensorData.gyro.z - sensorOffsetData.gyroOffset.z);
		//textRenderer->renderASCII(tmpBuffer, 250, windowHeight / 10 + 40);

		//_snprintf(tmpBuffer, 128, "GyrXYZ: %04.1f", processedSensorData.gyroXYZ - sensorOffsetData.gyroOffset.z);
		//textRenderer->renderASCII(tmpBuffer, 250, windowHeight / 10 + 60);

		/* �R���p�X */
		_snprintf(tmpBuffer, 128, "CmpX: %04.1f", rawSensorData.compass.x);
		textRenderer->renderASCII(tmpBuffer, 400, windowHeight / 10);

		_snprintf(tmpBuffer, 128, "CmpY: %04.1f", rawSensorData.compass.y);
		textRenderer->renderASCII(tmpBuffer, 400, windowHeight / 10 + 20);

		_snprintf(tmpBuffer, 128, "CmpZ: %04.1f", rawSensorData.compass.z);
		textRenderer->renderASCII(tmpBuffer, 400, windowHeight / 10 + 40);

		_snprintf(tmpBuffer, 128, "OriXY: %03.1fdeg", processedSensorData.orientationXY);
		textRenderer->renderASCII(tmpBuffer, 400, windowHeight / 10 + 80);

		/* GPS */
		_snprintf(tmpBuffer, 128, "Lat: %03.07f", latitude);
		textRenderer->renderASCII(tmpBuffer, 550, windowHeight / 10);

		_snprintf(tmpBuffer, 128, "Lon: %03.07f", longitude);
		textRenderer->renderASCII(tmpBuffer, 550, windowHeight / 10 + 20);

		_snprintf(tmpBuffer, 128, "Sats: %d", numSatellites);
		textRenderer->renderASCII(tmpBuffer, 550, windowHeight / 10 + 50);

	} // showTextInformation

};
