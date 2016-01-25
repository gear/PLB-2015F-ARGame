/**************************************************************************
		センサデータ通信クラス
		Created 2015/03/25 by 佐藤俊樹
		概要:
			シリアル通信でセンサデータの取得を行う。
 **************************************************************************/
#pragma once

#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

#include "../RS232C/rs232c.h"
#include "../Timer/Timer.h"

#include "../../Definitions.h"

/* コンパス方位計算 */
#include "../CompassDataCalculator/CompassDataCalculator.h"

/* コンパスキャリブレーション */
#include "../CompassDataCalculator/CompassCalibrator.h"

class RawSensorDataReceiver{

public:

	RawSensorDataReceiver();
	virtual ~RawSensorDataReceiver();

	/* 初期化 */
	int initialize(const char *configFileName, unsigned int rs232cReceiveLoopInterval);

	/* 開放 */
	void finalize();

	/* 送信処理 */
	static int receiveSensorDataThreadFunction(void *ptr);

	/* ボールデータの受け取り */
	void getSensorData(RawSensorData &dstSensroData, ProcessedSensorData &dstProcessedSensorData);

protected:

	Timer *timer;

	SDL_Thread *sensorDataReceiveThread;
	SDL_mutex *mutex;
	unsigned int rs232cReceiveLoopInterval;

	/* この3つの変数はRS232C受信スレッドから毎ループ更新・参照されるので、アクセス時はmutexをロックすること */
	RawSensorData rawSensorData;
	ProcessedSensorData processedSensorData;
	bool flagExit;

	const char *configFileName;

};
