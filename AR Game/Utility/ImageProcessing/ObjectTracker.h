/*****************************************************************
		トラッキングクラス
		2014/06/17 by 佐藤俊樹@UECISMS
		OS依存性について: 
			時間取得のためにWindows.hのtimeGetTime()を使用している。
			これに伴い、Windows.hのinclude及びwinmm.libの追加を行っている。
 *****************************************************************/
#ifndef __OBJECT_TRACKER_H__
#define __OBJECT_TRACKER_H__

#include <opencv2/opencv.hpp>
using namespace cv;

#include <Windows.h>

#include "../../Definitions.h"
#include "../Timer/Timer.h"
#include "RegionLabeler.h"

class ObjectTracker{

public:

	ObjectTracker();	// maxObjects: 配列の最大個数

	virtual ~ObjectTracker();

	/* 初期化 */
	virtual int initialize(int maxObjects){
		trackingObjectArray.resize(maxObjects);
		for (int i = 0; i < maxObjects; i++){
			trackingObjectArray[i].id = trackingObjectArray[i].lastUpdatedTime = 0;
			trackingObjectArray[i].x = trackingObjectArray[i].y = trackingObjectArray[i].size = 0.0f;
		}
		return 0;
	} // initialize

	/****************************************************
			入力配列: Region配列
			出力: TrackingObject配列および有効TrackingObjectの個数
	 ****************************************************/
	virtual int trackObjects(const Vector<Region> &srcRegionArray, unsigned int detectedRegionCount);

	/* 配列ポインタを返す */
	virtual vector<TrackingObject> *getTrackingObjectArrayPtr(){
		return &trackingObjectArray;
	} // getTrackingObjectArrayPtr

	/* 閾値の設定 */
	void setTrackingDistanceThreshold(float threshold){
		trackingDistanceThreshold = threshold;
	} // setTrackingDistanceThreshold

protected:

	vector<TrackingObject> trackingObjectArray;
	float trackingDistanceThreshold;
	unsigned int deleteWaitingTime;

	Timer *timer;

};

#endif