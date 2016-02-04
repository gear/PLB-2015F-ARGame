#include "NewOhajiki2DShooting_NetworkManager.h"

/* �R���X�g���N�^ */
NewOhajiki2DShootingNetworkManager::NewOhajiki2DShootingNetworkManager(int maxData, int numMaxShapes, CvRect roiRect, int applicationWidth, int applicationHeight){
	AGXNetwork *network = AGXNetwork::axnGetInstance();
	network->axnInitialize();

	/* �z�X�g�����t�@�C�����瓾�� */
	loadHostFromFile("./data/conf/applicationHost.conf");
	sessionSendReognitionData = network->axnCreateUDPClientSession(hostNameFromFile, portNoFromFile);
	
	sessionSendObjectReognitionData = network->axnCreateUDPClientSession(NEWOHAJIKI2DSHOOTING_APPLICATION_IP_ADDRESS, NEWOHAJIKI2DSHOOTING_APPLICATION_SHAPE_DATA_PORT_NO);
	sessionReceiveApplicationData = network->axnCreateUDPServerSession(NEWOHAJIKI2DSHOOTING_RECEIVE_APPLICATION_PORT_NO);

	transformMatrix = NULL;
	transformMatrixInv = NULL;
	halfWidth = applicationWidth / 2.0f;
	halfHeight = applicationHeight / 2.0f;
	this->roiRect = roiRect;
	this->handDataSendBufferArraySize = maxData;
	this->shapeDataSendBufferArraySize = numMaxShapes;
	handDataSendBuffer = new NEWOHAJIKI2DSHOOTING_NETWORK_HAND_DATA[handDataSendBufferArraySize];
	shapeDataSendBuffer = new NEWOHAJIKI2DSHOOTING_NETWORK_SHAPE_DATA[shapeDataSendBufferArraySize];

	recognitionModeReceiveBuffer.mode = ARM_RECOGNITION_HAND;
	loadCalibrationMatrix();
} // �R���X�g���N�^

/* �f�X�g���N�^ */
NewOhajiki2DShootingNetworkManager::~NewOhajiki2DShootingNetworkManager(){
	transformMatrix = NULL;
	transformMatrixInv = NULL;
	delete sessionReceiveApplicationData;
	delete sessionSendObjectReognitionData;
	delete sessionSendReognitionData;
	delete [] shapeDataSendBuffer;
	delete [] handDataSendBuffer;
} // �f�X�g���N�^

/*******************************************
								��f�[�^���M
 *******************************************/

/* �F���f�[�^�̑��M */
int NewOhajiki2DShootingNetworkManager::netSendRecognitionData( NEWOHAJIKI2DSHOOTING_RECOGNITION_DATA *recognitionDataArray ){
	CvPoint2D32f tmpPoint;
	for ( int i = 0; i < handDataSendBufferArraySize; i++ ){
		if ( recognitionDataArray[i].id ){

			handDataSendBuffer[i].id = recognitionDataArray[i].id;
			handDataSendBuffer[i].handState = recognitionDataArray[i].handState;

			tmpPoint.x = recognitionDataArray[i].handPosition.x + roiRect.x;
			tmpPoint.y = recognitionDataArray[i].handPosition.y + roiRect.y;
			handDataSendBuffer[i].transformedHandPosition = transformer.ptPerspectiveTransformation32F(tmpPoint, transformMatrix);
			
			tmpPoint.x = recognitionDataArray[i].armPosition.x + roiRect.x;
			tmpPoint.y = recognitionDataArray[i].armPosition.y + roiRect.y;
			handDataSendBuffer[i].transformedArmPosition = transformer.ptPerspectiveTransformation32F(tmpPoint, transformMatrix);

			handDataSendBuffer[i].transformedHandPosition.x = (handDataSendBuffer[i].transformedHandPosition.x - halfWidth);
			handDataSendBuffer[i].transformedHandPosition.y = (-handDataSendBuffer[i].transformedHandPosition.y + halfHeight);
			handDataSendBuffer[i].transformedArmPosition.x = (handDataSendBuffer[i].transformedArmPosition.x - halfWidth);
			handDataSendBuffer[i].transformedArmPosition.y = (-handDataSendBuffer[i].transformedArmPosition.y + halfHeight);
			handDataSendBuffer[i].circleSize = recognitionDataArray[i].circleSize;
			handDataSendBuffer[i].armSize = recognitionDataArray[i].armSize;

		}else{
			handDataSendBuffer[i].id = 0;
		}
	}

	/* ���M */
	return 	sessionSendReognitionData->axnSendMessage( handDataSendBuffer, sizeof( NEWOHAJIKI2DSHOOTING_NETWORK_HAND_DATA ) * handDataSendBufferArraySize );
} // netSendRecognitionData

/*******************************************
							�`��f�[�^���M
 *******************************************/

/* �F���f�[�^�̑��M */
int NewOhajiki2DShootingNetworkManager::netSendShapeRecognitionData( NEWOHAJIKI2DSHOOTING_NETWORK_SHAPE_DATA *srcShapeData, int numShapeDetected ){

	for ( int i = 0; i < numShapeDetected; i++ ){

		shapeDataSendBuffer[i].numLinePoints = srcShapeData[i].numLinePoints;

		/* ���W�ϊ� */
		shapeDataSendBuffer[i].centroid = transformer.ptPerspectiveTransformation32F(srcShapeData[i].centroid, transformMatrix);
		shapeDataSendBuffer[i].centroid.x = shapeDataSendBuffer[i].centroid.x - halfWidth;
		shapeDataSendBuffer[i].centroid.y = -shapeDataSendBuffer[i].centroid.y + halfHeight;

		for (unsigned int j = 0; j < srcShapeData[i].numLinePoints; j++){
			shapeDataSendBuffer[i].linePoints[j] = transformer.ptPerspectiveTransformation32F(srcShapeData[i].linePoints[j], transformMatrix);
			shapeDataSendBuffer[i].linePoints[j].x = (shapeDataSendBuffer[i].linePoints[j].x - halfWidth);
			shapeDataSendBuffer[i].linePoints[j].y = (-shapeDataSendBuffer[i].linePoints[j].y + halfHeight);
		}
	} // for

	return 	sessionSendObjectReognitionData->axnSendMessage( shapeDataSendBuffer, sizeof( NEWOHAJIKI2DSHOOTING_NETWORK_SHAPE_DATA ) * shapeDataSendBufferArraySize );
} // netSendShapeRecognitionData

/**************************************************
								�F�����[�h�̎�M
 **************************************************/

/* �F���f�[�^�̓����̃`�F�b�N */
AGESHOO_RECOGNITION_MODE NewOhajiki2DShootingNetworkManager::netCheckApplicationData(){
	if ( sessionReceiveApplicationData->axnReceiveMessage( &recognitionModeReceiveBuffer, sizeof( AGESHOO_NETWORK_RECOGNITION_MODE_DATA ) ) ){
		return recognitionModeReceiveBuffer.mode;
	}else{
		return ARM_RECOGNITION_UNKNOWN;			// ��M���Ȃ�������ARM_RECOGNITION_UNKNOWN���������Ƃ�
	}
} // netCheckApplicationData

/* �L�����u���[�V�����f�[�^�̓ǂݍ��� */
void NewOhajiki2DShootingNetworkManager::loadCalibrationMatrix(){
	if ( transformMatrix ){
		transformer.ptReleasePerspectiveTransformationMatrix(transformMatrix);
	}
	transformMatrix = transformer.ptCreatePerspectiveTransformationMatrix3x3FromFile(NEWOHAJIKI2DSHOOTING_TRANSFORM_MATRIX_FILE);

	if ( transformMatrixInv ){
		transformer.ptReleasePerspectiveTransformationMatrix(transformMatrixInv);
	}
	transformMatrixInv = transformer.ptCreatePerspectiveTransformationMatrix3x3FromFile(NEWOHAJIKI2DSHOOTING_TRANSFORM_INVERSE_MATRIX_FILE);
} // loadCalibrationMatrix


/* �z�X�g�����t�@�C������ǂݍ��� */
void NewOhajiki2DShootingNetworkManager::loadHostFromFile(const char *filename){
	fstream hostFile;
	char lineBuffer[128];
	hostFile.open(filename, ios::in);
	if ( !hostFile.fail() ){
	
		hostFile.getline(lineBuffer, 128);
		sscanf(lineBuffer, "%s", hostNameFromFile);
		hostFile.getline(lineBuffer, 128);
		sscanf(lineBuffer, "%u", &portNoFromFile);
		hostFile.close();

	}else{
		printf("�A�v���P�[�V�����z�X�g�t�@�C��: %s��������܂���ł���(�f�t�H���g�l���g���܂�)�B\n", filename);
		sscanf(NEWOHAJIKI2DSHOOTING_APPLICATION_IP_ADDRESS, "%s", hostNameFromFile);
		portNoFromFile = NEWOHAJIKI2DSHOOTING_APPLICATION_HAND_DATA_PORT_NO;
	}

	printf("ApplicationHost: %s:%u\n", hostNameFromFile, portNoFromFile);

} // loadHostName