#include "SensorDataReceiver.h"

RawSensorDataReceiver::RawSensorDataReceiver(){
	timer = Timer::getInstance();
}

RawSensorDataReceiver::~RawSensorDataReceiver(){}

/* 初期化 */
int RawSensorDataReceiver::initialize(const char *configFileName, unsigned int rs232cReceiveLoopInterval){

	this->configFileName = configFileName;

	/* スレッド作成 */
	mutex = SDL_CreateMutex();
	flagExit = 0;
	sensorDataReceiveThread = SDL_CreateThread(receiveSensorDataThreadFunction, (void*)this); //スレッドを実行

	return 0;	
} // initialize

/* 開放 */
void RawSensorDataReceiver::finalize(){

	/* 終了フラグを更新 */
	while ( !SDL_LockMutex(mutex) ) {
		flagExit = 1;
		SDL_UnlockMutex(mutex);
		break;
	}

	SDL_WaitThread(sensorDataReceiveThread, NULL); //スレッド処理終了を待つ

	SDL_DestroyMutex(mutex);
} // finalize

/* 受信処理 */
int RawSensorDataReceiver::receiveSensorDataThreadFunction(void *ptr){ 

	char receiveBuffer[128];			// 十分なサイズ
	char headerCharacter;

	RawSensorDataReceiver *thisPtr = (RawSensorDataReceiver*)ptr;
	unsigned int rs232cReceiveLoopInterval = thisPtr->rs232cReceiveLoopInterval;
	ULONGLONG illegalPacketCounter = 0;

	/* RS232C設定の読み込み */
	fstream rs232cConfigFile(thisPtr->configFileName, ios::in);
	if ( rs232cConfigFile.fail() ){
		printf("RS232C設定ファイルが読み込めませんでした: %s", thisPtr->configFileName);
		return -1;
	}
	char lineBuffer[128];
	char portName[32];
	int baudRate;
	int writeTimeOut = 0, readTimeOut = 0;
	rs232cConfigFile.getline(lineBuffer, 128);
	_snscanf(lineBuffer, 128, "%d %s %d %d", &baudRate, portName, &readTimeOut, &writeTimeOut);

	/* RS232C通信クラス */
	RS232c *rs232c = new RS232c();
	if ( rs232c->connect(portName, baudRate, 512, 512, readTimeOut, writeTimeOut) < 0 ){
		printf("シリアル通信の初期化に失敗しました。\n");
		return -1;
	}else{
		printf("シリアル通信を開始しました: %s @ %dbps\n", portName, baudRate);	
	}

	/* コンパスデータ処理クラス */
//	CompassCalibrator compassCalibrator;

	/***********************************************************
													受信ループ
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

		/* 終了フラグ確認 */
		while ( !SDL_LockMutex(thisPtr->mutex) ) {
			if ( thisPtr->flagExit ){
				SDL_UnlockMutex(thisPtr->mutex);

				/* 終了処理 */
				printf("受信に失敗したパケットは%llu個ありました。\n", illegalPacketCounter);
				rs232c->disconnect();
				return 0;
			}
			SDL_UnlockMutex(thisPtr->mutex);
			break;
		}

		/* 1文字ずつ受信し、頭文字'B'を探す */
		do {	
			receivedSize = rs232c->receive(&headerCharacter, 1);
		} while ( headerCharacter != 'B' );
	
		/* 残りを受信する */
		receivedSize = rs232c->receive(receiveBuffer, PACKET_SIZE_WITHOUT_HEADER);

		/* お尻に'E'があれば正しく受信できているとみなし、構造体に変換 */
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

				/* センサデータの計算 */
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

	return 0;		// ここには来ない
} // receiveSensorDataThreadFunction

/* ボールデータの受け取り */
void RawSensorDataReceiver::getSensorData(RawSensorData &dstSensroData, ProcessedSensorData &dstProcessedSensorData){
	while ( !SDL_LockMutex(mutex) ) {
		dstSensroData = rawSensorData;
		dstProcessedSensorData = processedSensorData;
		SDL_UnlockMutex(mutex);
		return;
	}
} // getSensorData
