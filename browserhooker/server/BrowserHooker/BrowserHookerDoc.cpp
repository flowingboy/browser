// BrowserHookerDoc.cpp : implementation of the CBrowserHookerDoc class
//

#include "stdafx.h"
#include "BrowserHooker.h"

#include "BrowserHookerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerDoc

IMPLEMENT_DYNCREATE(CBrowserHookerDoc, CDocument)

BEGIN_MESSAGE_MAP(CBrowserHookerDoc, CDocument)
	//{{AFX_MSG_MAP(CBrowserHookerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerDoc construction/destruction

CBrowserHookerDoc::CBrowserHookerDoc()
{
	// TODO: add one-time construction code here

}

CBrowserHookerDoc::~CBrowserHookerDoc()
{
}

BOOL CBrowserHookerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerDoc serialization

void CBrowserHookerDoc::Serialize(CArchive& ar)
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
// CBrowserHookerDoc diagnostics

#ifdef _DEBUG
void CBrowserHookerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBrowserHookerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBrowserHookerDoc commands
