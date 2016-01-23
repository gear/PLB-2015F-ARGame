#include "AccelerometerCalibrator.h"

AccelerometerCalibrator::AccelerometerCalibrator(){}

AccelerometerCalibrator::~AccelerometerCalibrator(){}

/* ファイルから読み込む */
int AccelerometerCalibrator::createCalibrationDataFromFile(const char *filename, SensorOffsetData *dstCalibrationData){
	char lineBuffer[64];
	fstream offsetConfigFile(filename, ios::in);
	if ( offsetConfigFile.fail() ){
		printf("加速度センサのキャリブレーションファイルが読み込めませんでした: %d\n", filename);
		return -1;
	}
	offsetConfigFile.getline(lineBuffer, 64);
	_snscanf(lineBuffer, 64, "%f %f %f %f", &calibrationData.accelerationOffset.x, &calibrationData.accelerationOffset.y, &calibrationData.accelerationOffset.z, &calibrationData.gravityAccelerationOffset);
	printf("加速度センサのキャリブレーションデータを読み込みました:\n\tOffset(%03.02f %03.02f %03.02f)\tGravity: %03.02f\n", calibrationData.accelerationOffset.x, calibrationData.accelerationOffset.y, calibrationData.accelerationOffset.z, calibrationData.gravityAccelerationOffset);
	offsetConfigFile.close();
	dstCalibrationData->accelerationOffset.x = calibrationData.accelerationOffset.x;
	dstCalibrationData->accelerationOffset.y = calibrationData.accelerationOffset.y;
	dstCalibrationData->accelerationOffset.z = calibrationData.accelerationOffset.z;
	dstCalibrationData->gravityAccelerationOffset = calibrationData.gravityAccelerationOffset;
	isInitialized = 1;
	return 0;
} // createCalibrationDataFromFile

/* ファイルに保存する */
int AccelerometerCalibrator::saveCalibrationDataToFile(const char *filename){
	if ( !isInitialized ){
		printf("キャリブレーションデータが初期化されていません。\n");
		return -1;
	}

	fstream offsetConfigFile(filename, ios::out);
	offsetConfigFile 
		<< calibrationData.accelerationOffset.x << ' ' 
		<< calibrationData.accelerationOffset.y << ' ' 
		<< calibrationData.accelerationOffset.z << ' ' 
		<< calibrationData.gravityAccelerationOffset << endl;
	offsetConfigFile.close();

	printf("加速度センサのキャリブレーションファイルを保存しました: %s\n", filename);

	return 0;
} // saveCalibrationDataToFile

/* キャリブレーションデータのセット */
void AccelerometerCalibrator::setCalibrationData(SensorOffsetData srcCalibrationData){
	calibrationData.accelerationOffset = srcCalibrationData.accelerationOffset;
	calibrationData.gravityAccelerationOffset = srcCalibrationData.gravityAccelerationOffset;
} // setCalibrationData