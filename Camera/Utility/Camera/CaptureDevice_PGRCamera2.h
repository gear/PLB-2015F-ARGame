/****************************************************************
		CaptureDevice_PGRCamera.h.h		FlyCaptureカメラクラス(Flycapture SDK Ver.2対応版)
		概要:
			FlyCaptureカメラを制御するクラス。
			基本的にカスタムイメージモードで動かす。
		ToDo: カラー現像処理はまだ未完成なので入れる
		2014/06/11 by 佐藤俊樹@UECISMS
 ****************************************************************/
#ifndef __CAPTUREDEVICE_FLYCAPTURECAM2_H__
#define __CAPTUREDEVICE_FLYCAPTURECAM2_H__

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "CaptureDevice.h"

#include <FlyCapture2.h>
using namespace FlyCapture2;

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

/* カスタムイメージのパラメータ */
typedef struct{
	unsigned int uiMode;
	unsigned int uiImagePosLeft;		// left
	unsigned int uiImagePosTop;			// top
	unsigned int uiWidth;						// width
	unsigned int uiHeight;					// height
	float fBandWidth;								// speed
	PixelFormat format;							// format
} CUSTOM_IMAGE_PARAMETER;

static const int FLY_DEFAULT_CAMERA_INDEX = 0;

/* デフォルトカスタムイメージパラメータ */
static const int FLY_DEFAULT_VEHICLE = 0;
static const int FLY_DEFAULT_LEFT = 0;
static const int FLY_DEFAULT_WIDTH = 640;
static const int FLY_DEFAULT_HEIGHT = 480;
static const float FLY_DEFAULT_SPEED = 100.0f;

/************************************************
					FlyCaptureカメラクラス
 ************************************************/
class PGRCamera : public CaptureDevice{

public:

	/* コンストラクタ */
	PGRCamera();
	
	/* デストラクタ */
	virtual ~PGRCamera();
	
	/* グラブしたフレームデータのポインタをそのままIPLイメージヘッダにくっつけて渡す */
	virtual Mat *grabImage();

	/* 与えられたイメージデータのimageDataポインタにフレームのポインタを書き込む */
	virtual void grabImage(Mat &imageHeader);
		
	/************************************
		Custom Image (Region of Interest)
	 ************************************/

	/* カスタムイメージパラメータを全部設定する。
		 ROIのサイズは設定できる値にカメラ毎に制限があるのでFlyCap.exeのCustomMode(Format7)の項目かマニュアルを参照 */
	virtual int setCustomImageParameter( int x, int y, int width, int height, PixelFormat format, Mode mode = MODE_0 );

	/**********************************
						初期化 / 終了
	 **********************************/

	/* 初期化 */
	virtual int initialize();

	/* 初期化 */
	virtual int initialize(int x, int y, int width, int height, PixelFormat format);

	/* 終了 */
	virtual void finalize();

	/* カメラの再スタート */
	virtual void restartCamera();

	/***********************************
						開始 / 停止
	 ***********************************/

	/* 開始 */
	virtual int startCamera();

	/* 停止 */
	virtual int stopCamera();

	/****************************************
										設定
	 ****************************************/

	/* カメラインデックスをセットする */
	virtual void setCameraIndex(int index);

	/****************************************
						カメラパラメータ設定
		(Flycap.exeでAuto設定をオフにしておく必要がある)
	 ****************************************/

	/* ゲイン */
	void setGain(float gainValue);

	/* ブライトネス */
	void setBrightness(float brightnessValue);

	/* エクスポージャ */
	void setExposure(float exposureValue);

	/* ガンマ */
	void setGamma(float gammaValue);

	/* シャッタースピード */
	void setShutterSpeed(float shutterValue);

	/* フレームレート */
	void setFrameRate(float frameRateValue);

	/* ROIを取得 */
	virtual CvRect getROI();

	/* Image Size/Offset Units */
	virtual int getImageSizeUnit();
	virtual int getImageOffsetUnit();

	/* width/height */
	virtual int getMaxCameraWidth();
	virtual int getMaxCameraHeight();

	/****************************************
						カラープロセス
	 ****************************************/
	virtual void setColorProcessing();

protected:

	Error error;

	PGRGuid guid;

	BusManager busManager;

	Camera camera;
	CameraInfo camInfo;

	Image rawImage;    
	Image convertedImage;

	Property frmRate;

	unsigned int numCameras;

	CvRect roiRect;

	Mode format7Mode;
	Format7Info format7Info;
	Format7ImageSettings format7ImageSettings;
	Format7PacketInfo format7PacketInfo;
  PixelFormat pixelFormat;

	bool supported;

 	Mat *tmpImageHeader;

	int enableColorProcessing;
	int isCustomParameterInitialized;
  int isCameraStarted;											// キャプチャが開始されているか
	int cameraIndex;

	CUSTOM_IMAGE_PARAMETER customImageParam;	// Custom Imageの設定

};
#endif