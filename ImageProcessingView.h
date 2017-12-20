// ImageProcessingView.h : interface of the CImageProcessingView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSINGVIEW_H__151AC17E_900E_4EA2_AF8E_742CDB056A89__INCLUDED_)
#define AFX_IMAGEPROCESSINGVIEW_H__151AC17E_900E_4EA2_AF8E_742CDB056A89__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CImageProcessingView : public CScrollView
{
protected: // create from serialization only
	CImageProcessingView();
	DECLARE_DYNCREATE(CImageProcessingView)

// Attributes
public:
	CImageProcessingDoc* GetDocument();

// Operations
public:
	BITMAPINFOHEADER m_BmiHeader;	// ȭ�� ����� ���� ��Ʈ�� ���
	int m_nCurrentImage;			// ���콺 ó���� ����, ���� ���� ��ȣ
	CPoint m_SaveStartPoint;		// ���콺 ó���� ����, ���� Ŭ���� ��ġ
	CRect m_PrevRect;				// ���콺 ó���� ����, ������ ǥ�� ���� �簢�� ����
	CPoint m_PastePoint;

	CPoint m_CurrentPoint;

	CString m_ViewMsg;

	int m_nSelectRegion;
	CRect m_rtSelectRegion;
	
	int GetCurrentImageRect(CPoint point);	// point ��ǥ�� �����ϴ� ������ ���� �������� ����
	int GetCurrentImagePos(CPoint point);	// point ��ǥ�� (�»�) ��ġ�� ������ ������ ���� �������� ����

	// ������ ������ ��, ����, ��ġ �б�
	bool GetCurrentImageInfo(int *pW, int *pH, int *pPosX, int *pPosY, int nIndex = 0);
	// ������ ������ 2���� ȸ���� ������ �б�
	bool GetCurrentImageGray(BYTE **ImageGray, int nIndex = 0);
	// 2���� ȸ���� ������ ���
	bool DisplayCimage2D(BYTE **ImageGray, int nW, int nH, int nPosX, int nPosY, bool bErase = true, bool bDelete = false, int Rate = 100);
	bool CImageProcessingView::DisplayCimage1D(BYTE *Image1D, int nW, int nH, int nPosX, int nPosY, bool bErase, bool bDelete);
	// ������ ������ 2���� �÷� ������ �б�
	bool GetCurrentImageColor(BYTE **ImageRed, BYTE **ImageGreen, BYTE **ImageBlue, int nIndex = 0);
	bool GetCurrentImage1DColor(BYTE *Image1D, int nIndex = 0);
	// 2���� �÷� ������ ���
	bool DisplayCimage2DColor(BYTE **ImageRed, BYTE **ImageGreen, BYTE **ImageBlue, int nW, int nH, 
		int nPosX, int nPosY, bool bErase = true, bool bDelete = false, int Rate = 100);

	bool DisplayIimage2D(int **ImageInt, int nW, int nH, int nPosX, int nPosY, bool bErase = true, bool bDelete = false);
	bool DisplayDimage2D(double **ImageDouble, int nW, int nH, int nPosX, int nPosY, bool bErase = true, bool bDelete = false);

	BYTE GetCurrPosColor(int x, int y, int Color);
	void GetCurrPosColor1D(int x, int y, BYTE *Image1D);
	void SelOperation();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessingView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageProcessingView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessingView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnGrayDuplicate();
	afx_msg void OnColorDuplicate();
	afx_msg void OnChap2();
	afx_msg void OnChap3();
	afx_msg void OnScaleX2();
	afx_msg void OnScaleHalf();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnChap4();
	afx_msg void OnChap5();
	afx_msg void OnApplication();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelRegion();
	afx_msg void OnSelRegionZoom();
	afx_msg void OnSum();
	afx_msg void OnSubtract();
	afx_msg void OnDifference();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnImagestitchJy();
	afx_msg void OnVideostitch();
};

#ifndef _DEBUG  // debug version in ImageProcessingView.cpp
inline CImageProcessingDoc* CImageProcessingView::GetDocument()
   { return (CImageProcessingDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSINGVIEW_H__151AC17E_900E_4EA2_AF8E_742CDB056A89__INCLUDED_)
