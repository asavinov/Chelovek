// RiProcessAnalysisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lk\Lk.h"
#include "Kb\Kb.h"

#include "Chelovek.h"
#include "RiDoc.h"
#include "RiOptAnalysisDlg.h"
#include "RiProcessAnalysisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRiProcessAnalysisDlg dialog


CRiProcessAnalysisDlg::CRiProcessAnalysisDlg( CRiDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CRiProcessAnalysisDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRiProcessAnalysisDlg)
	//}}AFX_DATA_INIT

  Document = pDoc; // Save the document with this data source
  Space = NULL; // No space object

  Options = & Document->KB->OptAnalysis; // Save pointer to the options from the knowledge base (document)

  ProcessState = 0; // No process is running

}


void CRiProcessAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRiProcessAnalysisDlg)
	DDX_Control(pDX, IDC_RECNOEDIT, m_RecNoStat);
	DDX_Control(pDX, IDC_VALNUMEDIT, m_ValNumStat);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDC_STARTBTN, m_StartBtn);
	DDX_Control(pDX, IDC_OPTIONSBTN, m_OptionsBtn);
	DDX_Control(pDX, IDC_ENDBTN, m_EndBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRiProcessAnalysisDlg, CDialog)
	//{{AFX_MSG_MAP(CRiProcessAnalysisDlg)
	ON_BN_CLICKED(IDC_OPTIONSBTN, OnOptionsBtn)
	ON_BN_CLICKED(IDC_STARTBTN, OnStartBtn)
	ON_BN_CLICKED(IDC_ENDBTN, OnEndBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRiProcessAnalysisDlg message handlers

BOOL CRiProcessAnalysisDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

  m_RecNoStat.SetWindowText( "0" );
  m_ValNumStat.SetWindowText( "0" );

  // Set state of buttons (disable End button)
  SendMessage( WM_COMMAND, MAKEWPARAM(IDC_ENDBTN,BN_CLICKED), (LPARAM) m_EndBtn.m_hWnd );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRiProcessAnalysisDlg::OnOptionsBtn() 
{
	// TODO: Add your control notification handler code here
	
	CRiOptAnalysisDlg OptAnalysisDlg( Document );
	int ret = OptAnalysisDlg.DoModal();

}


void CRiProcessAnalysisDlg::OnStartBtn() 
{
	// TODO: Add your control notification handler code here

  //
  // Check if we are able to start the analysis process
  //

  if ( Document->KB->DataSource == NULL ) {
    // ERROR: No data source has been defined
    return;
    }

  if ( ProcessState ) {
    // ERROR: Already in analysis process
    return;
    }

  //
  // Preparations for new process
  //
  if ( Space != NULL ) {
    delete Space;
    Space = NULL;
    }

  Space = new RiSpace( "Syntax" );
  if ( Space == NULL ) {
    // ERROR: Cannot create space
    return;
    }

  //
  // Set dialog state
  //
  ProcessState = 2; // Process is running normally
  m_OkBtn.EnableWindow( FALSE );
  m_CancelBtn.EnableWindow( FALSE );
  m_StartBtn.EnableWindow( FALSE );
  m_EndBtn.EnableWindow( TRUE );
  m_OptionsBtn.EnableWindow( FALSE );

  //
  // Start new thread of the analysis process
  //


  Document->KB->DataSource->Disconnect(); // DAO is not thread safe -- all functions should be within one thread

  AfxBeginThread( ProcessAnalysisProc, this );

}

void CRiProcessAnalysisDlg::OnEndBtn() 
{
	// TODO: Add your control notification handler code here
	
  if ( ProcessState != 0 ) {

    //
    // Ask for confirmation to stop the process
    //
    ProcessState = 1; // Suspend the process
    if ( FALSE ) {
      ProcessState = 2; // Resume the process calculations
      return;
      }

    //
    // Stop the process
    //
    ProcessState = 0; 

    // There will be one more call to this function when the process finds this sign and stops itself
    return;
    }


  // The process is finished normally and asks us 
  // to set the state of the dialog to normal (no process is running)

  m_OkBtn.EnableWindow( TRUE );
  m_CancelBtn.EnableWindow( TRUE );
  m_StartBtn.EnableWindow( TRUE );
  m_EndBtn.EnableWindow( FALSE ); // Disable this button (itself)
  m_OptionsBtn.EnableWindow( TRUE );

}

void CRiProcessAnalysisDlg::OnOK() 
{
	// TODO: Add extra validation here

  if ( Space == NULL ) {
    return; // ERROR: Space has not been created (no analysis has been carried out)
    }

  //
  // Now transfer this space as an object to the document (and delete old one)
  //

  if ( Document->KB->Rules != NULL ) {
    delete Document->KB->Rules;
    Document->KB->Rules = NULL;
    }

  if ( Document->KB->Space != NULL ) {

    delete Document->KB->Space;
    Document->KB->Space = NULL;
    }

  Document->KB->Space = Space;

  Space = NULL; // Forget about it
	
  //
  // Change the document state
  //
  Document->SetModifiedFlag( TRUE );
  Document->UpdateAllViews( NULL, 0L, NULL );

	CDialog::OnOK();
}

void CRiProcessAnalysisDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
  //
  // If space has been created, delete it
  //

  if ( Space != NULL ) {
    delete Space;
    Space = NULL;
    }

	CDialog::OnCancel();
}

UINT ProcessAnalysisProc( LPVOID pParam ) {
/*
  CRiProcessAnalysisDlg* Dlg = (CRiProcessAnalysisDlg*) pParam;

  //
  // Check possiblity to start the process
  //

  if ( Dlg == NULL ) {
    // ERROR: Something wrong -- bad parameter
    Dlg->ProcessState = 0;
    Dlg->SendMessage( WM_COMMAND, MAKEWPARAM(IDC_ENDBTN,BN_CLICKED), (LPARAM) Dlg->m_EndBtn.m_hWnd );
    return 1;
    }

  //
  // Prepare data source
  //
  Dlg->Document->KB->DataSource->Connect( NULL ); // Connect to default database
  Dlg->Document->KB->DataSource->Open( NULL, NULL ); // Open default recordset

  Dlg->Document->KB->DataSource->Recordset->MoveFirst();

  //
  // Insert variables into the space
  //
  RiCoord var, FieldNum = Dlg->Document->KB->DataSource->Recordset->GetFieldCount();
  CDaoFieldInfo fi;

  for ( var = 0; var < FieldNum; var++ ) {

    // Get field name
    Dlg->Document->KB->DataSource->Recordset->GetFieldInfo( var, fi, AFX_DAO_PRIMARY_INFO );

    // Add it to the space
    Dlg->Space->VarAdd( (char*) (LPCSTR) fi.m_strName );
    }

  //
  // Start the process: retrieve all records in loop
  //
  long RecNo = 0;
  RiCoord ValNum = 0;
  CString Str;
  COleVariant Value;
  RiVariable* VarNode;
  RiValue* ValNode;

  while ( ! Dlg->Document->KB->DataSource->Recordset->IsEOF() ) {

    Str.Format( "%lu", RecNo );
    Dlg->m_RecNoStat.SetWindowText( Str );

    Str.Format( "%lu", ValNum );
    Dlg->m_ValNumStat.SetWindowText( Str );

    while ( Dlg->ProcessState == 1 ) {
      ; // Somebody wants to suspend the process calculations (temporarily)
      }

    if ( Dlg->ProcessState == 0 ) {

      Dlg->SendMessage( WM_COMMAND, MAKEWPARAM(IDC_ENDBTN,BN_CLICKED), (LPARAM) Dlg->m_EndBtn.m_hWnd );
      Dlg->Document->KB->DataSource->Disconnect(); // Disconnect within this thread
      return 0; // Somebody wants to finish the process (usually the End button)
      }

    //
    // For each field get its value and compare on existence in the space
    //
    for ( var = 0; var < FieldNum; var++ ) {

      // Get field value
      Dlg->Document->KB->DataSource->Recordset->GetFieldValue( var, Value );

      if ( ((LPVARIANT) Value)->vt == VT_NULL ) {
        // ERROR: Bad value (cannot transform to the string)
        continue;
        }

      Value.ChangeType( VT_BSTR );

      Str = ((LPVARIANT) Value)->pbVal; // bstrVal

      if ( Str.IsEmpty() )
        continue; // Do not process NULL values

      VarNode = (RiVariable*) Dlg->Space->Coord2Object( var );

      // Continue if there exists already such a value
      ValNode = (RiValue*) VarNode->Name2Object( (char*) (LPCSTR) Str );
      if (  ValNode != NULL ) {
        // we have such a value already -- increment its frequency and go to the next field
        ValNode->Freq++;
        continue; 
        }

      if ( Dlg->Space->ValNumber[var+1] >= Dlg->Options->MaxValues - 1 ) {
        // No room for new record
        Str = "Default value";
        ValNode = (RiValue*) VarNode->Name2Object( (char*) (LPCSTR) Str );
        if ( ValNode != NULL ) {
          // we have a default record already -- increment its frequency and go to the next field
          ValNode->Freq++;
          continue; 
          }
        }

      Dlg->Space->ValAdd( var, (char*) (LPCSTR) Str );
      }

    // Go to the next record
    Dlg->Document->KB->DataSource->Recordset->MoveNext();
    RecNo++;
    ValNum = Dlg->Space->ValSum;
    }

  Str.Format( "%lu", RecNo );
  Dlg->m_RecNoStat.SetWindowText( Str );

  Str.Format( "%lu", ValNum );
  Dlg->m_ValNumStat.SetWindowText( Str );

  ::MessageBeep(0xFFFFFFFF);

  Dlg->ProcessState = 0;

  Dlg->SendMessage( WM_COMMAND, MAKEWPARAM(IDC_ENDBTN,BN_CLICKED), (LPARAM) Dlg->m_EndBtn.m_hWnd );
  Dlg->Document->KB->DataSource->Disconnect(); // Disconnect within this thread
*/
  return 0; // thread completed successfully
  }
