// RiListView.cpp : implementation of the CRiListView class
//

#include "stdafx.h"
#include "RiKB\Ri.h"

#include "Chelovek.h"

#include "RiDoc.h"
#include "RiListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRiListView

IMPLEMENT_DYNCREATE(CRiListView, CListView)

BEGIN_MESSAGE_MAP(CRiListView, CListView)
	//{{AFX_MSG_MAP(CRiListView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CListView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRiListView construction/destruction

CRiListView::CRiListView()
{
	// TODO: add construction code here

}

CRiListView::~CRiListView()
{
}

BOOL CRiListView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRiListView drawing

void CRiListView::OnDraw(CDC* pDC)
{
	CRiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CRiListView printing

BOOL CRiListView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRiListView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRiListView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRiListView diagnostics

#ifdef _DEBUG
void CRiListView::AssertValid() const
{
	CListView::AssertValid();
}

void CRiListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CRiDoc* CRiListView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRiDoc)));
	return (CRiDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRiListView message handlers

void CRiListView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	
}
