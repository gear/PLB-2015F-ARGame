/****************************************************************
		CaptureDevice_FireflyMV.h.h		FlyCaptureカメラクラス(Flycapture SDK Ver.2対応版)
		カメラ: Firefly MV(752x480@60fps)
		Date:
			Create	2014/06/11 By 佐藤俊樹
 ****************************************************************/
#ifndef __CAPTUREDEVICE_FLYCAPTURECAM2_FIREFLYMV_H__
#define __CAPTUREDEVICE_FLYCAPTURECAM2_FIREFLYMV_H__

#include "CaptureDevice_PGRCamera2.h"

/************************************************
					FireflyMVクラス
 ************************************************/
class FireflyMV : public PGRCamera{

public:

	/******************************************
								Singletonメソッド
	 ******************************************/

	/* Singletonインスタンス生成 */
	static FireflyMV *getInstance();

	/* Singletonインスタンス破棄 */
	static void releaseInstance();

	/* width/height */
	virtual int getMaxCameraWidth();

private:

	/***************************
	  シングルトンインスタンス
	***************************/
	static FireflyMV *instance;

	/* コンストラクタ */
	FireflyMV();
	
	/* デストラクタ */
	virtual ~FireflyMV();
	
};
#endif