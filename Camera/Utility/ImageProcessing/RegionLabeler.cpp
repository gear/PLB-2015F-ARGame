#include "RegionLabeler.h"

RegionLabeler::RegionLabeler(){}

RegionLabeler::~RegionLabeler(){}

/* ���x�����O */
int RegionLabeler::detectRegion(Mat &srcImageGRAY, float maxSizeThreshold, float minSizeThreshold, Vector<Region> &resultRegionArray){
	int regionCounter;
	regionCounter = 0;
	findContours(srcImageGRAY, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

	for (int i = 0; i < contours.size(); i++){
		double size = contourArea(contours[i]);

		/* �ʐςōi�� */
		if ( size < minSizeThreshold || size > maxSizeThreshold){
			continue;
		}

		/* �̈�̌`��������v�Z��Region�z��ɕϊ� */
		Moments tmpMoments;
		tmpMoments = moments(contours[i], 1);
		resultRegionArray[regionCounter].centroid.x = (float)(tmpMoments.m10 / tmpMoments.m00);		// �d�SX
		resultRegionArray[regionCounter].centroid.y = (float)(tmpMoments.m01 / tmpMoments.m00);		// �d�SY
		resultRegionArray[regionCounter].boundingRect = boundingRect(contours[i]);								// �o�E���f�B���O�{�b�N�X
		resultRegionArray[regionCounter].angle = (float)(atan2(
			(tmpMoments.m11 / tmpMoments.m00 - resultRegionArray[regionCounter].centroid.x * resultRegionArray[regionCounter].centroid.y) * 2.0, // �����厲�p
			(tmpMoments.m20 / tmpMoments.m00 - resultRegionArray[regionCounter].centroid.x * resultRegionArray[regionCounter].centroid.x) - (tmpMoments.m02 / tmpMoments.m00 - resultRegionArray[regionCounter].centroid.y * resultRegionArray[regionCounter].centroid.y)) / 2.0);
		resultRegionArray[regionCounter].size = (float)size;
		regionCounter++;

		/* Region�z��̍ő���ɓ��B */
		if ( regionCounter >= resultRegionArray.size() ){
			break;
		}
	}

	return regionCounter;		// ���o����Ԃ�
} // detectRegion