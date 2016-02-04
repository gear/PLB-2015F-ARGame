#include "RegionLabeler.h"

RegionLabeler::RegionLabeler(){}

RegionLabeler::~RegionLabeler(){}

/* ラベリング */
int RegionLabeler::detectRegion(Mat &srcImageGRAY, float maxSizeThreshold, float minSizeThreshold, Vector<Region> &resultRegionArray){
	int regionCounter;
	regionCounter = 0;
	findContours(srcImageGRAY, contours, hierarchy, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE );

	for (int i = 0; i < contours.size(); i++){
		double size = contourArea(contours[i]);

		/* 面積で絞る */
		if ( size < minSizeThreshold || size > maxSizeThreshold){
			continue;
		}

		/* 領域の形状特徴を計算しRegion配列に変換 */
		Moments tmpMoments;
		tmpMoments = moments(contours[i], 1);
		resultRegionArray[regionCounter].centroid.x = (float)(tmpMoments.m10 / tmpMoments.m00);		// 重心X
		resultRegionArray[regionCounter].centroid.y = (float)(tmpMoments.m01 / tmpMoments.m00);		// 重心Y
		resultRegionArray[regionCounter].boundingRect = boundingRect(contours[i]);								// バウンディングボックス
		resultRegionArray[regionCounter].angle = (float)(atan2(
			(tmpMoments.m11 / tmpMoments.m00 - resultRegionArray[regionCounter].centroid.x * resultRegionArray[regionCounter].centroid.y) * 2.0, // 慣性主軸角
			(tmpMoments.m20 / tmpMoments.m00 - resultRegionArray[regionCounter].centroid.x * resultRegionArray[regionCounter].centroid.x) - (tmpMoments.m02 / tmpMoments.m00 - resultRegionArray[regionCounter].centroid.y * resultRegionArray[regionCounter].centroid.y)) / 2.0);
		resultRegionArray[regionCounter].size = (float)size;
		regionCounter++;

		/* Region配列の最大個数に到達 */
		if ( regionCounter >= resultRegionArray.size() ){
			break;
		}
	}

	return regionCounter;		// 検出個数を返す
} // detectRegion