// BrowserHookerView.h : interface of the CBrowserHookerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BROWSERHOOKERVIEW_H__B30675D6_E171_4880_8B3D_70328EE82636__INCLUDED_)
#define AFX_BROWSERHOOKERVIEW_H__B30675D6_E171_4880_8B3D_70328EE82636__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBrowserHookerView : public CView
{
protected: // create from serialization only
	CBrowserHookerView();
	DECLARE_DYNCREATE(CBrowserHookerView)

// Attributes
public:
	CBrowserHookerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBrowserHookerView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBrowserHookerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBrowserHookerView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in BrowserHookerView.cpp
inline CBrowserHookerDoc* CBrowserHookerView::GetDocument()
   { return (CBrowserHookerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BROWSERHOOKERVIEW_H__B30675D6_E171_4880_8B3D_70328EE82636__INCLUDED_)
