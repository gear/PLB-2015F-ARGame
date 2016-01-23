/******************************************************************************
		NetworkManager.h			GelRecognitionNetManagerクラス
		概要:
			おはじき2Dシューティング用の新しいネットワーク通信クラス
		Date:
			Create 2008/10/31	By 佐藤俊樹@IS研究科小池研究室
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

/* 相手のIPアドレス/ポート番号 */
static char *APPLICATION_IP_ADDRESS = "localhost";
static const int APPLICATION_PORT_NO = 31416;

class NewOhajiki2DShootingNetworkManager{

public:

	/* コンストラクタ */
	NewOhajiki2DShootingNetworkManager(int maxData, int numMaxShapes, CvRect roiRect, int applicationWidth, int applicationHeight);

	/* デストラクタ */
	virtual ~NewOhajiki2DShootingNetworkManager();

	/*******************************************
									手データ送信
	 *******************************************/

	/* 認識データの送信 */
	virtual int sendData( RECOGNITION_DATA *recognitionDataArray, int num );

	/**************************************************
									認識モードの受信
	 **************************************************/

	/* 認識データの到着のチェック */
	virtual AGESHOO_RECOGNITION_MODE netCheckApplicationData();

protected:

	CvRect roiRect;
	AGXUDPClientSession *sessionSendReognitionData, *sessionSendObjectReognitionData;
	AGXUDPServerSession *sessionReceiveApplicationData;

	int handDataSendBufferArraySize, shapeDataSendBufferArraySize;

	PerspectiveTransformer transformer;
	CvMat *transformMatrix, *transformMatrixInv;

	/* 送信バッファ */
	NETWORK_HAND_DATA *handDataSendBuffer;
	NETWORK_SHAPE_DATA *shapeDataSendBuffer;

	/* 受信バッファ */
	AGESHOO_NETWORK_RECOGNITION_MODE_DATA recognitionModeReceiveBuffer;

	float halfWidth, halfHeight;

	/* キャリブレーションデータの読み込み */
	virtual void loadCalibrationMatrix();

	/* ホスト名をファイルから読み込む */
	char hostNameFromFile[128];
	int portNoFromFile;
	virtual void loadHostFromFile(const char *filename);

};
#endif