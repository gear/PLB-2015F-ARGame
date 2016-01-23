/**************************************************************************
		方位計算クラス				「CompassDataCalculator.h」
		Created 2015.03.28 by 佐藤俊樹
 **************************************************************************/
#pragma once

#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

#include "../../Definitions.h"

class CompassDataCalculator{

public:

	/* 方位の計算 */
	static float calcOrientationXY(Vector3f srcData, Vector3f calibrationOffset){

		float angle = atan2f(srcData.y, srcData.x);

		angle -= 3.141592265f / 2.0f;

		if(angle < 0){
			angle += 2 * 3.1415926f;
		}else if(angle > 360.0f){
			angle -= 2 * 3.1415926f;
		}

		return angle * 180.0f / 3.14159265f - 7.0f;		// 調布の磁気偏角: +7deg
	} // calcOrientationXY

	/* 方位の計算 */
	static float calcOrientationZ(Vector3f srcData, Vector3f calibrationOffset){

		Vector3f tmp;
		tmp.x = srcData.x - calibrationOffset.x;
		tmp.y = srcData.y - calibrationOffset.y;
		tmp.z = srcData.z - calibrationOffset.z;
		//tmp.x = srcData.x;
		//tmp.y = srcData.y;
		//tmp.z = srcData.z;

		float l = sqrtf((tmp.x) * (tmp.x) + (tmp.y) * (tmp.y));
		float angle =  atan2f(l, tmp.z) * 180.0f / 3.14159265f;
		
		if(angle < 0){
			angle += 360.0f;
		}else if(angle > 360.0f){
			angle -= 360.0f;
		}

		return angle;

	} // calcOrientationZ

protected:

};
