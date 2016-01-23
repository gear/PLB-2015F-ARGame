/*****************************************************************
		�g���b�L���O�N���X
		2014/06/17 by �����r��@UECISMS
		OS�ˑ����ɂ���: 
			���Ԏ擾�̂��߂�Windows.h��timeGetTime()���g�p���Ă���B
			����ɔ����AWindows.h��include�y��winmm.lib�̒ǉ����s���Ă���B
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

	ObjectTracker();	// maxObjects: �z��̍ő��

	virtual ~ObjectTracker();

	/* ������ */
	virtual int initialize(int maxObjects){
		trackingObjectArray.resize(maxObjects);
		for (int i = 0; i < maxObjects; i++){
			trackingObjectArray[i].id = trackingObjectArray[i].lastUpdatedTime = 0;
			trackingObjectArray[i].x = trackingObjectArray[i].y = trackingObjectArray[i].size = 0.0f;
		}
		return 0;
	} // initialize

	/****************************************************
			���͔z��: Region�z��
			�o��: TrackingObject�z�񂨂�їL��TrackingObject�̌�
	 ****************************************************/
	virtual int trackObjects(const Vector<Region> &srcRegionArray, unsigned int detectedRegionCount);

	/* �z��|�C���^��Ԃ� */
	virtual vector<TrackingObject> *getTrackingObjectArrayPtr(){
		return &trackingObjectArray;
	} // getTrackingObjectArrayPtr

	/* 臒l�̐ݒ� */
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