/*****************************************************************
		ラベリングクラス
		2014/06/11 OpenCV講習会 by 佐藤俊樹@UECISMS
 *****************************************************************/

#ifndef __REGIONLABELER_H__
#define __REGIONLABELER_H__

#include <opencv2/opencv.hpp>
using namespace cv;

/* ラベリング結果のデータ構造 */
typedef struct{	
	float size;					// サイズ
	Point2f centroid;		// 重心
	Rect boundingRect;	// 包含矩形
	float angle;				// 慣性主軸角
} Region;

class RegionLabeler{

public:

	RegionLabeler();

	virtual ~RegionLabeler();

	/* ラベリング */
	virtual int detectRegion(Mat &srcImageGRAY, float maxSizeThreshold, float minSizeThreshold, Vector<Region> &resultRegionArray);

protected:

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
};

#endif