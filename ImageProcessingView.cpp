// ImageProcessingView.cpp : implementation of the CImageProcessingView class
//

#include "stdafx.h"
#include "ImageProcessing.h"
#include "resource.h"

#include "ImageProcessingDoc.h"
#include "ImageProcessingView.h"

#include "MainFrm.h"		// �߰�

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView

IMPLEMENT_DYNCREATE(CImageProcessingView, CScrollView)

BEGIN_MESSAGE_MAP(CImageProcessingView, CScrollView)
	//{{AFX_MSG_MAP(CImageProcessingView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDM_GRAY_DUPLICATE, OnGrayDuplicate)
	ON_COMMAND(IDM_COLOR_DUPLICATE, OnColorDuplicate)
	ON_COMMAND(IDM_CHAP2, OnChap2)
	ON_COMMAND(IDM_CHAP3, OnChap3)
	ON_COMMAND(IDM_SCALE_X2, OnScaleX2)
	ON_COMMAND(IDM_SCALE_HALF, OnScaleHalf)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_CHAP4, OnChap4)
	ON_COMMAND(IDM_CHAP5, OnChap5)
	ON_COMMAND(IDM_APPLICATION, OnApplication)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_SEL_REGION, OnSelRegion)
	ON_COMMAND(IDM_SEL_REGION_ZOOM, OnSelRegionZoom)
	ON_COMMAND(IDM_SUM, OnSum)
	ON_COMMAND(IDM_SUBTRACT, OnSubtract)
	ON_COMMAND(IDM_DIFFERENCE, OnDifference)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
	ON_COMMAND(IDM_IMAGESTITCH_JY, &CImageProcessingView::OnImagestitchJy)
	ON_COMMAND(IDM_VIDEOSTITCH, &CImageProcessingView::OnVideostitch)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView construction/destruction

CImageProcessingView::CImageProcessingView()
{
	// �ʿ��� ��Ʈ�� ��� ���� �ʱ�ȭ
	m_BmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_BmiHeader.biPlanes = 1;
	m_BmiHeader.biBitCount = 24;
	m_BmiHeader.biCompression = BI_RGB;
	m_BmiHeader.biXPelsPerMeter = 1000;
	m_BmiHeader.biYPelsPerMeter = 1000;
	m_BmiHeader.biClrUsed = 0;
	m_BmiHeader.biClrImportant = 0;

	// ���� ���� ��ȣ �ʱ�ȭ
	m_nCurrentImage = -1;

	m_nSelectRegion = 0;
}

CImageProcessingView::~CImageProcessingView()
{
}

BOOL CImageProcessingView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView drawing

void CImageProcessingView::OnDraw(CDC* pDC)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// ���� ���
	for(int i = 0 ; i < pDoc->m_nImageCnt ; i++)
	{
		// �ʿ��� ��Ʈ�� �ش� ���� ����
		m_BmiHeader.biWidth = pDoc->m_Image[i].nW;
		m_BmiHeader.biHeight = pDoc->m_Image[i].nH;
		m_BmiHeader.biSizeImage = GetBmp24Size(pDoc->m_Image[i].nW, pDoc->m_Image[i].nH);
		
		// ���� ���
		StretchDIBits(pDC->m_hDC,
				pDoc->m_Image[i].nPosX, pDoc->m_Image[i].nPosY,
				m_BmiHeader.biWidth,
				m_BmiHeader.biHeight,
				0,0,
				m_BmiHeader.biWidth,
				m_BmiHeader.biHeight,
				pDoc->m_Image[i].Image1D,
				(LPBITMAPINFO)&m_BmiHeader,
				DIB_RGB_COLORS,
				SRCCOPY);
	}

	CRect rt;
	CFont font;

	GetClientRect(&rt);
	font.CreatePointFont(200, _T("Arial"));
	pDC->SelectObject(&font);

	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255, 0, 255));
	pDC->DrawText(m_ViewMsg, rt, DT_LEFT);
}

void CImageProcessingView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	
	// ��ũ�Ѻ��� ũ�� ����
	sizeTotal.cx = 1024*2;
	sizeTotal.cy = 768*2;
	SetScrollSizes(MM_TEXT, sizeTotal);

	m_PastePoint= CPoint(-1, -1);
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView printing

BOOL CImageProcessingView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CImageProcessingView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CImageProcessingView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView diagnostics

#ifdef _DEBUG
void CImageProcessingView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CImageProcessingView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CImageProcessingDoc* CImageProcessingView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImageProcessingDoc)));
	return (CImageProcessingDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingView message handlers

// point ��ǥ�� �����ϴ� ������ ���� �������� ����
int CImageProcessingView::GetCurrentImageRect(CPoint point)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPoint CurrentPoint= GetScrollPosition() + point;

	for(int i = pDoc->m_nImageCnt-1 ; i >= 0 ; i--)
	{
		// i��° ������ �ܰ� �簢��
		CRect rt(pDoc->m_Image[i].nPosX, pDoc->m_Image[i].nPosY, 
			pDoc->m_Image[i].nPosX + pDoc->m_Image[i].nW, pDoc->m_Image[i].nPosY+pDoc->m_Image[i].nH);

		// �簢���� point ���� ���� Ȯ��
		if(rt.PtInRect(CurrentPoint)) 
		{
			// point�� �����ϴ� ������ ���� ������ �������� ����
			ImageInfo SaveImage = pDoc->m_Image[i];
			for(int k = i ; k < pDoc->m_nImageCnt - 1 ; k++)
				pDoc->m_Image[k] = pDoc->m_Image[k+1];

			pDoc->m_Image[pDoc->m_nImageCnt-1] = SaveImage;

			return pDoc->m_nImageCnt-1;
		}
	}

	// ������ ã�� ����
	return -1;
}

// point ��ǥ�� (�»�) ��ġ�� ������ ������ ���� �������� ����
int CImageProcessingView::GetCurrentImagePos(CPoint point)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPoint CurrentPoint= GetScrollPosition() + point;

	for(int i = pDoc->m_nImageCnt-1 ; i >= 0 ; i--)
	{
		// i��° ������ (�»�)��ġ
		CPoint pt(pDoc->m_Image[i].nPosX, pDoc->m_Image[i].nPosY);

		if(CurrentPoint == pt) 
		{
			// point�� (�»�)��ġ�� ���� �� ������ ������ �������� ����
			ImageInfo SaveImage = pDoc->m_Image[i];
			for(int k = i ; k < pDoc->m_nImageCnt - 1 ; k++)
				pDoc->m_Image[k] = pDoc->m_Image[k+1];

			pDoc->m_Image[pDoc->m_nImageCnt-1] = SaveImage;

			return pDoc->m_nImageCnt-1;
		}
	}

	// ������ ã�� ����
	return -1;
}

// ������ ������ ��, ����, ��ġ �б�
bool CImageProcessingView::GetCurrentImageInfo(int *pW, int *pH, int *pPosX, int *pPosY, int nIndex) 
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int nCurrentImage = pDoc->m_nImageCnt-1-nIndex;	// ������ ����-nIndex

	if(nCurrentImage < 0) return false;

	*pW = pDoc->m_Image[nCurrentImage].nW;
	*pH = pDoc->m_Image[nCurrentImage].nH;
	*pPosX = pDoc->m_Image[nCurrentImage].nPosX;
	*pPosY = pDoc->m_Image[nCurrentImage].nPosY;

	return true;
}

// ������ ������ 2���� ȸ���� ������ �б�
bool CImageProcessingView::GetCurrentImageGray(BYTE **ImageGray, int nIndex)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int nCurrentImage = pDoc->m_nImageCnt-1-nIndex;	// ������ ����-nIndex

	if(nCurrentImage < 0) return false;

	int x, y, Pos;

	for(y = 0 ; y < pDoc->m_Image[nCurrentImage].nH ; y++)
		for(x = 0 ; x < pDoc->m_Image[nCurrentImage].nW ; x++)
		{
			Pos = GetBmp24Pos(pDoc->m_Image[nCurrentImage].nW, pDoc->m_Image[nCurrentImage].nH, x, y);
			ImageGray[y][x] = (pDoc->m_Image[nCurrentImage].Image1D[Pos]+pDoc->m_Image[nCurrentImage].Image1D[Pos+1]+pDoc->m_Image[nCurrentImage].Image1D[Pos+2])/3;
		}

	return true;
}

// 2���� ȸ���� ������ ���
bool CImageProcessingView::DisplayCimage2D(BYTE **ImageGray, int nW, int nH, int nPosX, int nPosY, 
	bool bErase, bool bDelete, int Rate)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// bDelete�� true�̸� nPosX, nPosY��ġ�� ���� ������ ����
	if(bDelete)
	{
		int nCurrentImage = GetCurrentImagePos(CPoint(nPosX, nPosY)-GetScrollPosition());
		if(nCurrentImage >= 0)
		{
			delete [] pDoc->m_Image[nCurrentImage].Image1D;

			pDoc->m_nImageCnt--;
		}
	}

	int nCurrentImage = pDoc->m_nImageCnt;		// ���ο� ����(��� ����)

	if(nCurrentImage >= MAX_IMAGE) return false;

	int x, y, Pos;

	pDoc->m_Image[nCurrentImage].nW = nW*Rate/100;
	pDoc->m_Image[nCurrentImage].nH = nH*Rate/100;
	pDoc->m_Image[nCurrentImage].nPosX = nPosX;
	pDoc->m_Image[nCurrentImage].nPosY = nPosY;

	pDoc->m_Image[nCurrentImage].Image1D = new BYTE[GetBmp24Size(nW*Rate/100, nH*Rate/100)];

	for(y = 0 ; y < nH*Rate/100 ; y++)
		for(x = 0 ; x < nW*Rate/100 ; x++)
		{
			Pos = GetBmp24Pos(nW*Rate/100, nH*Rate/100, x, y);

			pDoc->m_Image[nCurrentImage].Image1D[Pos++] = ImageGray[y*100/Rate][x*100/Rate];
			pDoc->m_Image[nCurrentImage].Image1D[Pos++] = ImageGray[y*100/Rate][x*100/Rate];
			pDoc->m_Image[nCurrentImage].Image1D[Pos] = ImageGray[y*100/Rate][x*100/Rate];
		}

	pDoc->m_nImageCnt++;
	Invalidate(bErase);
	UpdateWindow();

	return true;
}

// ������ ������ 2���� �÷� ������ �б�
bool CImageProcessingView::GetCurrentImageColor(BYTE **ImageRed, BYTE **ImageGreen, BYTE **ImageBlue, int nIndex)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int nCurrentImage = pDoc->m_nImageCnt-1-nIndex;	// ������ ����-nIndex

	if(nCurrentImage < 0) return false;

	int x, y, Pos;

	for(y = 0 ; y < pDoc->m_Image[nCurrentImage].nH ; y++)
		for(x = 0 ; x < pDoc->m_Image[nCurrentImage].nW ; x++)
		{
			Pos = GetBmp24Pos(pDoc->m_Image[nCurrentImage].nW, pDoc->m_Image[nCurrentImage].nH, x, y);
			ImageBlue[y][x] = pDoc->m_Image[nCurrentImage].Image1D[Pos++];
			ImageGreen[y][x] = pDoc->m_Image[nCurrentImage].Image1D[Pos++];
			ImageRed[y][x] = pDoc->m_Image[nCurrentImage].Image1D[Pos];
		}

	return true;
}

bool CImageProcessingView::GetCurrentImage1DColor(BYTE *Image1D, int nIndex)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int nCurrentImage = pDoc->m_nImageCnt-1-nIndex;	// ������ ����-nIndex

	if(nCurrentImage < 0) return false;

	memcpy(Image1D, pDoc->m_Image[nCurrentImage].Image1D, GetBmp24Size(pDoc->m_Image[nCurrentImage].nW, pDoc->m_Image[nCurrentImage].nH));

	return true;
}

bool CImageProcessingView::DisplayCimage1D(BYTE *Image1D, int nW, int nH, 
	int nPosX, int nPosY, bool bErase, bool bDelete)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// bDelete�� true�̸� nPosX, nPosY��ġ�� ���� ������ ����
	if(bDelete)
	{
		int nCurrentImage = GetCurrentImagePos(CPoint(nPosX, nPosY)-GetScrollPosition());
		if(nCurrentImage >= 0)
		{
			delete [] pDoc->m_Image[nCurrentImage].Image1D;

			pDoc->m_nImageCnt--;
		}
	}

	int nCurrentImage = pDoc->m_nImageCnt;	// ���ο� ����(��� ����)

	if(nCurrentImage >= MAX_IMAGE) return false;

	pDoc->m_Image[nCurrentImage].nW = nW;
	pDoc->m_Image[nCurrentImage].nH = nH;
	pDoc->m_Image[nCurrentImage].nPosX = nPosX;
	pDoc->m_Image[nCurrentImage].nPosY = nPosY;

	pDoc->m_Image[nCurrentImage].Image1D = new BYTE[GetBmp24Size(nW, nH)];

	memcpy(pDoc->m_Image[nCurrentImage].Image1D, Image1D, GetBmp24Size(nW, nH));

	pDoc->m_nImageCnt++;
	Invalidate(bErase);

	return true;
}
	
// 2���� �÷� ������ ���
bool CImageProcessingView::DisplayCimage2DColor(BYTE **ImageRed, BYTE **ImageGreen, BYTE **ImageBlue, int nW, int nH, 
	int nPosX, int nPosY, bool bErase, bool bDelete, int Rate)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// bDelete�� true�̸� nPosX, nPosY��ġ�� ���� ������ ����
	if(bDelete)
	{
		int nCurrentImage = GetCurrentImagePos(CPoint(nPosX, nPosY)-GetScrollPosition());
		if(nCurrentImage >= 0)
		{
			delete [] pDoc->m_Image[nCurrentImage].Image1D;

			pDoc->m_nImageCnt--;
		}
	}

	int nCurrentImage = pDoc->m_nImageCnt;	// ���ο� ����(��� ����)

	if(nCurrentImage >= MAX_IMAGE) return false;

	int x, y, Pos;

	pDoc->m_Image[nCurrentImage].nW = nW*Rate/100;
	pDoc->m_Image[nCurrentImage].nH = nH*Rate/100;
	pDoc->m_Image[nCurrentImage].nPosX = nPosX;
	pDoc->m_Image[nCurrentImage].nPosY = nPosY;

	pDoc->m_Image[nCurrentImage].Image1D = new BYTE[GetBmp24Size(nW*Rate/100, nH*Rate/100)];

	for(y = 0 ; y < nH*Rate/100 ; y++)
		for(x = 0 ; x < nW*Rate/100 ; x++)
		{
			Pos = GetBmp24Pos(nW*Rate/100, nH*Rate/100, x, y);

			pDoc->m_Image[nCurrentImage].Image1D[Pos++] = ImageBlue[y*100/Rate][x*100/Rate];
			pDoc->m_Image[nCurrentImage].Image1D[Pos++] = ImageGreen[y*100/Rate][x*100/Rate];
			pDoc->m_Image[nCurrentImage].Image1D[Pos] = ImageRed[y*100/Rate][x*100/Rate];
		}

	pDoc->m_nImageCnt++;
	Invalidate(bErase);
	UpdateWindow();

	return true;
}

bool CImageProcessingView::DisplayIimage2D(int **ImageInt, int nW, int nH, int nPosX, int nPosY, 
	bool bErase, bool bDelete)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// bDelete�� true�̸� nPosX, nPosY��ġ�� ���� ������ ����
	if(bDelete)
	{
		int nCurrentImage = GetCurrentImagePos(CPoint(nPosX, nPosY)-GetScrollPosition());
		if(nCurrentImage >= 0)
		{
			delete [] pDoc->m_Image[nCurrentImage].Image1D;

			pDoc->m_nImageCnt--;
		}
	}

	int nCurrentImage = pDoc->m_nImageCnt;		// ���ο� ����(��� ����)

	if(nCurrentImage >= MAX_IMAGE) return false;

	int x, y, Pos;

	pDoc->m_Image[nCurrentImage].nW = nW;
	pDoc->m_Image[nCurrentImage].nH = nH;
	pDoc->m_Image[nCurrentImage].nPosX = nPosX;
	pDoc->m_Image[nCurrentImage].nPosY = nPosY;

	pDoc->m_Image[nCurrentImage].Image1D = new BYTE[GetBmp24Size(nW, nH)];

	int nMax = ImageInt[0][0];
	int nMin = ImageInt[0][0];
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(ImageInt[y][x] > nMax) nMax = ImageInt[y][x];
			if(ImageInt[y][x] < nMin) nMin = ImageInt[y][x];
		}

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			Pos = GetBmp24Pos(nW, nH, x, y);

			if(nMin == nMax)
			{
				pDoc->m_Image[nCurrentImage].Image1D[Pos++] = 0;
				pDoc->m_Image[nCurrentImage].Image1D[Pos++] = 0;
				pDoc->m_Image[nCurrentImage].Image1D[Pos] = 0;
			}
			else
			{
				pDoc->m_Image[nCurrentImage].Image1D[Pos++] = (BYTE)((ImageInt[y][x]-nMin)*(GRAY_CNT-1)/(nMax-nMin));
				pDoc->m_Image[nCurrentImage].Image1D[Pos++] = (BYTE)((ImageInt[y][x]-nMin)*(GRAY_CNT-1)/(nMax-nMin));
				pDoc->m_Image[nCurrentImage].Image1D[Pos] = (BYTE)((ImageInt[y][x]-nMin)*(GRAY_CNT-1)/(nMax-nMin));
			}
		}

	pDoc->m_nImageCnt++;
	Invalidate(bErase);

	return true;
}

bool CImageProcessingView::DisplayDimage2D(double **ImageDouble, int nW, int nH, int nPosX, int nPosY, 
	bool bErase, bool bDelete)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// bDelete�� true�̸� nPosX, nPosY��ġ�� ���� ������ ����
	if(bDelete)
	{
		int nCurrentImage = GetCurrentImagePos(CPoint(nPosX, nPosY)-GetScrollPosition());
		if(nCurrentImage >= 0)
		{
			delete [] pDoc->m_Image[nCurrentImage].Image1D;

			pDoc->m_nImageCnt--;
		}
	}

	int nCurrentImage = pDoc->m_nImageCnt;		// ���ο� ����(��� ����)

	if(nCurrentImage >= MAX_IMAGE) return false;

	int x, y, Pos;

	pDoc->m_Image[nCurrentImage].nW = nW;
	pDoc->m_Image[nCurrentImage].nH = nH;
	pDoc->m_Image[nCurrentImage].nPosX = nPosX;
	pDoc->m_Image[nCurrentImage].nPosY = nPosY;

	pDoc->m_Image[nCurrentImage].Image1D = new BYTE[GetBmp24Size(nW, nH)];

	double dMax = ImageDouble[0][0];
	double dMin = ImageDouble[0][0];
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			if(ImageDouble[y][x] > dMax) dMax = ImageDouble[y][x];
			if(ImageDouble[y][x] < dMin) dMin = ImageDouble[y][x];
		}

	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			Pos = GetBmp24Pos(nW, nH, x, y);

			if(dMin == dMax)
			{
				pDoc->m_Image[nCurrentImage].Image1D[Pos++] = 0;
				pDoc->m_Image[nCurrentImage].Image1D[Pos++] = 0;
				pDoc->m_Image[nCurrentImage].Image1D[Pos] = 0;
			}
			else
			{
				pDoc->m_Image[nCurrentImage].Image1D[Pos++] = (BYTE)((ImageDouble[y][x]-dMin)*(GRAY_CNT-1)/(dMax-dMin));
				pDoc->m_Image[nCurrentImage].Image1D[Pos++] = (BYTE)((ImageDouble[y][x]-dMin)*(GRAY_CNT-1)/(dMax-dMin));
				pDoc->m_Image[nCurrentImage].Image1D[Pos] = (BYTE)((ImageDouble[y][x]-dMin)*(GRAY_CNT-1)/(dMax-dMin));
			}
		}

	pDoc->m_nImageCnt++;
	Invalidate(bErase);

	return true;
}

// ���� ���콺 ��ư�� ������ ���� �ڵ鷯
void CImageProcessingView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_nCurrentImage = GetCurrentImageRect(point);

	m_CurrentPoint = point;

	if(m_nSelectRegion > 0)
	{
		m_rtSelectRegion.left = point.x;
		m_rtSelectRegion.top = point.y;
		m_PrevRect = CRect(0, 0, 0, 0);
		SetCapture();	// ���콺 ĸ��
	}
	else if(m_nCurrentImage > -1)
	{
		// ���� Ŭ���� ��ġ ����
		m_SaveStartPoint = point;
		m_PrevRect = CRect(0, 0, 0, 0);
		SetCapture();	// ���콺 ĸ��
	}
	
	CScrollView::OnLButtonDown(nFlags, point);
}

// ���� ���콺 ��ư�� ������ ���� �ڵ鷯
void CImageProcessingView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(m_nSelectRegion > 0)
	{
		m_rtSelectRegion.right = point.x;
		m_rtSelectRegion.bottom = point.y;

		SelOperation();

		m_nSelectRegion = 0;
		ReleaseCapture();	// ���콺 ĸ�� ����
	}
	else if(m_nCurrentImage > -1)
	{
		// ���� �̵�
		pDoc->m_Image[m_nCurrentImage].nPosX += point.x - m_SaveStartPoint.x;
		pDoc->m_Image[m_nCurrentImage].nPosY += point.y - m_SaveStartPoint.y;

		m_nCurrentImage = -1;

		ReleaseCapture();	// ���콺 ĸ�� ����
		Invalidate();		// �ٽ� �׸���
	}
	
	CScrollView::OnLButtonUp(nFlags, point);
}

// ���콺 �̵��� ���� �ڵ鷯
void CImageProcessingView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	CPoint CurrentScrollPoint= GetScrollPosition();

	if(m_nSelectRegion > 0 && (nFlags & MK_LBUTTON ))
	{
		CClientDC dc(this);

		CRect rt(m_rtSelectRegion.left, m_rtSelectRegion.top, point.x, point.y);

		CBrush *pOldBrush;

		dc.SetROP2(R2_NOT);
		pOldBrush = (CBrush *)dc.SelectStockObject(NULL_BRUSH);
		
		dc.Rectangle(m_PrevRect);
		dc.Rectangle(rt);

		dc.SelectObject(pOldBrush);

		m_PrevRect = rt;
	}
	else if(m_nCurrentImage > -1 && (nFlags & MK_LBUTTON ))	// ���� ���콺 ���� ��ư�� ������ ����
	{
		CClientDC dc(this);

		CRect rt(pDoc->m_Image[m_nCurrentImage].nPosX, pDoc->m_Image[m_nCurrentImage].nPosY, 
			pDoc->m_Image[m_nCurrentImage].nPosX + pDoc->m_Image[m_nCurrentImage].nW, pDoc->m_Image[m_nCurrentImage].nPosY+pDoc->m_Image[m_nCurrentImage].nH);

		rt -= CurrentScrollPoint;
		rt += point -m_SaveStartPoint;

		CBrush *pOldBrush;

		dc.SetROP2(R2_NOT);
		pOldBrush = (CBrush *)dc.SelectStockObject(NULL_BRUSH);
		
		// �̵� �簢�� ���
		dc.Rectangle(m_PrevRect);
		dc.Rectangle(rt);

		dc.SelectObject(pOldBrush);

		m_PrevRect = rt;
	}
	
	CScrollView::OnMouseMove(nFlags, point);
}

// ���� ���콺 ��ư�� ���� Ŭ���� ���� �ڵ鷯
void CImageProcessingView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	m_nCurrentImage = GetCurrentImageRect(point);

	if(m_nCurrentImage > -1) // point�� �����ϴ� �簢���� �ִٸ�
	{
		// ������ ����� ����
		ImageInfo SaveImage = pDoc->m_Image[m_nCurrentImage];
		for(int i = 0 ; i < pDoc->m_nImageCnt ; i++)
			if(i != m_nCurrentImage) delete [] pDoc->m_Image[i].Image1D;

		pDoc->m_Image[0] = SaveImage;
		pDoc->m_nImageCnt = 1;

		m_nCurrentImage = -1;

		Invalidate();
	}
	
	CScrollView::OnLButtonDblClk(nFlags, point);
}

// ȸ���� ����
void CImageProcessingView::OnGrayDuplicate() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray;

	// ���� ���� �б�
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);

	// ȸ���� ���� �б�
	GetCurrentImageGray(ImageGray);

	// ȸ���� ���� ���
	DisplayCimage2D(ImageGray, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);
}

// �÷� ����
void CImageProcessingView::OnColorDuplicate() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageRed, **ImageGreen, **ImageBlue;

	// ���� ���� �б�
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageRed = cmatrix(nH, nW);
	ImageGreen = cmatrix(nH, nW);
	ImageBlue = cmatrix(nH, nW);

	// �÷� ���� �б�
	GetCurrentImageColor(ImageRed, ImageGreen, ImageBlue);

	// �÷� ���� ���
	DisplayCimage2DColor(ImageRed, ImageGreen, ImageBlue, nW, nH, nPosX+nW, nPosY);

	free_cmatrix(ImageRed, nH, nW);	
	free_cmatrix(ImageGreen, nH, nW);	
	free_cmatrix(ImageBlue, nH, nW);	
}

void CImageProcessingView::OnChap2() 
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(!pMain->m_pChap2->m_bCreated)
	{
		pMain->m_pChap2->Create(CChap2::IDD, pMain);
		pMain->m_pChap2->ShowWindow(SW_SHOW);

		pMain->m_pChap2->m_bCreated = TRUE;
	}	
}

void CImageProcessingView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	menu.LoadMenu(IDR_CONTEXT);
	CMenu* pPopup = menu.GetSubMenu(0);
	
	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, 
		point.x, point.y, AfxGetMainWnd());	
}

void CImageProcessingView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	GetCurrentImageRect(point);

	Invalidate();
	
	m_PastePoint = point;
	
	CScrollView::OnRButtonDown(nFlags, point);
}

void CImageProcessingView::OnScaleX2() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **ImageGrayG, **ImageGrayB;
	BYTE **OutputGray, **OutputGrayG, **OutputGrayB;

	// ���� ���� �б�
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	ImageGrayG = cmatrix(nH, nW);
	ImageGrayB = cmatrix(nH, nW);

	OutputGray = cmatrix(nH*2, nW*2);
	OutputGrayG = cmatrix(nH*2, nW*2);
	OutputGrayB = cmatrix(nH*2, nW*2);

	// ȸ���� ���� �б�
	GetCurrentImageColor(ImageGray, ImageGrayG, ImageGrayB);

	int x, y;
	for(y = 0 ; y < nH*2 ; y++)
		for(x = 0 ; x < nW*2 ; x++)
		{
			OutputGray[y][x] = ImageGray[y/2][x/2];
			OutputGrayG[y][x] = ImageGrayG[y/2][x/2];
			OutputGrayB[y][x] = ImageGrayB[y/2][x/2];
		}

	// Ȯ��� ���� ���
	DisplayCimage2DColor(OutputGray, OutputGrayG, OutputGrayB, nW*2, nH*2, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);
	free_cmatrix(ImageGrayG, nH, nW);
	free_cmatrix(ImageGrayB, nH, nW);
	free_cmatrix(OutputGray, nH*2, nW*2);
	free_cmatrix(OutputGrayG, nH*2, nW*2);
	free_cmatrix(OutputGrayB, nH*2, nW*2);
}

void CImageProcessingView::OnScaleHalf() 
{
	int nW, nH, nPosX, nPosY;
	BYTE **ImageGray, **ImageGrayG, **ImageGrayB;
	BYTE **OutputGray, **OutputGrayG, **OutputGrayB;

	// ���� ���� �б�
	if(!GetCurrentImageInfo(&nW, &nH, &nPosX, &nPosY)) return;

	ImageGray = cmatrix(nH, nW);
	ImageGrayG = cmatrix(nH, nW);
	ImageGrayB = cmatrix(nH, nW);
	OutputGray = cmatrix(nH/2, nW/2);
	OutputGrayG = cmatrix(nH/2, nW/2);
	OutputGrayB = cmatrix(nH/2, nW/2);

	// ȸ���� ���� �б�
	GetCurrentImageColor(ImageGray, ImageGrayG, ImageGrayB);

	int x, y;
	for(y = 0 ; y < nH/2 ; y++)
		for(x = 0 ; x < nW/2 ; x++)
		{
			OutputGray[y][x] = ImageGray[y*2][x*2];
			OutputGrayG[y][x] = ImageGrayG[y*2][x*2];
			OutputGrayB[y][x] = ImageGrayB[y*2][x*2];
		}

	// ��ҵ� ���� ���
	DisplayCimage2DColor(OutputGray, OutputGrayG, OutputGrayB, nW/2, nH/2, nPosX+nW, nPosY);

	free_cmatrix(ImageGray, nH, nW);
	free_cmatrix(ImageGrayG, nH, nW);
	free_cmatrix(ImageGrayB, nH, nW);
	free_cmatrix(OutputGray, nH/2, nW/2);	
	free_cmatrix(OutputGrayG, nH/2, nW/2);	
	free_cmatrix(OutputGrayB, nH/2, nW/2);	
}

void CImageProcessingView::OnEditCopy() 
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_nImageCnt <= 0) return;

	HGLOBAL hGlobal;
	DWORD dwBitsSize;
	LPSTR pGlobal;

	int nW = pDoc->m_Image[pDoc->m_nImageCnt-1].nW;
	int nH = pDoc->m_Image[pDoc->m_nImageCnt-1].nH;
	UINT nSize = GetBmp24Size(nW, nH);

	dwBitsSize = sizeof(BITMAPINFOHEADER) + nSize;

	m_BmiHeader.biWidth = nW;
	m_BmiHeader.biHeight = nH;
	m_BmiHeader.biSizeImage = nSize;

	hGlobal = (HGLOBAL)GlobalAlloc(GHND, dwBitsSize);
	
	if(hGlobal == NULL) return;

	pGlobal = (LPSTR)GlobalLock((HGLOBAL)hGlobal);

	memcpy(pGlobal, &m_BmiHeader, sizeof(BITMAPINFOHEADER));

	memcpy(pGlobal + sizeof(BITMAPINFOHEADER), 
		pDoc->m_Image[pDoc->m_nImageCnt-1].Image1D, nSize); 

	if(OpenClipboard())
	{
		EmptyClipboard();

		SetClipboardData(CF_DIB, hGlobal);

		CloseClipboard();
	}

	GlobalUnlock(hGlobal);
}

void CImageProcessingView::OnEditCut() 
{
	SendMessage(WM_COMMAND, ID_EDIT_COPY);

	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if(pDoc->m_nImageCnt <= 0) return;

	delete [] pDoc->m_Image[pDoc->m_nImageCnt-1].Image1D;
	pDoc->m_nImageCnt--;

	Invalidate();
}

#define IS_WIN30_DIB(lpbi) ((*(LPDWORD)(lpbi)) == sizeof(BITMAPINFOHEADER))

WORD WINAPI DIBNumColors(LPSTR lpbi)
{
	WORD wBitCount;  // DIB bit count

	if (IS_WIN30_DIB(lpbi))
	{
		DWORD dwClrUsed;

		dwClrUsed = ((LPBITMAPINFOHEADER)lpbi)->biClrUsed;
		if (dwClrUsed != 0)
			return (WORD)dwClrUsed;
	}

	if (IS_WIN30_DIB(lpbi))
		wBitCount = ((LPBITMAPINFOHEADER)lpbi)->biBitCount;
	else
		wBitCount = ((LPBITMAPCOREHEADER)lpbi)->bcBitCount;

	/* return number of colors based on bits per pixel */
	switch (wBitCount)
	{
		case 1:
			return 2;

		case 4:
			return 16;

		case 8:
			return 256;

		default:
			return 0;
	}
}

WORD WINAPI PaletteSize(LPSTR lpbi)
{

   if (((LPBITMAPINFOHEADER)(lpbi))->biCompression == BI_BITFIELDS)
	   return (sizeof(DWORD) * 3) + (::DIBNumColors (lpbi) * sizeof (RGBQUAD));
   else
   {		
	   if (IS_WIN30_DIB (lpbi))
		  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBQUAD));
	   else
		  return (WORD)(::DIBNumColors(lpbi) * sizeof(RGBTRIPLE));
   }
}

LPSTR WINAPI FindDIBBits(LPSTR lpbi)
{
	return (lpbi + *(LPDWORD)lpbi + ::PaletteSize(lpbi));
}

void CImageProcessingView::OnEditPaste() 
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	HGLOBAL hGlobal;
	CPoint CurrentScrollPoint = GetScrollPosition();

	CPoint Pos;
	if(m_PastePoint != CPoint(-1, -1))
		Pos = m_PastePoint + CurrentScrollPoint;
	else
		Pos = CPoint(0, 0) + CurrentScrollPoint;

	m_PastePoint= CPoint(-1, -1);

	if(!IsClipboardFormatAvailable(CF_DIB)) return; 
	if(!OpenClipboard()) return;  
        
	hGlobal = (HGLOBAL)GetClipboardData(CF_DIB);
	
	if(hGlobal)
	{ 
		LPSTR pGlobal  = (LPSTR)GlobalLock((HGLOBAL)hGlobal);

		if(pDoc->m_nImageCnt >= MAX_IMAGE ||
			*(LPDWORD)(pGlobal) != sizeof(BITMAPINFOHEADER)) {
			GlobalUnlock(hGlobal); 

			CloseClipboard(); 
			return;
		}

		int nH = ((LPBITMAPINFOHEADER)pGlobal)->biHeight;	
		int nW = ((LPBITMAPINFOHEADER)pGlobal)->biWidth;

		int biCompression = ((LPBITMAPINFOHEADER)pGlobal)->biCompression;

		int BitsPixel = ((LPBITMAPINFOHEADER)pGlobal)->biBitCount;

//		if(BitsPixel != 24 && BitsPixel != 32 && BitsPixel != 16) {
//			GlobalUnlock(hGlobal); 

//			CloseClipboard(); 
//			return;
//		}

		pDoc->m_Image[pDoc->m_nImageCnt].Image1D = new unsigned char[GetBmp24Size(nW, nH)];
		pDoc->m_Image[pDoc->m_nImageCnt].nW = nW;
		pDoc->m_Image[pDoc->m_nImageCnt].nH = nH;
		pDoc->m_Image[pDoc->m_nImageCnt].nPosX = Pos.x;
		pDoc->m_Image[pDoc->m_nImageCnt].nPosY = Pos.y;

		if(BitsPixel == 24)
		{
			memcpy(pDoc->m_Image[pDoc->m_nImageCnt].Image1D, pGlobal+sizeof(BITMAPINFOHEADER), GetBmp24Size(nW, nH));
		}
		else if(BitsPixel == 32)
		{
			int x, y;
			int offset, offset2;
			DWORD dwPixelValue;

			LPSTR lpDIBBits = (LPSTR)::FindDIBBits(pGlobal);
			
			for(y = 0 ; y < nH ; y++)
			{
				offset2 = (nW*1) * (nH-y-1);
				for(x = 0 ; x < nW ; x++)
				{
					offset = GetBmp24Pos(nW, nH, x, y);

					//dwPixelValue = ((DWORD *)(pGlobal+sizeof(BITMAPINFOHEADER)))[offset2+x];
					dwPixelValue = ((DWORD *)lpDIBBits)[offset2+x];

					pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset++] = (BYTE)dwPixelValue;
					pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset++] = (BYTE)(dwPixelValue>>8);
					pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset] = (BYTE)(dwPixelValue>>16);
				}
			}
		}
		else if(BitsPixel == 16)
		{
			int x, y;
			int offset, offset2;
			WORD dwPixelValue;

			LPSTR lpDIBBits = (LPSTR)::FindDIBBits(pGlobal);
			
			for(y = 0 ; y < nH ; y++)
			{
				offset2 = (nW+1)/2*2 * (nH-y-1);
				
				for(x = 0 ; x < nW ; x++)
				{
					offset = GetBmp24Pos(nW, nH, x, y);

					//dwPixelValue = ((WORD *)(pGlobal+sizeof(BITMAPINFOHEADER)))[offset2+x];
					dwPixelValue = ((WORD *)lpDIBBits)[offset2+x];

					if(biCompression == BI_BITFIELDS)
					{
						pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset++] = (BYTE)(((dwPixelValue>>0) & 0x001F) * 8);
						pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset++] = (BYTE)(((dwPixelValue>>5) & 0x003F) * 4);
						pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset] = (BYTE)(((dwPixelValue>>11) & 0x001F) * 8);
					}
					else
					{
						pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset++] = (BYTE)(((dwPixelValue>>0) & 0x001F) * 8);
						pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset++] = (BYTE)(((dwPixelValue>>5) & 0x001F) * 8);
						pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset] = (BYTE)(((dwPixelValue>>10) & 0x001F) * 8);
					}
				}
			}
		}
		else
		{
			LPSTR lpDIBPal = pGlobal + sizeof(BITMAPINFOHEADER);
			LPSTR lpDIBBits = (LPSTR)::FindDIBBits(pGlobal);

			int x, y;
			int offset, offset2, i, pix;
			BYTE inbyte;
			int bytesread;

			UINT mask = (1 << BitsPixel) - 1;

			i = 0;
			bytesread = 0;
			for(y = nH-1 ; y >= 0 ; y--)
			{
				offset = (nW*3+3)/4*4 * (nH-1-y);

				int bit_count = 0;
				for(x = 0 ; x < nW ; x++)
				{
					if (bit_count <= 0) {
						bit_count = 8;
						//inbyte = ((BYTE *)(pGlobal+sizeof(BITMAPINFOHEADER)))[i++];
						inbyte = ((BYTE *)lpDIBBits)[i++];
						bytesread++;
					}
					bit_count -= BitsPixel;
					pix = ( inbyte >> bit_count) & mask;

					offset2 = offset + 3*x;
					
					pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset2++] = lpDIBPal[pix*4];
					pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset2++] = lpDIBPal[pix*4+1];
					pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset2] = lpDIBPal[pix*4+2];
				}

				while (bytesread&3) {
					i++;
					bytesread++;
				}
			}
		}

		pDoc->m_nImageCnt++;
		Invalidate();

		GlobalUnlock(hGlobal);  
	} 
    
    CloseClipboard(); 
}

void CImageProcessingView::OnChap3() 
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(!pMain->m_pChap3->m_bCreated)
	{
		pMain->m_pChap3->Create(CChap3::IDD, pMain);
		pMain->m_pChap3->ShowWindow(SW_SHOW);

		pMain->m_pChap3->m_bCreated = TRUE;
	}
}

void CImageProcessingView::OnChap4() 
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(!pMain->m_pChap4->m_bCreated)
	{
		pMain->m_pChap4->Create(CChap4::IDD, pMain);
		pMain->m_pChap4->ShowWindow(SW_SHOW);

		pMain->m_pChap4->m_bCreated = TRUE;
	}	
}

void CImageProcessingView::OnChap5() 
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(!pMain->m_pChap5->m_bCreated)
	{
		pMain->m_pChap5->Create(CChap5::IDD, pMain);
		pMain->m_pChap5->ShowWindow(SW_SHOW);

		pMain->m_pChap5->m_bCreated = TRUE;
	}	
}

void CImageProcessingView::OnApplication() 
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if(!pMain->m_pApplication->m_bCreated)
	{
		pMain->m_pApplication->Create(CApplication::IDD, pMain);
		pMain->m_pApplication->ShowWindow(SW_SHOW);

		pMain->m_pApplication->m_bCreated = TRUE;
	}	
}

BYTE CImageProcessingView::GetCurrPosColor(int x, int y, int Color)// Color 0 : Red, 1 : Green, 2 : Blue
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	int i, offset;

	for (i = pDoc->m_nImageCnt-1; i >= 0 ; i--) 
	{
		if(	pDoc->m_Image[i].nPosX <= x && 
			pDoc->m_Image[i].nPosX + pDoc->m_Image[i].nW > x &&
			pDoc->m_Image[i].nPosY <= y && 
			pDoc->m_Image[i].nPosY + pDoc->m_Image[i].nH > y)
		{
			offset = GetBmp24Pos(pDoc->m_Image[i].nW, pDoc->m_Image[i].nH, x-pDoc->m_Image[i].nPosX, y-pDoc->m_Image[i].nPosY);

			if(Color == 0) return pDoc->m_Image[i].Image1D[offset+2];
			else if(Color == 1) return pDoc->m_Image[i].Image1D[offset+1];
			else return pDoc->m_Image[i].Image1D[offset];
		}
	}

	if(Color == 0) return (BYTE)(0x00FFFFFF & 0x000000FF);
	else if(Color == 1) return (BYTE)((0x00FFFFFF & 0x0000FF00)/0x100);
	else return (BYTE)((0x00FFFFFF & 0x00FF0000)/0x10000);
}

void CImageProcessingView::GetCurrPosColor1D(int x, int y, BYTE *Image1D)
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	int i, offset;

	for (i = pDoc->m_nImageCnt-1; i >= 0 ; i--) 
	{
		if(	pDoc->m_Image[i].nPosX <= x && 
			pDoc->m_Image[i].nPosX + pDoc->m_Image[i].nW > x &&
			pDoc->m_Image[i].nPosY <= y && 
			pDoc->m_Image[i].nPosY + pDoc->m_Image[i].nH > y)
		{
			offset = GetBmp24Pos(pDoc->m_Image[i].nW, pDoc->m_Image[i].nH, x-pDoc->m_Image[i].nPosX, y-pDoc->m_Image[i].nPosY);

			*Image1D = pDoc->m_Image[i].Image1D[offset];
			*(Image1D+1) = pDoc->m_Image[i].Image1D[offset+1];
			*(Image1D+2) = pDoc->m_Image[i].Image1D[offset+2];

			return;
		}
	}

	*Image1D = 255;
	*(Image1D+1) = 255;
	*(Image1D+2) = 255;
}

void CImageProcessingView::SelOperation()
{
	CImageProcessingDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	int nW, nH;
	int x, y, offset;
	CPoint Pos;

	if(m_nSelectRegion == 1)
	{
		nW = m_rtSelectRegion.Width()+1;
		nH = m_rtSelectRegion.Height()+1;
	}
	else if(m_nSelectRegion == 2)
	{
		nW = (m_rtSelectRegion.Width()+1)*2;
		nH = (m_rtSelectRegion.Height()+1)*2;
	}

	CPoint CurrentScrollPoint = GetScrollPosition();

	pDoc->m_Image[pDoc->m_nImageCnt].Image1D = new unsigned char[GetBmp24Size(nW, nH)];
	pDoc->m_Image[pDoc->m_nImageCnt].nW = nW;
	pDoc->m_Image[pDoc->m_nImageCnt].nH = nH;
	pDoc->m_Image[pDoc->m_nImageCnt].nPosX = m_rtSelectRegion.left+CurrentScrollPoint.x;
	pDoc->m_Image[pDoc->m_nImageCnt].nPosY = m_rtSelectRegion.top+CurrentScrollPoint.y;

	Pos = CPoint(m_rtSelectRegion.left+CurrentScrollPoint.x, m_rtSelectRegion.top+CurrentScrollPoint.y);
	
	for(y = 0 ; y < nH ; y++)
		for(x = 0 ; x < nW ; x++)
		{
			offset = GetBmp24Pos(nW, nH, x, y);
			if(m_nSelectRegion == 1)
			{
				pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset++] = GetCurrPosColor(x+Pos.x, y+Pos.y, 2);
				pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset++] = GetCurrPosColor(x+Pos.x, y+Pos.y, 1);
				pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset] = GetCurrPosColor(x+Pos.x, y+Pos.y, 0);	
			}
			else if(m_nSelectRegion == 2)
			{
				pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset++] = GetCurrPosColor(x/2+Pos.x, y/2+Pos.y, 2);
				pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset++] = GetCurrPosColor(x/2+Pos.x, y/2+Pos.y, 1);
				pDoc->m_Image[pDoc->m_nImageCnt].Image1D[offset] = GetCurrPosColor(x/2+Pos.x, y/2+Pos.y, 0);	
			}
		}
	
	pDoc->m_nImageCnt++;
	Invalidate();
}

void CImageProcessingView::OnSelRegion() 
{
	m_nSelectRegion = 1;
}

void CImageProcessingView::OnSelRegionZoom() 
{
	m_nSelectRegion = 2;	
}

void CImageProcessingView::OnSum() 
{
	ImageOperation(0);
}

void CImageProcessingView::OnSubtract() 
{
	ImageOperation(1);
}

void CImageProcessingView::OnDifference() 
{
	ImageOperation(4);	
}


void CImageProcessingView::OnImagestitchJy()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if (!pMain->m_pImageStitching->m_bCreated)
	{
		pMain->m_pImageStitching->Create(ImageStitch_jy::IDD, pMain);
		pMain->m_pImageStitching->ShowWindow(SW_SHOW);

		pMain->m_pImageStitching->m_bCreated = TRUE;
	}
}


void CImageProcessingView::OnVideostitch()
{
	CMainFrame *pMain = (CMainFrame *)AfxGetMainWnd();
	if (!pMain->m_pVideoStitch->m_bCreated)
	{
		pMain->m_pVideoStitch->Create(CVideoStitch::IDD, pMain);
		pMain->m_pVideoStitch->ShowWindow(SW_SHOW);

		pMain->m_pVideoStitch->m_bCreated = TRUE;
	}
}
