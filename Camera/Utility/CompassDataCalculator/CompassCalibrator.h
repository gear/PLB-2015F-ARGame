/**************************************************************************
		コンパスキャリブレーションクラス				「CompassCalibrator.h」
		Created 2015/03/28 by 佐藤俊樹
		概要:
			いくつかのデータを取得し、その最大・最小値から中間値を求め、
			それをオフセットとする。
			角度を計算する際は、実測データからオフセットを引いたものを用いる。
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
			コンパスキャリブレーション
	 *************************************************************/

	/* オフセットの再計算 */
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

	/* Offsetの計算 */
	Vector3f updateCalibrationOffset(Vector3f srcData){
		calibrationOffset.x = (min.x + max.x) / 2.0f;
		calibrationOffset.y = (min.y + max.y) / 2.0f;
		calibrationOffset.z = (min.z + max.z) / 2.0f;
		return calibrationOffset;
	} // updateCalibrationOffset

	/* Calibrationオフセットを取得 */
	Vector3f getCalibrationOffset(){
		return calibrationOffset;
	} // getCalibrationOffset
	
protected:

	Vector3f min, max;
	Vector3f calibrationOffset;

};
