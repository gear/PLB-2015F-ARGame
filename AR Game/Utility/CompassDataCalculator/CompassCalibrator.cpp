#include "CompassCalibrator.h"

CompassCalibrator::CompassCalibrator(){
	calibrationOffset.x = 0.0f;
	calibrationOffset.y = 0.0f;
	calibrationOffset.z = 0.0f;
	min.x = min.y = min.z = 10000000.0f;
	max.x = max.y = max.z = -10000000.0f;
}

CompassCalibrator::~CompassCalibrator(){}
