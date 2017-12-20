// ImageProcessingDoc.h : interface of the CImageProcessingDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSINGDOC_H__2227DE75_C442_4BB1_912D_5B0F3847407B__INCLUDED_)
#define AFX_IMAGEPROCESSINGDOC_H__2227DE75_C442_4BB1_912D_5B0F3847407B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"		
// â�� ǥ���� �ִ� ������ ����
#define MAX_IMAGE		1000

class CImageProcessingDoc : public CDocument
{
protected: // create from serialization only
	CImageProcessingDoc();
	DECLARE_DYNCREATE(CImageProcessingDoc)

// Attributes
public:
	void FileOpenCur(CString FilePathName);

// Operations
public:
	ImageInfo m_Image[MAX_IMAGE];	// ���� �迭
	int m_nImageCnt;				// ���� â�� ǥ���� �ִ� ����

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessingDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CImageProcessingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessingDoc)
	afx_msg void OnFileOpenCur();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSINGDOC_H__2227DE75_C442_4BB1_912D_5B0F3847407B__INCLUDED_)
