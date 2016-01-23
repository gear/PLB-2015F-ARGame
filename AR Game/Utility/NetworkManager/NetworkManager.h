/******************************************************************************
		NetworkManager.h			GelRecognitionNetManager�N���X
		�T�v:
			���͂���2D�V���[�e�B���O�p�̐V�����l�b�g���[�N�ʐM�N���X
		Date:
			Create 2008/10/31	By �����r��@IS�����ȏ��r������
 *******************************************************************************/
#ifndef __NETWORKMANAGER_H__
#define __NETWORKMANAGER_H__

#include <stdlib.h>
#include <stdio.h>

#include "../vis/PerspectiveTransformer.h"
#include "../net/AGX_Network.h"
#include "../Definition_DragonflyExpressDemo.h"

#include <vector>
using namespace std;

#include "../vis/RegionExtractor2.h"

/* �����IP�A�h���X/�|�[�g�ԍ� */
static char *APPLICATION_IP_ADDRESS = "localhost";
static const int APPLICATION_PORT_NO = 31416;

class NewOhajiki2DShootingNetworkManager{

public:

	/* �R���X�g���N�^ */
	NewOhajiki2DShootingNetworkManager(int maxData, int numMaxShapes, CvRect roiRect, int applicationWidth, int applicationHeight);

	/* �f�X�g���N�^ */
	virtual ~NewOhajiki2DShootingNetworkManager();

	/*******************************************
									��f�[�^���M
	 *******************************************/

	/* �F���f�[�^�̑��M */
	virtual int sendData( RECOGNITION_DATA *recognitionDataArray, int num );

	/**************************************************
									�F�����[�h�̎�M
	 **************************************************/

	/* �F���f�[�^�̓����̃`�F�b�N */
	virtual AGESHOO_RECOGNITION_MODE netCheckApplicationData();

protected:

	CvRect roiRect;
	AGXUDPClientSession *sessionSendReognitionData, *sessionSendObjectReognitionData;
	AGXUDPServerSession *sessionReceiveApplicationData;

	int handDataSendBufferArraySize, shapeDataSendBufferArraySize;

	PerspectiveTransformer transformer;
	CvMat *transformMatrix, *transformMatrixInv;

	/* ���M�o�b�t�@ */
	NETWORK_HAND_DATA *handDataSendBuffer;
	NETWORK_SHAPE_DATA *shapeDataSendBuffer;

	/* ��M�o�b�t�@ */
	AGESHOO_NETWORK_RECOGNITION_MODE_DATA recognitionModeReceiveBuffer;

	float halfWidth, halfHeight;

	/* �L�����u���[�V�����f�[�^�̓ǂݍ��� */
	virtual void loadCalibrationMatrix();

	/* �z�X�g�����t�@�C������ǂݍ��� */
	char hostNameFromFile[128];
	int portNoFromFile;
	virtual void loadHostFromFile(const char *filename);

};
#endif