/********************************************
			キャプチャデバイス基底クラス
	Date:
			Create 2014/06/11 by 佐藤俊樹@UECISMS
 ********************************************/
#ifndef __CVCAPTUREDEVICE_H__
#define __CVCAPTUREDEVICE_H__

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <opencv2/opencv.hpp>
using namespace cv;

class CaptureDevice{

public:

	/* コンストラクタ */
	CaptureDevice();

	/* デストラクタ */
	virtual ~CaptureDevice();

	/***********************************
					フレームのキャプチャ
	 ***********************************/
	virtual void grabImage(Mat &imageHeader) = 0;		// 与えられたイメージヘッダにデータを追加する
	virtual Mat *grabImage() = 0;										// データ付きのイメージヘッダを返す

	/***********************************
						初期化 / 解放
	 ***********************************/
	virtual int initialize() = 0;
	virtual void finalize() = 0;

	/***********************************
						開始 / 停止
	 ***********************************/
	virtual int startCamera() = 0;
	virtual int stopCamera() = 0;

protected:

	int isInitialized;
	int imageWidth, imageHeight;

};
#endif