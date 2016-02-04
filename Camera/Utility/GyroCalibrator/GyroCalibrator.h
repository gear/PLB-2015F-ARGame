/**************************************************************************
		ジャイロセンサキャリブレーションクラス				「GyroCalibrator.h」
		Created 2013.03.04 by 佐藤俊樹
		概要:
			ジャイロセンサの各軸のオフセット値及び重力加速度情報を保存する。
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