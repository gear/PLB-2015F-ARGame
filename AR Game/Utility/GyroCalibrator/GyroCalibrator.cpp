#include "GyroCalibrator.h"

GyroCalibrator::GyroCalibrator(){}

GyroCalibrator::~GyroCalibrator(){}

/* ファイルから読み込む */
int GyroCalibrator::createCalibrationDataFromFile(const char *filename, SensorOffsetData *dstCalibrationData){
	char lineBuffer[64];
	fstream offsetConfigFile(filename, ios::in);
	if ( offsetConfigFile.fail() ){
		printf("ジャイロセンサのキャリブレーションファイルが読み込めませんでした: %d\n", filename);
		return -1;
	}
	offsetConfigFile.getline(lineBuffer, 64);
	_snscanf(lineBuffer, 64, "%f %f %f", &calibrationData.gyroOffset.x, &calibrationData.gyroOffset.y, &calibrationData.gyroOffset.z);
	printf("ジャイロセンサのキャリブレーションデータを読み込みました:\n\tOffset(%03.02f %03.02f %03.02f)\n", calibrationData.gyroOffset.x, calibrationData.gyroOffset.y, calibrationData.gyroOffset.z);
	offsetConfigFile.close();
	dstCalibrationData->gyroOffset.x = calibrationData.gyroOffset.x;
	dstCalibrationData->gyroOffset.y = calibrationData.gyroOffset.y;
	dstCalibrationData->gyroOffset.z = calibrationData.gyroOffset.z;
	isInitialized = 1;
	return 0;
} // createCalibrationDataFromFile

/* ファイルに保存する */
int GyroCalibrator::saveCalibrationDataToFile(const char *filename){
	if ( !isInitialized ){
		printf("キャリブレーションデータが初期化されていません。\n");
		return -1;
	}

	fstream offsetConfigFile(filename, ios::out);
	offsetConfigFile 
		<< calibrationData.gyroOffset.x << ' ' 
		<< calibrationData.gyroOffset.y << ' ' 
		<< calibrationData.gyroOffset.z << endl;
	offsetConfigFile.close();

	printf("ジャイロセンサのキャリブレーションファイルを保存しました: %s\n", filename);

	return 0;
} // saveCalibrationDataToFile
	
/* キャリブレーションデータのセット */
void GyroCalibrator::setCalibrationData(SensorOffsetData srcCalibrationData){
	calibrationData.gyroOffset = srcCalibrationData.gyroOffset;
} // setCalibrationData