/**************************************************************************
		�R���p�X�L�����u���[�V�����N���X				�uCompassCalibrator.h�v
		Created 2015/03/28 by �����r��
		�T�v:
			�������̃f�[�^���擾���A���̍ő�E�ŏ��l���璆�Ԓl�����߁A
			������I�t�Z�b�g�Ƃ���B
			�p�x���v�Z����ۂ́A�����f�[�^����I�t�Z�b�g�����������̂�p����B
 **************************************************************************/
#pragma once

#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

#include "../../Definitions.h"

class CompassCalibrator{

public:

	CompassCalibrator();
	virtual ~CompassCalibrator();
	/*************************************************************
			�R���p�X�L�����u���[�V����
	 *************************************************************/

	/* �I�t�Z�b�g�̍Čv�Z */
	Vector3f calculateCalibrationOffset(const vector<Vector3f> &srcCompassDataArray){
		max.x = max.y = max.z = -100000.0f;
		min.x = min.y = min.z = 1000000.0f;
		for (int i = 0; i < srcCompassDataArray.size(); i++){
			if(max.x < srcCompassDataArray[i].x){
				max.x = srcCompassDataArray[i].x;
			}
			if(min.x > srcCompassDataArray[i].x){
				min.x = srcCompassDataArray[i].x;
			}
				if(max.y < srcCompassDataArray[i].y){
				max.y = srcCompassDataArray[i].y;
			}
			if(min.y > srcCompassDataArray[i].y){
				min.y = srcCompassDataArray[i].y;
			}
				if(max.z < srcCompassDataArray[i].z){
				max.z = srcCompassDataArray[i].z;
			}
			if(min.z > srcCompassDataArray[i].z){
				min.z = srcCompassDataArray[i].z;
			}
		}
		calibrationOffset.x = (min.x + max.x) / 2.0f;
		calibrationOffset.y = (min.y + max.y) / 2.0f;
		calibrationOffset.z = (min.z + max.z) / 2.0f;
		return calibrationOffset;
	} // calculateCalibrationOffset

	/* Offset�̌v�Z */
	Vector3f updateCalibrationOffset(Vector3f srcData){
		calibrationOffset.x = (min.x + max.x) / 2.0f;
		calibrationOffset.y = (min.y + max.y) / 2.0f;
		calibrationOffset.z = (min.z + max.z) / 2.0f;
		return calibrationOffset;
	} // updateCalibrationOffset

	/* Calibration�I�t�Z�b�g���擾 */
	Vector3f getCalibrationOffset(){
		return calibrationOffset;
	} // getCalibrationOffset
	
protected:

	Vector3f min, max;
	Vector3f calibrationOffset;

};
