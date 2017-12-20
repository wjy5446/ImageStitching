// ImageProcessingDoc.cpp : implementation of the CImageProcessingDoc class
//

#include "stdafx.h"
#include "ImageProcessing.h"

#include "ImageProcessingDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingDoc

IMPLEMENT_DYNCREATE(CImageProcessingDoc, CDocument)

BEGIN_MESSAGE_MAP(CImageProcessingDoc, CDocument)
	//{{AFX_MSG_MAP(CImageProcessingDoc)
	ON_COMMAND(ID_FILE_OPEN_CUR, OnFileOpenCur)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingDoc construction/destruction

CImageProcessingDoc::CImageProcessingDoc()
{
	// ���� â�� ������ �ʱ⿡ 0���� ����
	m_nImageCnt = 0;
}

CImageProcessingDoc::~CImageProcessingDoc()
{
	// ���� â�� ���� ���� ���� �޸𸮸� ����
	for(int i = 0 ; i < m_nImageCnt ; i++)
		delete [] m_Image[i].Image1D;
}

BOOL CImageProcessingDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CImageProcessingDoc serialization

void CImageProcessingDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingDoc diagnostics

#ifdef _DEBUG
void CImageProcessingDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CImageProcessingDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CImageProcessingDoc commands

BOOL CImageProcessingDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// â�� ǥ���� ���� Ȯ��
	if(m_nImageCnt >= MAX_IMAGE) return FALSE;

	CString FileName = lpszPathName;
	FileName.MakeUpper();

	// ��Ʈ�� ����
	if(FileName.Right(4) == _T(".BMP"))
	{
		// ���� �б�
		m_Image[m_nImageCnt].Image1D = ReadBmp(lpszPathName, &m_Image[m_nImageCnt].nW, &m_Image[m_nImageCnt].nH);

		if(m_Image[m_nImageCnt].Image1D == NULL) return FALSE;

		m_Image[m_nImageCnt].nPosX = 0;
		m_Image[m_nImageCnt].nPosY = 0;

		m_nImageCnt++;
	}
	// 256*256 ����
	else if(FileName.Right(4) == _T(".256"))
	{
		m_Image[m_nImageCnt].nW = 256;
		m_Image[m_nImageCnt].nH = 256;

		// ���� �б�
		m_Image[m_nImageCnt].Image1D = ReadRaw(lpszPathName, m_Image[m_nImageCnt].nW, m_Image[m_nImageCnt].nH);

		if(m_Image[m_nImageCnt].Image1D == NULL) return FALSE;

		m_Image[m_nImageCnt].nPosX = 0;
		m_Image[m_nImageCnt].nPosY = 0;

		m_nImageCnt++;
	}
	// 320*240 ����
	else if(FileName.Right(4) == _T(".320"))
	{
		m_Image[m_nImageCnt].nW = 320;
		m_Image[m_nImageCnt].nH = 240;

		// ���� �б� 
		m_Image[m_nImageCnt].Image1D = ReadRaw(lpszPathName, m_Image[m_nImageCnt].nW, m_Image[m_nImageCnt].nH);

		if(m_Image[m_nImageCnt].Image1D == NULL) return FALSE;

		m_Image[m_nImageCnt].nPosX = 0;
		m_Image[m_nImageCnt].nPosY = 0;

		m_nImageCnt++;
	}
	// PCX ����
	else if(FileName.Right(4) == _T(".PCX"))
	{
		// ���� �б� 
		m_Image[m_nImageCnt].Image1D = ReadPcx(lpszPathName, &m_Image[m_nImageCnt].nW, &m_Image[m_nImageCnt].nH);

		if(m_Image[m_nImageCnt].Image1D == NULL) return FALSE;

		m_Image[m_nImageCnt].nPosX = 0;
		m_Image[m_nImageCnt].nPosY = 0;

		m_nImageCnt++;
	}
	// JPEG ����
	else if(FileName.Right(4) == _T(".JPG"))
	{
		// ���� �б� 
		m_Image[m_nImageCnt].Image1D = ReadJpeg(lpszPathName, &m_Image[m_nImageCnt].nW, &m_Image[m_nImageCnt].nH);

		if(m_Image[m_nImageCnt].Image1D == NULL) return FALSE;

		m_Image[m_nImageCnt].nPosX = 0;
		m_Image[m_nImageCnt].nPosY = 0;

		m_nImageCnt++;
	}
	else
		return FALSE;
	
	return TRUE;
}

BOOL CImageProcessingDoc::OnSaveDocument(LPCTSTR lpszPathName) 
{
	// â�� ǥ�õ� ���� Ȯ��
	if(m_nImageCnt <= 0) return FALSE;

	CString FileName = lpszPathName;
	FileName.MakeUpper();

	// ��Ʈ�� ���� ����
	if(FileName.Right(4) == _T(".BMP"))
	{
		return SaveBmp(lpszPathName, m_Image[m_nImageCnt-1].Image1D, m_Image[m_nImageCnt-1].nW, m_Image[m_nImageCnt-1].nH);
	}
	// JPEG ���� ����
	else if(FileName.Right(4) == _T(".JPG"))
	{
		return SaveJpeg(lpszPathName, m_Image[m_nImageCnt-1].Image1D, m_Image[m_nImageCnt-1].nW, m_Image[m_nImageCnt-1].nH,
			true, 75);
	}
	
	return FALSE;
//	return CDocument::OnSaveDocument(lpszPathName);
}

void CImageProcessingDoc::OnFileOpenCur() 
{
	CFileDialog dlg(TRUE, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("Image Files(*.bmp, *.pcx, *.jpg, *.320, *256)|\
*.bmp;*.pcx;*.jpg;*.320;*.256|\
BMP Files(*.bmp)|*.bmp|PCX Files(*.pcx)|*.pcx|\
JPEG Files(*.jpg)|*.jpg|RAW Files(*.320, *.256)|\
*.320;*256|All Files(*.*)|*.*||"));
	
	if(dlg.DoModal() != IDOK) return;

	FileOpenCur(dlg.GetPathName());
}

void CImageProcessingDoc::FileOpenCur(CString FilePathName) 
{
	// â�� ǥ���� ���� Ȯ��
	if(m_nImageCnt >= MAX_IMAGE) return;

	CString FileName = FilePathName;
	FileName.MakeUpper();

	// ��Ʈ�� ����
	if(FileName.Right(4) == _T(".BMP"))
	{
		// ���� �б�
		m_Image[m_nImageCnt].Image1D = ReadBmp(FileName, &m_Image[m_nImageCnt].nW, &m_Image[m_nImageCnt].nH);

		if(m_Image[m_nImageCnt].Image1D == NULL) return;

		m_Image[m_nImageCnt].nPosX = 0;
		m_Image[m_nImageCnt].nPosY = 0;

		m_nImageCnt++;
	}
	// 256*256 ����
	else if(FileName.Right(4) == _T(".256"))
	{
		m_Image[m_nImageCnt].nW = 256;
		m_Image[m_nImageCnt].nH = 256;

		// ���� �б�
		m_Image[m_nImageCnt].Image1D = ReadRaw(FileName, m_Image[m_nImageCnt].nW, m_Image[m_nImageCnt].nH);

		if(m_Image[m_nImageCnt].Image1D == NULL) return;

		m_Image[m_nImageCnt].nPosX = 0;
		m_Image[m_nImageCnt].nPosY = 0;

		m_nImageCnt++;
	}
	// 320*240 ����
	else if(FileName.Right(4) == _T(".320"))
	{
		m_Image[m_nImageCnt].nW = 320;
		m_Image[m_nImageCnt].nH = 240;

		// ���� �б� 
		m_Image[m_nImageCnt].Image1D = ReadRaw(FileName, m_Image[m_nImageCnt].nW, m_Image[m_nImageCnt].nH);

		if(m_Image[m_nImageCnt].Image1D == NULL) return;

		m_Image[m_nImageCnt].nPosX = 0;
		m_Image[m_nImageCnt].nPosY = 0;

		m_nImageCnt++;
	}
	// PCX ����
	else if(FileName.Right(4) == _T(".PCX"))
	{
		// ���� �б� 
		m_Image[m_nImageCnt].Image1D = ReadPcx(FileName, &m_Image[m_nImageCnt].nW, &m_Image[m_nImageCnt].nH);

		if(m_Image[m_nImageCnt].Image1D == NULL) return;

		m_Image[m_nImageCnt].nPosX = 0;
		m_Image[m_nImageCnt].nPosY = 0;

		m_nImageCnt++;
	}
	// JPEG ����
	else if(FileName.Right(4) == _T(".JPG"))
	{
		// ���� �б� 
		m_Image[m_nImageCnt].Image1D = ReadJpeg(FileName, &m_Image[m_nImageCnt].nW, &m_Image[m_nImageCnt].nH);

		if(m_Image[m_nImageCnt].Image1D == NULL) return;

		m_Image[m_nImageCnt].nPosX = 0;
		m_Image[m_nImageCnt].nPosY = 0;

		m_nImageCnt++;
	}
	else 
		return;

	UpdateAllViews (NULL);	
}
