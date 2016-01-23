#include "SensorDataVisualizer.h"

SensorDataVisualizer::SensorDataVisualizer(){
	inputter = Inputter::getInstance();
	timer = Timer::getInstance();
}

SensorDataVisualizer::~SensorDataVisualizer(){
} // SensorDataVisualizer

/* 初期化 */
int SensorDataVisualizer::initialize(float windowWidth, float windowHeight, TextRenderer *textRenderer){

	this->textRenderer = textRenderer;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	/* センサーグラフの初期化 */
	//graphAccelerationX.initialize(windowWidth, windowHeight, -windowHeight / 4 * 2 + windowHeight / 2, 1, 1, GRAPH_MAGNIFICATION_RATIO_50);
	//graphAccelerationY.initialize(windowWidth, windowHeight, -windowHeight / 4 * 2 + windowHeight / 2, 0, 0, GRAPH_MAGNIFICATION_RATIO_50);
	//graphAccelerationZ.initialize(windowWidth, windowHeight, -windowHeight / 4 * 2 + windowHeight / 2, 0, 0, GRAPH_MAGNIFICATION_RATIO_50);
	//graphAccelerationXYZ.initialize(windowWidth, windowHeight, -windowHeight / 4 * 2 + windowHeight / 2, 0, 1, GRAPH_MAGNIFICATION_RATIO_50);

	//graphGyroX.initialize(windowWidth, windowHeight, -windowHeight / 4 + windowHeight / 2, 0, 1, GRAPH_MAGNIFICATION_RATIO_50);
	//graphGyroY.initialize(windowWidth, windowHeight, -windowHeight / 4 + windowHeight / 2, 0, 0, GRAPH_MAGNIFICATION_RATIO_50);
	//graphGyroZ.initialize(windowWidth, windowHeight, -windowHeight / 4 + windowHeight / 2, 0, 0, GRAPH_MAGNIFICATION_RATIO_50);
	//graphGyroXYZ.initialize(windowWidth, windowHeight, -windowHeight / 4 + windowHeight / 2, 0, 1, GRAPH_MAGNIFICATION_RATIO_50);

	//graphAccelerationX.setRenderingOption(1, 1.0f, 0.0f, 0.0f);
	//graphAccelerationY.setRenderingOption(1, 0.0f, 1.0f, 0.0f);
	//graphAccelerationZ.setRenderingOption(1, 0.0f, 0.0f, 1.0f);
	//graphAccelerationXYZ.setRenderingOption(1, 1.0f, 0.5f, 1.0f);

	//graphGyroX.setRenderingOption(1, 1.0f, 0.4f, 0.4f);
	//graphGyroY.setRenderingOption(1, 0.4f, 1.0f, 0.4f);
	//graphGyroZ.setRenderingOption(1, 0.4f, 0.4f, 1.0f);
	//graphGyroXYZ.setRenderingOption(1, 1.0f, 0.4f, 1.0f);

	///* 加速度センサキャリブレーションデータを読み込む */
	//if ( accelerometerCalibrator.createCalibrationDataFromFile("./data/conf/accelerationOffset.dat", &sensorOffsetData) < 0 ){
	//	printf("加速度センサのオフセットデータの初期化に失敗しました。\n");
	//	return -1;
	//}
	///* ジャイロセンサキャリブレーションデータを読み込む */
	//if ( gyroCalibrator.createCalibrationDataFromFile("./data/conf/gyroOffset.dat", &sensorOffsetData) < 0 ){
	//	printf("ジャイロのオフセットデータの初期化に失敗しました。\n");
	//	return -1;
	//}

	/* コンパス */
	compassOrientationVisualizer.initialize();

	compassOffset = 0;
	latitude = longitude = 0.0f;

	return 0;
} // initialize

/* 解放 */
void SensorDataVisualizer::finalize(){
	compassOrientationVisualizer.finalize();
} // finalize

/* 描画する */
void SensorDataVisualizer::update(RawSensorData &latestRawSensorData, ProcessedSensorData &latestProcessedSensorData){

	/* センサグラフデータ追加 */
	float tmpX, tmpY, tmpZ;
	tmpX = latestRawSensorData.acceleration.x - sensorOffsetData.accelerationOffset.x;
	tmpY = latestRawSensorData.acceleration.y - sensorOffsetData.accelerationOffset.y;
	tmpZ = latestRawSensorData.acceleration.z - sensorOffsetData.accelerationOffset.z;
	latestProcessedSensorData.accelerationXYZ = sqrtf(tmpX * tmpX + tmpY * tmpY + tmpZ * tmpZ);

	///* 加速度 */
	//graphAccelerationXYZ.addData(latestProcessedSensorData.accelerationXYZ, currentSensorValueIndex);
	//graphAccelerationX.addData(latestRawSensorData.acceleration.x, currentSensorValueIndex);
	//graphAccelerationY.addData(latestRawSensorData.acceleration.y, currentSensorValueIndex);
	//graphAccelerationZ.addData(latestRawSensorData.acceleration.z, currentSensorValueIndex);
	//
	///* ジャイロ */
	//graphGyroX.addData(latestRawSensorData.gyro.x, currentSensorValueIndex);
	//graphGyroY.addData(latestRawSensorData.gyro.y, currentSensorValueIndex);
	//graphGyroZ.addData(latestRawSensorData.gyro.z, currentSensorValueIndex);

	/* GPSは更新されたタイミングのみ値をコピーする */
	if (latestRawSensorData.location.latitude != 0){
		latitude = latestRawSensorData.location.latitude;
	}
	if (latestRawSensorData.location.longitude != 0){
		longitude = latestRawSensorData.location.longitude;
	}
	if (latestRawSensorData.location.longitude != 0){
		numSatellites = latestRawSensorData.numSatellites;
	}

	/* グラフの現在位置をずらす */
	currentSensorValueIndex++;
	if (currentSensorValueIndex >= windowWidth){
		currentSensorValueIndex = 0;
	}
} // update

/* 描画する */
void SensorDataVisualizer::render(RawSensorData &latestRawSensorData, ProcessedSensorData &latestProcessedSensorData){

	/* 文字情報 */
	showTextInformation(latestRawSensorData, latestProcessedSensorData);

	///* 加速度1 */
	//graphAccelerationX.render();
	//graphAccelerationY.render();
	//graphAccelerationZ.render();
	//graphAccelerationXYZ.render();

	///* ジャイロ */
	//graphGyroX.render();
	//graphGyroY.render();
	//graphGyroZ.render();

	//printf("Ang: %03.02f\n", latestProcessedSensorData.orientationXY);

	/* コンパス */
	compassOrientationVisualizer.render(0.0f, 0.0f, latestProcessedSensorData.orientationXY);
} // render
