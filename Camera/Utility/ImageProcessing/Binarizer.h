/*****************************************************************
		2�l���N���X
		2014/06/11 by �����r��@UECISMS
 *****************************************************************/

#ifndef __BINARIZER_H__
#define __BINARIZER_H__

#include <opencv2/opencv.hpp>
using namespace cv;

class Binarizer{

public:

	Binarizer();

	virtual ~Binarizer();

	/* ������ */
	virtual int initialize( int width, int height );

	/* 臒l��2�l�� */
	virtual void calcThreshold(Mat &srcImageGRAY, Mat &dstImageGRAY, double thresholdValue);

	/* �w�i���� */
	virtual void calcBackgroundSubtraction( Mat &srcImageGRAY, Mat &dstBinarizedImage, Mat &backgroundImageGRAY, double thresholdValue );

protected:

	Mat *tmpImage;

};

#endif