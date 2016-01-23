/*****************************************************************
		���x�����O�N���X
		2014/06/11 OpenCV�u�K�� by �����r��@UECISMS
 *****************************************************************/

#ifndef __REGIONLABELER_H__
#define __REGIONLABELER_H__

#include <opencv2/opencv.hpp>
using namespace cv;

/* ���x�����O���ʂ̃f�[�^�\�� */
typedef struct{	
	float size;					// �T�C�Y
	Point2f centroid;		// �d�S
	Rect boundingRect;	// ��܋�`
	float angle;				// �����厲�p
} Region;

class RegionLabeler{

public:

	RegionLabeler();

	virtual ~RegionLabeler();

	/* ���x�����O */
	virtual int detectRegion(Mat &srcImageGRAY, float maxSizeThreshold, float minSizeThreshold, Vector<Region> &resultRegionArray);

protected:

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
};

#endif