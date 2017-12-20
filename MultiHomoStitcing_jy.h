//#pragma once
//#include "opencv2\core\core.hpp"
//#include "opencv2\imgproc\imgproc.hpp"
//#include "opencv2\features2d\features2d.hpp"
//#include "opencv2\nonfree\nonfree.hpp"
//#include "opencv2\nonfree\features2d.hpp"
//#include "opencv2\calib3d\calib3d.hpp"
//#include<vector>
//#include<list>
//#include "meanshift\MeanShift.h"
//
//#include"Mesh.h"
//
//using namespace cv;
//using namespace std;
//
//enum imgType { BINARY, GRAY, COLOR };
//
//typedef struct Image2D{
//	imgType type;
//	int nH, nW, nPosX, nPosY;
//	BYTE** imgRed, **imgBlue, **imgGreen;
//	BYTE** imgGray;
//} Image2D;
//
//class CImageStitchBasic
//{
//	/*****************************************************************
//	�� �Լ��� �̿��ϱ� ���ؼ��� ������ ���� ������ �� �ʿ��ϴ�.
//	1. GetMatchingPoint();
//	2. GetHomography();
//	3. WarpHomography();
//	*******************************************************************/
//protected :
//	// value for Image
//	Image2D imgL, imgR;
//	Image2D imgMark;
//	Image2D imgTotal;
//
//	// value for match
//	vector< Point2f > GodPointL;
//	vector< Point2f > GodPointR;
//	int nNumPoint;
//
//	// homography
//	Mat homo;
//
//	// value for multibandblend
//	double** weightSeamR, ** weightSeamL;
//	double **weightLtrans, **weightRtrans;
//	Image2D imgRtrans, imgLtrans;
//
//	void DrawFeature();
//	void DrawMatch();
//	void DrawLR();
//
//	void MultiBandBlending(int nH, int nW, BYTE** Result_R, BYTE** Result_G, BYTE** Result_B); // multiBandBlending
//	void MakeWeightSeam(double** weightSeam, int nH, int nW);
//	int BilinearInterpolation(Point2f point, BYTE** image, int nH, int nW);
//
//public:
//	CImageStitchBasic();
//	CImageStitchBasic(BYTE* img1D_L, BYTE* img1D_R, int nHL, int nWL, int nHR, int nWR);
//	~CImageStitchBasic();
//
//	void GetMatchingPoint(int nPara=800, bool bOnFeature=false, bool bOnMatch=false);
//	void GetHomography();
//	void WarpHomography(bool bOnLR=false);
//};
//
////=================================================================================================================================
////=================================================================================================================================
////=================================================================================================================================
//class CImageStitchMeanShift : public CImageStitchBasic
//{
//	/*****************************************************************
//	�� �Լ��� �̿��ϱ� ���ؼ��� ������ ���� ������ �� �ʿ��ϴ�.
//	1. GetMatchingPoint();
//	2. SegmentMeanshift();
//	3. DetermineActive();
//	4. FindSingleObject();
//	5. MakeWeightMap();
//	6. WarpHomograpy();
//	*******************************************************************/
//private :
//	Image2D imgSegment;
//
//	int nNumSeg;
//	int** LabelMeanShift; // Labeling map
//
//	int nNumObjCandi;
//	int* arrObjIdx;
//	int* arrErr;
//	double* arrInfo;
//
//	int* arrFeatureNum;
//	int* arrRegion;
//	Mat* arrHomo;
//
//	int nIdxObj;
//	Mat homoBack, homoObj;
//	Mat homoGlo;
//
//	BYTE ** weightObj;
//
//
//	double GetExceptInlierRate(int* arrIdx, int numIdx, int mode = 0, Mat& homo = Mat());
//	double GetInlierRate(int Idx, Mat& homo);
//	void GetArrFeatureNum();
//	void GetArrRegion();
//	void GetArrHomo();
//	void GetHomography(int Idx, Mat& homo);
//	void DrawSegment();
//	double KLdivergenceForBivariateGaussiian(double* mean0, double* mean1, double** covMat0, double** covMat1);
//	double DissimilarHomo(int nH, int nW, Mat& homo0, Mat& homo1, bool bOnDraw = false);
//
//	//�ĺ��� �Ǻ�
//	void ActiveFeatureNum(int minFeature);
//	void ActiveRegion(int minArea);
//	bool ActiveFeatureRate(double minFeatureRate);
//	void ActiveMisMatch();
//	void ActiveKLdivergence(double minKLdivergence);
//	void DrawActiveErr();
//
//public :
//	CString strFileName;
//	bool bExeDualHomo;
//
//	CImageStitchMeanShift();
//	CImageStitchMeanShift(BYTE* img1D_L, BYTE* img1D_R, int nHL, int nWL, int nHR, int nWR, CString fileName);
//	~CImageStitchMeanShift();
//
//	void SegmentMeanshift(int minRegion = 100);
//	void DetermineActive(int minFeature = 4, int minArea = 200, double minFeatureRate = 0.5, int minKLdiver = 50, bool bDrawErrInfo = false);
//	void FindSingleObject(float fDiffInlierRate = 0.1, bool bDrawErrInfo = false);
//	void MakeWeightMap(int dilation, bool bOnDraw = false);
//	void WarpHomography(bool bWrite = false);
//
//	//------------------------------
//	void FindMultiObject();
//	void FindObject();
//
//	void Run();
//};
////=================================================================================================================================
////=================================================================================================================================
////=================================================================================================================================
//class CImageStitchMultiHomo : public CImageStitchBasic
//{
//protected:
//	Point2f* arrInitKmean;
//	Point2f* arrCenter;
//	int* labelCluster;
//
//	// Mesh
//	CMesh* arrMesh;
//	int nNumVert;
//
//	Mat** homoAll;
//	vector <Point2f> * arrClusterR, *arrClusterL;
//
//public:
//	CImageStitchMultiHomo();
//	~CImageStitchMultiHomo();
//	
//	void KMeansCluster(bool bForgy = false);
//	void BlendingHomo();
//
//	void MakeMesh();
//	void WarpMeshHomography();
//};
//
//class CImageStitchLayer : public CImageStitchMultiHomo
//{
//private:
//	vector< vector<Point2f> > SetPointLayerL, SetPointLayerR;
//	vector< Mat> setHomoLayer;
//	int nNumLayer;
//	int* nNumSetPoint;
//
//public:
//	CImageStitchLayer();
//	~CImageStitchLayer();
//
//	void FindLayer(int nThresh);
//	void BlendingHomo(double sigma = 20);
//
//	void DrawLayer();
//};
//
//class CImageStitchKMeans : public CImageStitchMultiHomo
//{
//private:
//	Mat* arrHomo;
//	double** weightMap;
//	
//	void RemoveOutlier(vector<Point2f>& pointR, vector<Point2f>& pointL, double thresh);
//	double GetInlierRate(vector<Point2f>& pointR, vector<Point2f>& pointL, Mat& homo, double thresh);
//	void DrawFeature();
//public:
//	CImageStitchKMeans();
//	~CImageStitchKMeans();
//
//	void RemoveOutlier1st(double dThresh = 7);
//	void KMeansCluster();
//	void RemoveOutlier2nd(double dThresh = 3);
//	void BlendingHomo();
//};
//
////==================================================================================================================================
////==================================================================================================================================
////==================================================================================================================================
//
//class CMultiHomoStitcing_jy
//{
//public:
//
//	// value for Image
//	Image2D imgL, imgR;
//	Image2D imgMark;
//	Image2D imgTotal;
//	
//	int nH_total, nW_total;
//
//	// value for match
//	vector< Point2f > GodPointL;
//	vector< Point2f > GodPointR;
//	int nNumPoint;
//
//	// value for cluster
//	int nNumCluster; // ������			
//	Point2f* arrInitKmean; // k-mean's initial value
//	vector< Point2f > * arrClusterR; // ImageR's cluster
//	vector< Point2f > * arrClusterL; // ImageL's cluster
//	Point2f* arrCenter; // cluster's center
//
//	int* labelCluster; // cluster's label for pixel
//
//	Mat * arrHomo; // cluster's Homography
//	double*** arrCovMat; // cluster's Covarience Matrix
//	double** arrPCAvec; // cluster's PCA
//
//	double* arrAngle; // two cluster's PCA's angle
//	double** weightMap; // two cluster's Weight Value
//	int** labelWeight;  //labeling cluster's number
//
//	Mat** homography_all; // Homography for pixel
//
//	//experient value
//	double nOperator;
//
//	//MultiBlending value
//	double **weightLtrans, **weightRtrans;
//	BYTE **ImageLtrans_R, **ImageLtrans_G, **ImageLtrans_B;
//	BYTE **ImageRtrans_R, **ImageRtrans_G, **ImageRtrans_B;
//
//	CMultiHomoStitcing_jy(int k);
//	~CMultiHomoStitcing_jy();
//
//	//*******ImageStitching using segmentation value
//	BYTE** segmentRed, ** segmentBlue, ** segmentGreen;
//	int** LabelMeanShift; // Labeling map
//	int* arrRegionFeaturepoint; // feature's number in region
//	int* arrRegionArea;			// Area's number in region
//	int* regionIdx;	// region Index for more than minFeature.
//
//	int nNumSeg; // region's number
//	int nNumHomoRegion; // region's number for more than minFeature
//	Mat* homo_seg; // homography excepted region
//	
//	bool bExeDualHomo;
//
//	Mat gloHomo;	// Global Homography
//	Mat objHomo;	// object's Homography
//	vector< Point2f > objClusterR;	// object's ClusterR
//	vector< Point2f > objClusterL;	// object's ClusterL
//	Mat backHomo;	// background's Homography
//	vector< Point2f > backClusterR; // background's ClusterR
//	vector< Point2f > backClusterL; // background's ClusterL
//
//	float* arrInlierRate; // inlier rate excepted region ( inlier / all point - feature's number in region)
//	vector< Point2f > *segClusterExcptR; // feature excepted region in imageR
//	vector< Point2f > *segClusterExcptL; // feature excepted region in imageL
//	vector< Point2f > *segClusterR;
//	vector< Point2f > *segClusterL;
//	Point2f* segMarkR; // mark position for describing region
//	int nMaxInlierRegion;
//
//	/*****************************************************************
//	�� �Լ��� �̿��ϱ� ���ؼ��� ������ ���� ������ �� �ʿ��ϴ�.
//	1. GetMatchingPoint();
//	2. SetInitCenPoint() or GetInitCenPointForKmean();
//	3. KmeanForMultiHomo();
//	4. GetHomography();
//	5. MakeWeightMap(); series
//	6. BlendHomo();
//	7. WarpHomography() or MeshWarpHomography();
//
//	�� ������ �������� ���� �� ������ NULL�� �ʵ� ���� �ʴ´�. �Ф�
//	*******************************************************************/
//
//	void GetMatchingPoint();
//	void DrawMatchingPoint();
//
//	void SetInitCenPoint(Point2f* arrInput);
//	void GetInitCenPointForKmean(bool bForgy = false);
//
//	void KmeanForMultiHomo();
//
//	void DrawCluster();
//
//	void GetHomography();
//
//	void WarpHomography(int nMode, int nIdx = 0, bool bOnContour = false);
//	void MeshWarpHomography(bool bOnGrid = false);
//
//	void PCA();
//	void GetVectAngle2D();
//
//	void MakeWeightMap(); // k > 2 �� �� ����ġ �� ���� 
//	void MakeWeightMap_TopBtm(); // ���Ҷ��񽺸� �̿��� ����ġ �� ����
//	void MakeWeightMap_OriginTopBtm(); // ��Ŭ���� �Ÿ��� �̿��� ����ġ �� ����
//
//	void DrawWeightMap(bool bOnAngle = 0);
//
//	void BlendHomo();
//
//	/////////////////////////////////////////////////////////////////////////////
//
//	void MakeWeightSeam(double** weightSeam, int nH, int nW); //Seam ������ �׸��̿�
//	void MultiBandBlending(int nH, int nW, BYTE** Result_R, BYTE** Result_G, BYTE** Result_B); // multiBandBlending
//
//	///////////////////////////////////////////////////////////////////////////////
//	// Mean Shift Segmentation�� �̿��� �̹��� ��ƼĪ ���
//	//
//	// Mean Shift �ϴ� ��� (160426 �߰�.)
//	/*********************************************************
//	���� ����<���� ��ü>
//	
//	1. GetMatchingPoint
//	2. SegmentMeanShift
//	3. GetHomographyForSeg
//	4. SetWegithMap
//	5. GetObjectHomography
//	6. Warping function
//
//	���� ����<�ټ� ��ü>
//	1. GetMatchingPoint
//	2. SegmentMeanShift
//	3. DetectMultiObject
//	**********************************************************/
//	//value
//	BYTE** weightObj;
//
//	void SegmentMeanshift(int minRegio1=100, int minFeature=3, int minArea=200);
//	
//	void DetectMultiObject();
//
//	void GetHomographyForSeg();
//	void GetObjectHomography(float maxInlierDis);
//	void SetWeightMap(int dilation);
//	void MeshWarpForSeg();
//	void WarpTest();
//	void WarpSingleHomo();
//	void DrawSegmentImage(bool bOnOnlyRegion=false);
//
//	///////////////////////////////////////////////////////////////////////////
//	//Parallax-Rpbust Surveillance Video Stitching (160527 �߰�)
//	//*************************************************************************
//	//value 
//	vector< vector<Point2f> > SetPointLayerL, SetPointLayerR;
//	vector< Mat> setHomoLayer;
//	void WarpLayer(int nThresh);
//	void MeshWarpHomographyForLayer(bool bOnGrid = false);
//
//	////////////////////////////////////////////////////////////////////////////
//	//Parallax Handling of Image Stitching using Dominant-plane Homography (160607 �߰�)
//	//*************************************************************************
//	//value : use arrCenter, arrClusterR, arrNumData, arrHomo
//
//	double* arrCost;
//
//	//function
//	void ParallaxHandling(int maxInlierDis);
//
//	void CheckDImageColor(double ** R, double ** G, double ** B, int nH, int nW);
//	
//	float GetDissmiliarity(int nH, int nW, Mat& homo0, Mat& homo1);
//	double GetExceptInlierRate(int* arrIdx, int numIdx, int mode = 0, Mat& homo = Mat());
//	bool CompareDistri(int searchR, int thresh);
//	int BilinearInterpolation(Point2f point, BYTE** image, int nH, int nW);
//	double KLdivergenceForBivariateGaussiian(double* mean0, double* mean1, double** covMat0, double** covMat1);
//};