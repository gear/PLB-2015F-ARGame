#include "SensorDataVisualizer.h"

SensorDataVisualizer::SensorDataVisualizer(){
	inputter = Inputter::getInstance();
	timer = Timer::getInstance();
}

SensorDataVisualizer::~SensorDataVisualizer(){
} // SensorDataVisualizer

/* ������ */
int SensorDataVisualizer::initialize(float windowWidth, float windowHeight, TextRenderer *textRenderer){

	this->textRenderer = textRenderer;
	this->windowWidth = windowWidth;
	this->windowHeight = windowHeight;

	/* �Z���T�[�O���t�̏����� */
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

	///* �����x�Z���T�L�����u���[�V�����f�[�^��ǂݍ��� */
	//if ( accelerometerCalibrator.createCalibrationDataFromFile("./data/conf/accelerationOffset.dat", &sensorOffsetData) < 0 ){
	//	printf("�����x�Z���T�̃I�t�Z�b�g�f�[�^�̏������Ɏ��s���܂����B\n");
	//	return -1;
	//}
	///* �W���C���Z���T�L�����u���[�V�����f�[�^��ǂݍ��� */
	//if ( gyroCalibrator.createCalibrationDataFromFile("./data/conf/gyroOffset.dat", &sensorOffsetData) < 0 ){
	//	printf("�W���C���̃I�t�Z�b�g�f�[�^�̏������Ɏ��s���܂����B\n");
	//	return -1;
	//}

	/* �R���p�X */
	compassOrientationVisualizer.initialize();

	compassOffset = 0;
	latitude = longitude = 0.0f;

	return 0;
} // initialize

/* ��� */
void SensorDataVisualizer::finalize(){
	compassOrientationVisualizer.finalize();
} // finalize

/* �`�悷�� */
void SensorDataVisualizer::update(RawSensorData &latestRawSensorData, ProcessedSensorData &latestProcessedSensorData){

	/* �Z���T�O���t�f�[�^�ǉ� */
	float tmpX, tmpY, tmpZ;
	tmpX = latestRawSensorData.acceleration.x - sensorOffsetData.accelerationOffset.x;
	tmpY = latestRawSensorData.acceleration.y - sensorOffsetData.accelerationOffset.y;
	tmpZ = latestRawSensorData.acceleration.z - sensorOffsetData.accelerationOffset.z;
	latestProcessedSensorData.accelerationXYZ = sqrtf(tmpX * tmpX + tmpY * tmpY + tmpZ * tmpZ);

	///* �����x */
	//graphAccelerationXYZ.addData(latestProcessedSensorData.accelerationXYZ, currentSensorValueIndex);
	//graphAccelerationX.addData(latestRawSensorData.acceleration.x, currentSensorValueIndex);
	//graphAccelerationY.addData(latestRawSensorData.acceleration.y, currentSensorValueIndex);
	//graphAccelerationZ.addData(latestRawSensorData.acceleration.z, currentSensorValueIndex);
	//
	///* �W���C�� */
	//graphGyroX.addData(latestRawSensorData.gyro.x, currentSensorValueIndex);
	//graphGyroY.addData(latestRawSensorData.gyro.y, currentSensorValueIndex);
	//graphGyroZ.addData(latestRawSensorData.gyro.z, currentSensorValueIndex);

	/* GPS�͍X�V���ꂽ�^�C�~���O�̂ݒl���R�s�[���� */
	if (latestRawSensorData.location.latitude != 0){
		latitude = latestRawSensorData.location.latitude;
	}
	if (latestRawSensorData.location.longitude != 0){
		longitude = latestRawSensorData.location.longitude;
	}
	if (latestRawSensorData.location.longitude != 0){
		numSatellites = latestRawSensorData.numSatellites;
	}

	/* �O���t�̌��݈ʒu�����炷 */
	currentSensorValueIndex++;
	if (currentSensorValueIndex >= windowWidth){
		currentSensorValueIndex = 0;
	}
} // update

/* �`�悷�� */
void SensorDataVisualizer::render(RawSensorData &latestRawSensorData, ProcessedSensorData &latestProcessedSensorData){

	/* ������� */
	showTextInformation(latestRawSensorData, latestProcessedSensorData);

	///* �����x1 */
	//graphAccelerationX.render();
	//graphAccelerationY.render();
	//graphAccelerationZ.render();
	//graphAccelerationXYZ.render();

	///* �W���C�� */
	//graphGyroX.render();
	//graphGyroY.render();
	//graphGyroZ.render();

	//printf("Ang: %03.02f\n", latestProcessedSensorData.orientationXY);

	/* �R���p�X */
	compassOrientationVisualizer.render(0.0f, 0.0f, latestProcessedSensorData.orientationXY);
} // render
