#include "CaptureDevice_PGRCamera2_FireflyMV.h"

/* Singletonインスタンスと初期化フラグ */
FireflyMV *FireflyMV::instance = NULL;

/******************************************
							Singletonメソッド
 ******************************************/

/* Singletonインスタンス生成 */
FireflyMV *FireflyMV::getInstance(){
	if ( instance ){
		return instance;
	}else{
		instance = new FireflyMV();
		return instance;
	}
} // bb2GetInstance

/* Singletonインスタンス破棄 */
void FireflyMV::releaseInstance(){
	if ( instance ){
		delete instance;
		instance = NULL;
	}
} // bb2ReleaseInstance

/* コンストラクタ */
FireflyMV::FireflyMV() : PGRCamera(){
	cameraIndex = 0;
	isCameraStarted = 0;
	isCustomParameterInitialized = 0;
	tmpImageHeader = NULL;
} // コンストラクタ

/* デストラクタ */
FireflyMV::~FireflyMV(){
} // デストラクタ

int FireflyMV::getMaxCameraWidth(){
	return 752;
} // getMaxCameraWidth
