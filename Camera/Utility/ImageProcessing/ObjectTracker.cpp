#include "ObjectTracker.h"

ObjectTracker::ObjectTracker(){
	timer = Timer::getInstance();
	trackingDistanceThreshold = 30.0f;
}

ObjectTracker::~ObjectTracker(){}

/* ������ */
int ObjectTracker::trackObjects(const Vector<Region> &srcRegionArray, unsigned int detectedRegionCount){
	int flagUpdated, objectCounter;
	flagUpdated = objectCounter = 0;
	float distance;

	unsigned int idCounter = 1;

	/* ���̃t���[���Ŕ������ꂽ�S�Ă�Region�ɑ΂��� */
	for ( unsigned int j = 0; j < detectedRegionCount; j++ ){

		/* �O�t���[���܂łɓo�^���ꂽ�r�Ƃ̑Ή��𒲂ׂ� */
		for ( unsigned int i = 0; i < trackingObjectArray.size(); i++ ){

			if ( trackingObjectArray[i].id ){
				distance = sqrtf( ( trackingObjectArray[i].x - srcRegionArray[j].centroid.x ) * ( trackingObjectArray[i].x - srcRegionArray[j].centroid.x ) 
					+ ( trackingObjectArray[i].y - srcRegionArray[j].centroid.y ) * ( trackingObjectArray[i].y - srcRegionArray[j].centroid.y ) );

				/* �d�S�ړ��������������瓯���肾�Ƃ݂Ȃ��A�X�V���s�� */
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

		/* ����������V�����ǉ� */
		if ( !flagUpdated ){
			for ( unsigned int i = 0; i < trackingObjectArray.size(); i++ ){
				if ( !trackingObjectArray[i].id ){
					trackingObjectArray[i].id = trackingObjectArray[i].lastUpdatedTime = timer->systemTime + idCounter++;				// �肪�d�Ȃ�A�Ăї��ꂽ�Ƃ��ɓ���ID�����肪�������Ȃ��悤��++���Ă���(���܂�悢�������@�ł͂Ȃ����E�E�E)
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

	/* �X�V�̖����f�[�^���폜���� */
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
///* �g���b�L���O */
//int trackObjects(Vector<Region> *srcRegionArray, int regionCount, Vector<TrackingObject> &trackingObjectArray){
//	int trackingObjectCount = 0;
//	int flagUpdated = 0;
//
//	/* �O��̌��o���ʂ�������Ƃ��߂��f�[�^��T���A�X�V���� */
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
//					/* �ŐV��Region�f�[�^���g���čX�V���� */
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
//		/* ����������V�����ǉ� */
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
//	/* �X�V�̖����f�[�^���폜���� */
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
