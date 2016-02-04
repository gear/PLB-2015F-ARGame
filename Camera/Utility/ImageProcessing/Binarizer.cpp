#include "Binarizer.h"

Binarizer::Binarizer(){
	tmpImage = NULL;
}

Binarizer::~Binarizer(){
	delete tmpImage;
}

/* ������ */
int Binarizer::initialize( int width, int height ){
	if ( tmpImage ){
		delete tmpImage;
		tmpImage = new Mat(height, width, CV_8UC1);
	}else{
		tmpImage = new Mat(height, width, CV_8UC1);
	}
	return 0;
} // initialize

/* 臒l��2�l�� */
void Binarizer::calcThreshold(Mat &srcImageGRAY, Mat &dstImageGRAY, double thresholdValue){
	threshold(srcImageGRAY, dstImageGRAY, thresholdValue, 255.0, THRESH_BINARY);
} // calcThresohld

/* �w�i���� */
void Binarizer::calcBackgroundSubtraction( Mat &srcImageGRAY, Mat &dstBinarizedImage, Mat &backgroundImageGRAY, double thresholdValue ){
	absdiff(srcImageGRAY, backgroundImageGRAY, *tmpImage);
	threshold(*tmpImage, dstBinarizedImage, thresholdValue, 255.0, THRESH_BINARY);
} // calcBackgroundSubtraction
