/**************************************************************************
		�Z���T�f�[�^�ʐM�N���X
		Created 2015/03/25 by �����r��
		�T�v:
			�V���A���ʐM�ŃZ���T�f�[�^�̎擾���s���B
 **************************************************************************/
#pragma once

#include <stdlib.h>
#include <math.h>
#include <fstream>
using namespace std;

#include "../RS232C/rs232c.h"
#include "../Timer/Timer.h"

#include "../../Definitions.h"

/* �R���p�X���ʌv�Z */
#include "../CompassDataCalculator/CompassDataCalculator.h"

/* �R���p�X�L�����u���[�V���� */
#include "../CompassDataCalculator/CompassCalibrator.h"

class RawSensorDataReceiver{

public:

	RawSensorDataReceiver();
	virtual ~RawSensorDataReceiver();

	/* ������ */
	int initialize(const char *configFileName, unsigned int rs232cReceiveLoopInterval);

	/* �J�� */
	void finalize();

	/* ���M���� */
	static int receiveSensorDataThreadFunction(void *ptr);

	/* �{�[���f�[�^�̎󂯎�� */
	void getSensorData(RawSensorData &dstSensroData, ProcessedSensorData &dstProcessedSensorData);

protected:

	Timer *timer;

	SDL_Thread *sensorDataReceiveThread;
	SDL_mutex *mutex;
	unsigned int rs232cReceiveLoopInterval;

	/* ����3�̕ϐ���RS232C��M�X���b�h���疈���[�v�X�V�E�Q�Ƃ����̂ŁA�A�N�Z�X����mutex�����b�N���邱�� */
	RawSensorData rawSensorData;
	ProcessedSensorData processedSensorData;
	bool flagExit;

	const char *configFileName;

};
