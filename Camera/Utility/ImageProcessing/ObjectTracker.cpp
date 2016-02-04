#include "ObjectTracker.h"

ObjectTracker::ObjectTracker(){
	timer = Timer::getInstance();
	trackingDistanceThreshold = 30.0f;
}

ObjectTracker::~ObjectTracker(){}

/* 初期化 */
int ObjectTracker::trackObjects(const Vector<Region> &srcRegionArray, unsigned int detectedRegionCount){
	int flagUpdated, objectCounter;
	flagUpdated = objectCounter = 0;
	float distance;

	unsigned int idCounter = 1;

	/* このフレームで発見された全てのRegionに対して */
	for ( unsigned int j = 0; j < detectedRegionCount; j++ ){

		/* 前フレームまでに登録された腕との対応を調べる */
		for ( unsigned int i = 0; i < trackingObjectArray.size(); i++ ){

			if ( trackingObjectArray[i].id ){
				distance = sqrtf( ( trackingObjectArray[i].x - srcRegionArray[j].centroid.x ) * ( trackingObjectArray[i].x - srcRegionArray[j].centroid.x ) 
					+ ( trackingObjectArray[i].y - srcRegionArray[j].centroid.y ) * ( trackingObjectArray[i].y - srcRegionArray[j].centroid.y ) );

				/* 重心移動が小さかったら同じ手だとみなし、更新を行う */
				if ( distance < trackingDistanceThreshold ){
					trackingObjectArray[i].lastUpdatedTime = timer->systemTime;
					trackingObjectArray[i].x = (float)srcRegionArray[j].centroid.x;
					trackingObjectArray[i].y = (float)srcRegionArray[j].centroid.y;
					trackingObjectArray[i].size = (float)srcRegionArray[j].size;
					trackingObjectArray[i].lastUpdatedTime = timer->systemTime;
					flagUpdated = 1;
					objectCounter++;
					break;
				}
			}
		} // for

		/* 無かったら新しく追加 */
		if ( !flagUpdated ){
			for ( unsigned int i = 0; i < trackingObjectArray.size(); i++ ){
				if ( !trackingObjectArray[i].id ){
					trackingObjectArray[i].id = trackingObjectArray[i].lastUpdatedTime = timer->systemTime + idCounter++;				// 手が重なり、再び離れたときに同じIDを持つ手が発生しないように++しておく(あまりよい解決方法ではないが・・・)
					trackingObjectArray[i].x = (float)srcRegionArray[j].centroid.x;
					trackingObjectArray[i].y = (float)srcRegionArray[j].centroid.y;
					trackingObjectArray[i].size = srcRegionArray[j].size;
					printf("Added: %u\n", trackingObjectArray[i].id);
					objectCounter++;
					break;
				}
			}
		}

		flagUpdated = 0;
	} // for

	/* 更新の無いデータを削除する */
	for ( unsigned int i = 0; i < trackingObjectArray.size(); i++ ){
		if ( trackingObjectArray[i].id != 0 ){
			if ( trackingObjectArray[i].lastUpdatedTime + 40 < timer->systemTime ){
				trackingObjectArray[i].id = 0;
				objectCounter--;
				printf("Removed: %u\n", trackingObjectArray[i].id);
			}
		}
	}

	return objectCounter;
} // trackObjects

//
///* トラッキング */
//int trackObjects(Vector<Region> *srcRegionArray, int regionCount, Vector<TrackingObject> &trackingObjectArray){
//	int trackingObjectCount = 0;
//	int flagUpdated = 0;
//
//	/* 前回の検出結果からもっとも近いデータを探し、更新する */
//	for ( int i = 0; i < regionCount; i++ ){
//
//		flagUpdated = 0;
//		for ( int j = 0; j < trackingObjectArray.size(); j++ ){
//			if ( trackingObjectArray[j].id != 0 ){
//
//				float distance = sqrtf(
//					((*srcRegionArray)[i].centroid.x - trackingObjectArray[j].x) * ((*srcRegionArray)[i].centroid.x - trackingObjectArray[j].x)
//					+ ((*srcRegionArray)[i].centroid.y - trackingObjectArray[j].y) * ((*srcRegionArray)[i].centroid.y - trackingObjectArray[j].y) );
//
//				if ( distance < 30.0f ){					
//					/* 最新のRegionデータを使って更新する */
//					trackingObjectArray[j].x = (*srcRegionArray)[i].centroid.x;
//					trackingObjectArray[j].y = (*srcRegionArray)[i].centroid.y;
//					trackingObjectArray[j].size = (*srcRegionArray)[i].size;
//					trackingObjectArray[j].lastUpdatedTime = timer->systemTime;
//					flagUpdated = 1;
//					break;
//				}
//			}
//		}
//
//		/* 無かったら新しく追加 */
//		if (flagUpdated == 0){
//			for ( unsigned int j = 0; j < trackingObjectArray.size(); j++ ){
//				if ( trackingObjectArray[j].id == 0 ){
//					trackingObjectArray[j].x = (*srcRegionArray)[i].centroid.x;
//					trackingObjectArray[j].y = (*srcRegionArray)[i].centroid.y;
//					trackingObjectArray[j].size = (*srcRegionArray)[i].size;
//					trackingObjectArray[j].id = trackingObjectArray[j].lastUpdatedTime = timer->systemTime;			
//					printf("Added: %u\n", trackingObjectArray[j].id);
//					break;
//				}
//			}
//		}
//
//	} // for
//
//	/* 更新の無いデータを削除する */
//	for ( unsigned int i = 0; i < trackingObjectArray.size(); i++ ){
//		if ( trackingObjectArray[i].id != 0 ){
//			if ( trackingObjectArray[i].lastUpdatedTime + 40 < timer->systemTime ){
//				printf("Removed: %u\n", trackingObjectArray[i].id);
//				trackingObjectArray[i].id = 0;
//			}
//		}
//	}
//
//	return trackingObjectCount;
//} // trackObjects
