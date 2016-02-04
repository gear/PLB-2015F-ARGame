/**************************************************************************
		�����x�Z���T�L�����u���[�V�����N���X				�uAccelerometerCalibrator.h�v
		Created 2013.03.03 by �����r��
		�T�v:
			�����x�Z���T�̊e���̃I�t�Z�b�g�l�y�яd�͉����x����ۑ�����B
 **************************************************************************/
#ifndef __ACCELEROMETERCALIBRATOR_H__
#define __ACCELEROMETERCALIBRATOR_H__

#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

#include "../../Definitions.h"

class AccelerometerCalibrator{

public:

	AccelerometerCalibrator();
	virtual ~AccelerometerCalibrator();

	/* �t�@�C������ǂݍ��� */
	int createCalibrationDataFromFile(const char *filename, SensorOffsetData *dstCalibrationData);

	/* �t�@�C���ɕۑ����� */
	int saveCalibrationDataToFile(const char *filename);

	/* �L�����u���[�V�����f�[�^�̃Z�b�g */
	void setCalibrationData(SensorOffsetData srcCalibrationData);

protected:

	int isInitialized;
	SensorOffsetData calibrationData;

};

#endif