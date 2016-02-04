/*****************************************************************
		2値化クラス
		2014/06/11 by 佐藤俊樹@UECISMS
 *****************************************************************/

#ifndef __BINARIZER_H__
#define __BINARIZER_H__

#include <opencv2/opencv.hpp>
using namespace cv;

class Binarizer{

public:

	Binarizer();

	virtual ~Binarizer();

	/* 初期化 */
	virtual int initialize( int width, int height );

	/* 閾値で2値化 */
	virtual void calcThreshold(Mat &srcImageGRAY, Mat &dstImageGRAY, double thresholdValue);

	/* 背景差分 */
	virtual void calcBackgroundSubtraction( Mat &srcImageGRAY, Mat &dstBinarizedImage, Mat &backgroundImageGRAY, double thresholdValue );

protected:

	Mat *tmpImage;

};

#endif