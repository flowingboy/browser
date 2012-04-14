// BrowserHookerView.cpp : implementation of the CBrowserHookerView class
//

#include "stdafx.h"
#include "BrowserHooker.h"

#include "BrowserHookerDoc.h"
#include "BrowserHookerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerView

IMPLEMENT_DYNCREATE(CBrowserHookerView, CView)

BEGIN_MESSAGE_MAP(CBrowserHookerView, CView)
	//{{AFX_MSG_MAP(CBrowserHookerView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerView construction/destruction

CBrowserHookerView::CBrowserHookerView()
{
	// TODO: add construction code here

}

CBrowserHookerView::~CBrowserHookerView()
{
}

BOOL CBrowserHookerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerView drawing

void CBrowserHookerView::OnDraw(CDC* pDC)
{
	CBrowserHookerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerView printing

BOOL CBrowserHookerView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CBrowserHookerView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CBrowserHookerView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerView diagnostics

#ifdef _DEBUG
void CBrowserHookerView::AssertValid() const
{
	CView::AssertValid();
}

void CBrowserHookerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CBrowserHookerDoc* CBrowserHookerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CBrowserHookerDoc)));
	return (CBrowserHookerDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerView message handlers
