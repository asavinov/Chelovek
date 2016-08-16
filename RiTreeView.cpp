// CRiTreeView.cpp : implementation of the CRiTreeView class
//

#include "stdafx.h"
#include "Lk\Lk.h"
#include "Kb\Kb.h"

#include "Chelovek.h"

#include "RiDoc.h"
#include "RiTreeView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRiTreeView

IMPLEMENT_DYNCREATE(CRiTreeView, CTreeView)

BEGIN_MESSAGE_MAP(CRiTreeView, CTreeView)
	//{{AFX_MSG_MAP(CRiTreeView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRiTreeView construction/destruction

CRiTreeView::CRiTreeView()
{
	// TODO: add construction code here

}

CRiTreeView::~CRiTreeView()
{

}

BOOL CRiTreeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

  //
  // Change style of the tree control
  //
  cs.style |= TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS;

	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CRiTreeView drawing

void CRiTreeView::OnDraw(CDC* pDC)
{
	CRiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here

}

/////////////////////////////////////////////////////////////////////////////
// CRiTreeView printing

BOOL CRiTreeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CRiTreeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CRiTreeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CRiTreeView diagnostics

#ifdef _DEBUG
void CRiTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CRiTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CRiDoc* CRiTreeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRiDoc)));
	return (CRiDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRiTreeView message handlers

void CRiTreeView::OnInitialUpdate() 
{
	CTreeView::OnInitialUpdate();
	
	// TODO: Add your specialized code here and/or call the base class
	

}

void CRiTreeView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class

	CTreeView::OnUpdate( pSender, lHint, pHint);

	CRiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

  CTreeCtrl& Tree = GetTreeCtrl();

  // Empty tree control
  Tree.DeleteAllItems();

  //
  // Insert standard elements of any KB tree
  //
  HTREEITEM ItemKB = Tree.InsertItem( "Knowledge Base", TVI_ROOT, TVI_LAST );
  HTREEITEM ItemSyn = Tree.InsertItem( "Syntax", ItemKB, TVI_LAST );
  HTREEITEM ItemSem = Tree.InsertItem( "Semantics", ItemKB, TVI_LAST );
  HTREEITEM ItemDat = Tree.InsertItem( "Data Source", ItemKB, TVI_LAST );

  Tree.SetItemImage( ItemKB, IDB_TREE_KB-IDB_TREE_FIRST, IDB_TREE_KB_SEL-IDB_TREE_FIRST );
  Tree.SetItemImage( ItemSyn, IDB_TREE_SYN-IDB_TREE_FIRST, IDB_TREE_SYN_SEL-IDB_TREE_FIRST );
  Tree.SetItemImage( ItemSem, IDB_TREE_SEM-IDB_TREE_FIRST, IDB_TREE_SEM_SEL-IDB_TREE_FIRST );
  Tree.SetItemImage( ItemDat, IDB_TREE_DS-IDB_TREE_FIRST, IDB_TREE_DS_SEL-IDB_TREE_FIRST );

  char ItemName[4096];
  HTREEITEM VarItem, ValItem;
  RiCoord var, val;

  //
  // Insert KB tree syntax elements from the document (attributes and values)
  //
  for ( var = 0; var < pDoc->KB->VarNumber(); var++ ) {

    // Get variable description
    pDoc->KB->VarGetName( var, ItemName );

    // Form item (bitmap etc.)

    // and insert it into syntax subtree
    VarItem = Tree.InsertItem( ItemName, ItemSyn, TVI_LAST );
    Tree.SetItemImage( VarItem, IDB_TREE_VAR-IDB_TREE_FIRST, IDB_TREE_VAR_SEL-IDB_TREE_FIRST );

    // Then add all values for this variable
    for ( val = 0; val < pDoc->KB->ValNumber(var); val++ ) {

      // Get value description
      pDoc->KB->ValGetName( var, val, ItemName );

      // and insert it into the current value subtree
      ValItem = Tree.InsertItem( ItemName, VarItem, TVI_LAST );
      Tree.SetItemImage( ValItem, IDB_TREE_VAL-IDB_TREE_FIRST, IDB_TREE_VAL_SEL-IDB_TREE_FIRST );

      }

    }

  //
  // Insert KB tree semantic elements from the document (rules)
  //
  RiLine line;
  HTREEITEM RuleItem, PropItem, CompItem;
  RiLine RuleNum = 0;
  RiCoord GoalVar = pDoc->KB->OptInduction.GoalVar;

  if ( pDoc->KB->Rules != NULL ) {
    RuleNum = pDoc->KB->Rules->Lines;
    }

  for ( line = 0; line < RuleNum; line++ ) {

    // Get rule description
    pDoc->KB->Rules->Text( line, ItemName );

    // and insert it into semantic subtree
    RuleItem = Tree.InsertItem( ItemName, ItemSem, TVI_LAST );
    Tree.SetItemImage( RuleItem, IDB_TREE_RULE-IDB_TREE_FIRST, IDB_TREE_RULE_SEL-IDB_TREE_FIRST );

    // Then add all children for this item
    for ( var = 0; var < pDoc->KB->VarNumber(); var++ ) {

      if ( var == GoalVar )
        continue;

      // Get proposition text description
      pDoc->KB->Rules->Text( line, var, ItemName );

      if ( lstrlen( ItemName ) == 0 )
        continue;

      // and insert it into the current rule subtree
      PropItem = Tree.InsertItem( ItemName, RuleItem, TVI_LAST );
      Tree.SetItemImage( PropItem, IDB_TREE_IF-IDB_TREE_FIRST, IDB_TREE_IF_SEL-IDB_TREE_FIRST );

      // Insert concrete components
      for ( val = 0; val < pDoc->KB->ValNumber(var); val++ ) {

        // Show only 0 components (inverted)
        if ( pDoc->KB->Rules->GetComp( line, var, val ) != 0 )
          continue;

        // Get value text description
        pDoc->KB->Space->ValGetName( var, val, ItemName );

        // and insert it into the current proposition subtree
        CompItem = Tree.InsertItem( ItemName, PropItem, TVI_LAST );
        Tree.SetItemImage( CompItem, IDB_TREE_COMP15-IDB_TREE_FIRST, IDB_TREE_COMP15-IDB_TREE_FIRST );
        }

      }

    // Now add one conclusion
    pDoc->KB->Rules->Text( line, GoalVar, ItemName );

    PropItem = Tree.InsertItem( ItemName, RuleItem, TVI_LAST );
    Tree.SetItemImage( PropItem, IDB_TREE_THEN-IDB_TREE_FIRST, IDB_TREE_THEN_SEL-IDB_TREE_FIRST );

    if ( GoalVar != RiCoord_Null ) {
      // Insert concrete components for the goal proposition
      for ( val = 0; val < pDoc->KB->ValNumber(GoalVar); val++ ) {

        // Show only non-0 components
        if ( pDoc->KB->Rules->GetComp( line, GoalVar, val ) == 0 )
          continue;

        // Get value text description
        pDoc->KB->Space->ValGetName( GoalVar, val, ItemName );

        // and insert it into the current proposition subtree
        CompItem = Tree.InsertItem( ItemName, PropItem, TVI_LAST );
        Tree.SetItemImage( CompItem, IDB_TREE_COMP15-IDB_TREE_FIRST, IDB_TREE_COMP15-IDB_TREE_FIRST );
        }
      }

    }

	
}

int CRiTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

  //
  // Create an ImageList and attach to the tree
  //
  CBitmap BitMap;
  UINT nID;

  CImageList *ImageList = new CImageList();
  ImageList->Create( 16, 16, TRUE, IDB_TREE_LAST-IDB_TREE_FIRST+1, 1);

  // Load all bitmaps into the imagelist (in loop since there may be a lot of them)
  for ( nID = IDB_TREE_FIRST; nID <= IDB_TREE_LAST; nID++ ) {

    BitMap.LoadBitmap(nID);
    ImageList->Add( & BitMap, (COLORREF)0xFFFFFF );
    BitMap.DeleteObject();

    }

  CTreeCtrl& Tree = GetTreeCtrl();

  // attach our imagelist to the control
  if ( ImageList != NULL )
    Tree.SetImageList ( ImageList, TVSIL_NORMAL );

  //
  // Set other parameters of the tree (e.g., indent)
  //

	return 0;
}

void CRiTreeView::OnDestroy() 
{
	// TODO: Add your message handler code here
	
  //
  // Remove image list from the list and delete it
  //

  CTreeCtrl& Tree = GetTreeCtrl();
  CImageList *ImageList = Tree.SetImageList ( NULL, TVSIL_NORMAL );

  if ( ImageList != NULL ) {
    ImageList->DeleteImageList();
    delete ImageList;
    }

	CTreeView::OnDestroy();
	
}
