#include "stdafx.h"
#include "Mesh.h"
#include "Image.h"
#include "opencv2\core\core.hpp"
#include "opencv2\imgproc\imgproc.hpp"
#include "opencv2\features2d\features2d.hpp"
#include "opencv2\nonfree\nonfree.hpp"
#include "opencv2\nonfree\features2d.hpp"
#include "opencv2\calib3d\calib3d.hpp"

using namespace cv;

CMesh::CMesh()
{
	for (i = 0; i < 4; i++)
	{
		vertex[i].y = 0;
		vertex[i].x = 0;

		vertexDeform[i].y = 0;
		vertexDeform[i].x = 0;
	}

	GetCentral();
}

CMesh::CMesh(int nLeftUpY, int nLeftUpX, int nSize)
{
	vertex[0].x = nLeftUpX;
	vertex[0].y = nLeftUpY;

	vertex[1].x = nLeftUpX + nSize;
	vertex[1].y = nLeftUpY;

	vertex[2].x = nLeftUpX;
	vertex[2].y = nLeftUpY + nSize;

	vertex[3].x = nLeftUpX + nSize;
	vertex[3].y = nLeftUpY + nSize;

	for (i = 0; i < 4; i++)
	{
		vertexDeform[i].y = 0;
		vertexDeform[i].x = 0;
	}

	GetCentral();
}

CMesh::CMesh(int nLeftUpY, int nLeftUpX, int nH, int nW)
{
	vertex[0].x = nLeftUpX;
	vertex[0].y = nLeftUpY;

	vertex[1].x = nLeftUpX + nW;
	vertex[1].y = nLeftUpY;

	vertex[2].x = nLeftUpX;
	vertex[2].y = nLeftUpY + nH;

	vertex[3].x = nLeftUpX + nW;
	vertex[3].y = nLeftUpY + nH;

	for (i = 0; i < 4; i++)
	{
		vertexDeform[i].y = 0;
		vertexDeform[i].x = 0;
	}

	GetCentral();
}

CMesh::~CMesh()
{
}

/****************************************************
<DeformMesh>

���� �ִ� �޽��� �ȼ����� ȣ��׷��ǰ� �Ҵ�� �����͸� �̿��Ͽ� �������ش�.
�� �� ������ �޽��� �ּҰ�, �ִ�, ȣ��׷��Ǹ� �����ش�.

bFeature�� true�� ��� content�� ��� rigid�� �̿��Ͽ� ����� �����ϴ�.

bFearture == false : Fearture�� �ƴϴ�.
bFearture == true : Fearture�̴�.
------------------------------------
Pre : vertex[i], 
Post : vertexDeform[i], nMinX, nMinY, nMaxX, nMaxY, Homo
*******************************************************/
void CMesh::DeformMesh(bool bFeature, Mat** homography)
{
	if (bFeature == false)
	{
		for (i = 0; i < 4; i++)
			GetPerspectivePoint(vertex[i], vertexDeform[i], homography[int(vertex[i].y)][int(vertex[i].x)]);
	}

	if (bFeature == true)
	{
		this->GetRigidMatrix();

		for (i = 0; i < 4; i++)
			GetPerspectivePoint(vertex[i], vertexDeform[i], Rigid);

	}

	//get Max and Min of vertexDeform
	if (vertexDeform[0].x > vertexDeform[2].x)
		nMinX = vertexDeform[2].x;
	else
		nMinX = vertexDeform[0].x;

	if (vertexDeform[1].x > vertexDeform[3].x)
		nMaxX = vertexDeform[1].x;
	else
		nMaxX = vertexDeform[3].x;

	if (vertexDeform[0].y > vertexDeform[1].y)
		nMinY = vertexDeform[1].y;
	else
		nMinY = vertexDeform[0].y;

	if (vertexDeform[2].y > vertexDeform[3].y)
		nMaxY = vertexDeform[2].y;
	else
		nMaxY = vertexDeform[3].y;

	//get Homography between mesh and meshDeform;

	Point2f tmpVert[4], tmpVertDeform[4];

	tmpVert[0] = Point2f(vertex[0].x, vertex[0].y);
	tmpVert[1] = Point2f(vertex[1].x, vertex[1].y);
	tmpVert[2] = Point2f(vertex[2].x, vertex[2].y);
	tmpVert[3] = Point2f(vertex[3].x, vertex[3].y);

	tmpVertDeform[0] = Point2f(vertexDeform[0].x, vertexDeform[0].y);
	tmpVertDeform[1] = Point2f(vertexDeform[1].x, vertexDeform[1].y);
	tmpVertDeform[2] = Point2f(vertexDeform[2].x, vertexDeform[2].y);
	tmpVertDeform[3] = Point2f(vertexDeform[3].x, vertexDeform[3].y);

	Homo = getPerspectiveTransform(tmpVert, tmpVertDeform);
}

/****************************************************
<DeformVertex>

�޽��� vertex�� ���� �ٸ� homography�� ������ �� ���ȴ�.
------------------------------------
Pre : vertex[i]
Post : vertexDeform[i]
*******************************************************/
void CMesh::DeformVertex(int vertPos, Mat homography)
{
	GetPerspectivePoint(vertex[vertPos], vertexDeform[vertPos], homography);
}

/****************************************************
<GetSizeDeformMesh>

������ �޽��� �ּڰ�, �ִ�, ȣ�� �����ش�. 

** ���� : DeformVertex�� ���ؼ� vertexDeform�� �Ҵ��ϰ� ����ؾ� �Ѵ�.

------------------------------------
Pre : vertex[i], vertexDeform[i]
Post :  nMinX, nMinY, nMaxX, nMaxY, Homo
*******************************************************/
void CMesh::GetSizeDeformMesh()
{
	//get Max and Min of vertexDeform
	if (vertexDeform[0].x > vertexDeform[2].x)
		nMinX = vertexDeform[2].x;
	else
		nMinX = vertexDeform[0].x;

	if (vertexDeform[1].x > vertexDeform[3].x)
		nMaxX = vertexDeform[1].x;
	else
		nMaxX = vertexDeform[3].x;

	if (vertexDeform[0].y > vertexDeform[1].y)
		nMinY = vertexDeform[1].y;
	else
		nMinY = vertexDeform[0].y;

	if (vertexDeform[2].y > vertexDeform[3].y)
		nMaxY = vertexDeform[2].y;
	else
		nMaxY = vertexDeform[3].y;

	//get Homography between mesh and meshDeform;

	Point2f tmpVert[4], tmpVertDeform[4];

	tmpVert[0] = Point2f(vertex[0].x, vertex[0].y);
	tmpVert[1] = Point2f(vertex[1].x, vertex[1].y);
	tmpVert[2] = Point2f(vertex[2].x, vertex[2].y);
	tmpVert[3] = Point2f(vertex[3].x, vertex[3].y);

	tmpVertDeform[0] = Point2f(vertexDeform[0].x, vertexDeform[0].y);
	tmpVertDeform[1] = Point2f(vertexDeform[1].x, vertexDeform[1].y);
	tmpVertDeform[2] = Point2f(vertexDeform[2].x, vertexDeform[2].y);
	tmpVertDeform[3] = Point2f(vertexDeform[3].x, vertexDeform[3].y);

	Homo = getPerspectiveTransform(tmpVert, tmpVertDeform);
}

void CMesh::GetRigidMatrix()
{
	CvPoint vertexMean, vertexDeformMean;
	CvPoint varVert[4], varVertDeform[4]; // varVert = vertex - vertexDeform;

	int tmpX=0, tmpY=0;

	for (i = 0; i < 4; i++)
	{
		tmpX += vertex[i].x;
		tmpY += vertex[i].y;
	}

	vertexMean.x = tmpX / 4;
	vertexMean.y = tmpY / 4;

	tmpX = 0;
	tmpY = 0;

	for (i = 0; i < 4; i++)
	{
		tmpX += vertexDeform[i].x;
		tmpY += vertexDeform[i].y;
	}

	vertexDeformMean.x = tmpX / 4;
	vertexDeformMean.y = tmpY / 4;

	for (i = 0; i < 4; i++)
	{
		varVert[i].x = vertex[i].x - vertexMean.x;
		varVert[i].y = vertex[i].y - vertexMean.y;

		varVertDeform[i].x = vertexDeform[i].x - vertexDeformMean.x;
		varVertDeform[i].y = vertexDeform[i].y - vertexDeformMean.y;
	}

	//--------------------------------

	Mat vertDeformMatrix(3, 4, CV_64FC1);
	Mat vertMatrix(3, 4, CV_64FC1);
	Mat transMatrix;

	int Pos = 0;

	for (i = 0; i < 4; i++)
	{
		vertDeformMatrix.at<double>(0,i) = varVertDeform[i].x;
		vertDeformMatrix.at<double>(1,i) = varVertDeform[i].y;
		vertDeformMatrix.at<double>(2, i) = 1;
	}

	Pos = 0;

	for (i = 0; i < 4; i++)
	{
		vertMatrix.at<double>(0, i) = varVert[i].x;
		vertMatrix.at<double>(1, i) = varVert[i].y;
		vertDeformMatrix.at<double>(2, i) = 1;
	}

	Mat tmpMat = vertMatrix.inv(DECOMP_SVD);
	transMatrix=vertDeformMatrix*tmpMat;
	  
	SVD svd(transMatrix);

	Rigid = svd.vt*svd.u.t();
}

void CMesh::GetCentral()
{
	central.x = (vertex[0].x + vertex[1].x) / 2;
	central.y = (vertex[0].y + vertex[2].y) / 2;
}