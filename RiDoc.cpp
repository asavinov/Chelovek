// RiDoc.cpp : implementation of the CRiDoc class
//

#include "stdafx.h"
#include "Lk\Lk.h"
#include "Kb\Kb.h"

#include "Chelovek.h"
#include "RiDoc.h"
#include "RiDataSourceDlg.h"
#include "RiProcessAnalysisDlg.h"
#include "RiProcessInductionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRiDoc

IMPLEMENT_DYNCREATE(CRiDoc, CDocument)

BEGIN_MESSAGE_MAP(CRiDoc, CDocument)
	//{{AFX_MSG_MAP(CRiDoc)
	ON_COMMAND(ID_INSERT_DATASOURCE, OnInsertDatasource)
	ON_COMMAND(ID_PROCESS_ANALYSIS, OnProcessAnalysis)
	ON_COMMAND(ID_PROCESS_INDUCTION, OnProcessInduction)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRiDoc construction/destruction

CRiDoc::CRiDoc()
{
	// TODO: add one-time construction code here

	KB = NULL; // Create new knowledge base object only when NewDocument is created
}

CRiDoc::~CRiDoc()
{

	// Delete knowledge base object before destroying the document
	if ( KB != NULL )
	{
		delete KB;
	}

	KB == NULL;
}

BOOL CRiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	KbKB* NewKB = new KbKB( "Knowledge Base" );

	if ( NewKB == NULL ) 
		return FALSE; // ERROR:

	// Delete knowledge base object and save new pointer
	if ( KB != NULL )
	{
		delete KB;
	}

	KB = NewKB;  

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CRiDoc serialization

void CRiDoc::Serialize(CArchive& ar)
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
// CRiDoc diagnostics

#ifdef _DEBUG
void CRiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CRiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRiDoc commands

void CRiDoc::OnInsertDatasource() 
{
	// TODO: Add your command handler code here
	
	//
	// Start dialog to edit the data source parameters (data source view)
	//
	CRiDataSourceDlg DataSourceDlg( this );
	int ret = DataSourceDlg.DoModal();
}

void CRiDoc::OnProcessAnalysis() 
{
	// TODO: Add your command handler code here

	CRiProcessAnalysisDlg ProcessAnalysisDlg( this );
	int ret = ProcessAnalysisDlg.DoModal();
}

void CRiDoc::OnProcessInduction() 
{
	// TODO: Add your command handler code here
	
	CRiProcessInductionDlg ProcessInductionDlg( this );
	int ret = ProcessInductionDlg.DoModal();
}
