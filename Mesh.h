#pragma once

#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\nonfree\nonfree.hpp"
#include "opencv2\nonfree\features2d.hpp"
#include "opencv2\calib3d\calib3d.hpp"

using namespace cv;
class CMesh
{
public:
	int i, j, y, x;

	Point2f vertex[4];
	Point2f vertexDeform[4];
	// 0 : ���� ��,   1 : ������ ��
	// 2 : ���� �Ʒ�, 3 : ������ �Ʒ�
	
	int nMaxX, nMaxY;
	int nMinX, nMinY;

	Point2f central;

	Mat Homo; // Homography Matrix
	Mat Rigid; // Rigid Matrix

	CMesh();
	CMesh(int nLeftUpX, int nLeftUpY, int nSize);
	CMesh(int nLeftUpX, int nLeftUpY, int nH, int nW);
	~CMesh();

	void DeformMesh(bool bFeature, Mat** homography);
	void DeformVertex(int vertPos,Mat homography);
	void GetSizeDeformMesh();
	void GetRigidMatrix();
	void GetCentral();
};

