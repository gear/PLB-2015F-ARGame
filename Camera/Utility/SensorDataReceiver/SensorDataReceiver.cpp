#include "SensorDataReceiver.h"

RawSensorDataReceiver::RawSensorDataReceiver(){
	timer = Timer::getInstance();
}

RawSensorDataReceiver::~RawSensorDataReceiver(){}

/* ������ */
int RawSensorDataReceiver::initialize(const char *configFileName, unsigned int rs232cReceiveLoopInterval){

	this->configFileName = configFileName;

	/* �X���b�h�쐬 */
	mutex = SDL_CreateMutex();
	flagExit = 0;
	sensorDataReceiveThread = SDL_CreateThread(receiveSensorDataThreadFunction, (void*)this); //�X���b�h�����s

	return 0;	
} // initialize

/* �J�� */
void RawSensorDataReceiver::finalize(){

	/* �I���t���O���X�V */
	while ( !SDL_LockMutex(mutex) ) {
		flagExit = 1;
		SDL_UnlockMutex(mutex);
		break;
	}

	SDL_WaitThread(sensorDataReceiveThread, NULL); //�X���b�h�����I����҂�

	SDL_DestroyMutex(mutex);
} // finalize

/* ��M���� */
int RawSensorDataReceiver::receiveSensorDataThreadFunction(void *ptr){ 

	char receiveBuffer[128];			// �\���ȃT�C�Y
	char headerCharacter;

	RawSensorDataReceiver *thisPtr = (RawSensorDataReceiver*)ptr;
	unsigned int rs232cReceiveLoopInterval = thisPtr->rs232cReceiveLoopInterval;
	ULONGLONG illegalPacketCounter = 0;

	/* RS232C�ݒ�̓ǂݍ��� */
	fstream rs232cConfigFile(thisPtr->configFileName, ios::in);
	if ( rs232cConfigFile.fail() ){
		printf("RS232C�ݒ�t�@�C�����ǂݍ��߂܂���ł���: %s", thisPtr->configFileName);
		return -1;
	}
	char lineBuffer[128];
	char portName[32];
	int baudRate;
	int writeTimeOut = 0, readTimeOut = 0;
	rs232cConfigFile.getline(lineBuffer, 128);
	_snscanf(lineBuffer, 128, "%d %s %d %d", &baudRate, portName, &readTimeOut, &writeTimeOut);

	/* RS232C�ʐM�N���X */
	RS232c *rs232c = new RS232c();
	if ( rs232c->connect(portName, baudRate, 512, 512, readTimeOut, writeTimeOut) < 0 ){
		printf("�V���A���ʐM�̏������Ɏ��s���܂����B\n");
		return -1;
	}else{
		printf("�V���A���ʐM���J�n���܂���: %s @ %dbps\n", portName, baudRate);	
	}

	/* �R���p�X�f�[�^�����N���X */
//	CompassCalibrator compassCalibrator;

	/***********************************************************
													��M���[�v
	 ***********************************************************/
	int retryCounter = 0;
	int receivedSize = 0;
	unsigned int frameCounter = 0;
	unsigned int tick = 0;
	while (1){

		frameCounter++;
		if ( tick + 1000 < SDL_GetTicks() ){
			printf("S:%ufps\n", frameCounter);
			tick = SDL_GetTicks();
			frameCounter = 0;
		}

		SDL_Delay(rs232cReceiveLoopInterval);

		/* �I���t���O�m�F */
		while ( !SDL_LockMutex(thisPtr->mutex) ) {
			if ( thisPtr->flagExit ){
				SDL_UnlockMutex(thisPtr->mutex);

				/* �I������ */
				printf("��M�Ɏ��s�����p�P�b�g��%llu����܂����B\n", illegalPacketCounter);
				rs232c->disconnect();
				return 0;
			}
			SDL_UnlockMutex(thisPtr->mutex);
			break;
		}

		/* 1��������M���A������'B'��T�� */
		do {	
			receivedSize = rs232c->receive(&headerCharacter, 1);
		} while ( headerCharacter != 'B' );
	
		/* �c�����M���� */
		receivedSize = rs232c->receive(receiveBuffer, PACKET_SIZE_WITHOUT_HEADER);

		/* ���K��'E'������ΐ�������M�ł��Ă���Ƃ݂Ȃ��A�\���̂ɕϊ� */
		if ( receiveBuffer[PACKET_SIZE_WITHOUT_HEADER - 1] == 'E' ){

			while ( !SDL_LockMutex(thisPtr->mutex) ) {

				int buffer = 0;
				char *headPtr = (char*)&buffer;

				headPtr[3] = receiveBuffer[1];
				headPtr[2] = receiveBuffer[0];		
				thisPtr->rawSensorData.gyro.x = (float)(buffer >> 16);
				headPtr[3] = receiveBuffer[3];
				headPtr[2] = receiveBuffer[2];		
				thisPtr->rawSensorData.gyro.y = (float)(buffer >> 16);
				headPtr[3] = receiveBuffer[5];
				headPtr[2] = receiveBuffer[4];		
				thisPtr->rawSensorData.gyro.z = (float)(buffer >> 16);

				headPtr[3] = receiveBuffer[7];
				headPtr[2] = receiveBuffer[6];		
				thisPtr->rawSensorData.acceleration.x = (float)(buffer >> 16);
				headPtr[3] = receiveBuffer[9];
				headPtr[2] = receiveBuffer[8];		
				thisPtr->rawSensorData.acceleration.y = (float)(buffer >> 16);
				headPtr[3] = receiveBuffer[11];
				headPtr[2] = receiveBuffer[10];		
				thisPtr->rawSensorData.acceleration.z = (float)(buffer >> 16);

				headPtr[3] = receiveBuffer[13];
				headPtr[2] = receiveBuffer[12];
				thisPtr->rawSensorData.compass.x = (float)(buffer >> 16);
				headPtr[3] = receiveBuffer[15];
				headPtr[2] = receiveBuffer[14];
				thisPtr->rawSensorData.compass.y = (float)(buffer >> 16);
				headPtr[3] = receiveBuffer[17];
				headPtr[2] = receiveBuffer[16];
				thisPtr->rawSensorData.compass.z = (float)(buffer >> 16);

				headPtr[3] = receiveBuffer[21];
				headPtr[2] = receiveBuffer[20];
				headPtr[1] = receiveBuffer[19];
				headPtr[0] = receiveBuffer[18];
				thisPtr->rawSensorData.location.latitude = (float)buffer * 0.0000001f;
				//if ( buffer != 0 ){
				//	printf("Lat: %x %x %x %x = %X\n", receiveBuffer[18], receiveBuffer[19], receiveBuffer[20], receiveBuffer[21], buffer);
				//}
				headPtr[3] = receiveBuffer[25];
				headPtr[2] = receiveBuffer[24];		
				headPtr[1] = receiveBuffer[23];
				headPtr[0] = receiveBuffer[22];		
				thisPtr->rawSensorData.location.longitude = (float)buffer * 0.0000001f;

				thisPtr->rawSensorData.numSatellites = (unsigned)receiveBuffer[26];

				/* �Z���T�f�[�^�̌v�Z */
				//compassCalibrator.updateCalibrationOffset(thisPtr->rawSensorData.compass);
//				thisPtr->processedSensorData.orientationXY = CompassDataCalculator::calcOrientationXY(thisPtr->rawSensorData.compass, compassCalibrator.getCalibrationOffset());

				SDL_UnlockMutex(thisPtr->mutex);
				break;
			}

		}else{
			illegalPacketCounter++;
			printf("Irregular Packet is Skipped\n");
		}

	} // while

	return 0;		// �����ɂ͗��Ȃ�
} // receiveSensorDataThreadFunction

/* �{�[���f�[�^�̎󂯎�� */
void RawSensorDataReceiver::getSensorData(RawSensorData &dstSensroData, ProcessedSensorData &dstProcessedSensorData){
	while ( !SDL_LockMutex(mutex) ) {
		dstSensroData = rawSensorData;
		dstProcessedSensorData = processedSensorData;
		SDL_UnlockMutex(mutex);
		return;
	}
} // getSensorData
