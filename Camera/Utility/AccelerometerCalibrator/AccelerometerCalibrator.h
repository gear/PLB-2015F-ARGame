/**************************************************************************
		加速度センサキャリブレーションクラス				「AccelerometerCalibrator.h」
		Created 2013.03.03 by 佐藤俊樹
		概要:
			加速度センサの各軸のオフセット値及び重力加速度情報を保存する。
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

	/* ファイルから読み込む */
	int createCalibrationDataFromFile(const char *filename, SensorOffsetData *dstCalibrationData);

	/* ファイルに保存する */
	int saveCalibrationDataToFile(const char *filename);

	/* キャリブレーションデータのセット */
	void setCalibrationData(SensorOffsetData srcCalibrationData);

protected:

	int isInitialized;
	SensorOffsetData calibrationData;

};

#endif