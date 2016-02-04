/**************************************************************************
		�W���C���Z���T�L�����u���[�V�����N���X				�uGyroCalibrator.h�v
		Created 2013.03.04 by �����r��
		�T�v:
			�W���C���Z���T�̊e���̃I�t�Z�b�g�l�y�яd�͉����x����ۑ�����B
 **************************************************************************/
#ifndef __GYROCALIBRATOR_H__
#define __GYROCALIBRATOR_H__

#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

#include "../../Definitions.h"

class GyroCalibrator{

public:

	GyroCalibrator();
	virtual ~GyroCalibrator();

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