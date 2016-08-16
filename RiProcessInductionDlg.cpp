// RiProcessInductionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lk\Lk.h"
#include "Kb\Kb.h"

#include "Chelovek.h"
#include "RiDoc.h"
#include "RiOptInductionDlg.h"
#include "RiProcessInductionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRiProcessInductionDlg dialog


CRiProcessInductionDlg::CRiProcessInductionDlg( CRiDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CRiProcessInductionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRiProcessInductionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

  Document = pDoc; // Save the document with this data source
  Space =  Document->KB->Space; // Space from the knowledge base
  Options = & Document->KB->OptInduction; // Save pointer to the options from the knowledge base (document)

  CNF = NULL; // No CNF object

  ProcessState = 0; // No process is running

}


void CRiProcessInductionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRiProcessInductionDlg)
	DDX_Control(pDX, IDC_RULENUMEDIT, m_RuleNumStat);
	DDX_Control(pDX, IDC_RECNOEDIT, m_RecNoStat);
	DDX_Control(pDX, IDOK, m_OkBtn);
	DDX_Control(pDX, IDCANCEL, m_CancelBtn);
	DDX_Control(pDX, IDC_STARTBTN, m_StartBtn);
	DDX_Control(pDX, IDC_OPTIONSBTN, m_OptionsBtn);
	DDX_Control(pDX, IDC_ENDBTN, m_EndBtn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRiProcessInductionDlg, CDialog)
	//{{AFX_MSG_MAP(CRiProcessInductionDlg)
	ON_BN_CLICKED(IDC_OPTIONSBTN, OnOptionsBtn)
	ON_BN_CLICKED(IDC_STARTBTN, OnStartBtn)
	ON_BN_CLICKED(IDC_ENDBTN, OnEndBtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRiProcessInductionDlg message handlers

BOOL CRiProcessInductionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
  m_RecNoStat.SetWindowText( "0" );
  m_RuleNumStat.SetWindowText( "0" );

  // Set the state of buttons
  SendMessage( WM_COMMAND, MAKEWPARAM(IDC_ENDBTN,BN_CLICKED), (LPARAM) m_EndBtn.m_hWnd );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRiProcessInductionDlg::OnOptionsBtn() 
{
	// TODO: Add your control notification handler code here
	
	CRiOptInductionDlg OptInductionDlg( Document );
	int ret = OptInductionDlg.DoModal();

}

void CRiProcessInductionDlg::OnStartBtn() 
{
	// TODO: Add your control notification handler code here
	

  //
  // Check if we are able to start the induction process
  //

  if ( Document->KB->DataSource == NULL ) {
    // ERROR: No data source has been defined
    return;
    }

  if ( Space == NULL ) {
    // ERROR: No space has been defined or denerated
    return;
    }

  if ( ProcessState ) {
    // ERROR: Already in induction process
    return;
    }

  //
  // Preparations for new process
  //
  if ( CNF != NULL ) {
    delete CNF;
    CNF = NULL;
    }

  CNF = new RiCNF_B( Space, Options, "Semantics" );
  if ( CNF == NULL ) {
    // ERROR: Cannot create CNF
    return;
    }
  CNF->Init();

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

  AfxBeginThread( ProcessInductionProc, this );

}

void CRiProcessInductionDlg::OnEndBtn() 
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

void CRiProcessInductionDlg::OnOK() 
{
	// TODO: Add extra validation here
	
  if ( CNF == NULL ) {
    return; // ERROR: CNF has not been created (no induction has been carried out)
    }

  //
  // Now transfer this CNF as an object to the document (and delete old one)
  //

  if ( Document->KB->Rules != NULL ) {
    delete Document->KB->Rules;
    Document->KB->Rules = NULL;
    }

  if ( CNF->Lines == 0 ) {
    delete CNF;
    CNF = NULL;
    }

  // Trnasform CNF into rules (should be implemented as a member function)
  Document->KB->Rules = CNF;

  CNF = NULL; // Forget about it
  Space = NULL;
  Options = NULL;

  //
  // Change the document state
  //
  Document->SetModifiedFlag( TRUE );
  Document->UpdateAllViews( NULL, 0L, NULL );

	CDialog::OnOK();
}

void CRiProcessInductionDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
  //
  // If CNF has been created, delete it
  //

  if ( CNF != NULL ) {
    delete CNF;
    CNF = NULL;
    }

	CDialog::OnCancel();
}

UINT ProcessInductionProc( LPVOID pParam ) {
/*
  CRiProcessInductionDlg* Dlg = (CRiProcessInductionDlg*) pParam;

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

  // Check if the data source has the same number of fields as the number of variables in the space
  // todo!!!
  RiCoord FieldNum = Dlg->Document->KB->DataSource->Recordset->GetFieldCount();

  if ( FieldNum != Dlg->Space->VarNumber ) {
    // ERROR: Something wrong -- the number of fields is not equal to the number of variables in the space
    Dlg->ProcessState = 0;
    Dlg->SendMessage( WM_COMMAND, MAKEWPARAM(IDC_ENDBTN,BN_CLICKED), (LPARAM) Dlg->m_EndBtn.m_hWnd );
    Dlg->Document->KB->DataSource->Disconnect(); // Disconnect within this thread
    return 1;
    }

  //
  // Start the process: retrieve all records in loop
  //
  RiCoord Point[2048]; Point[0] = FieldNum;
  RiCoord var;
  long RecNo = 0;
  RiLine RuleNum = 1;
  CString Str;
  COleVariant Value;
  RiVariable* VarNode;

  Dlg->CNF->StartInduction();

  while ( ! Dlg->Document->KB->DataSource->Recordset->IsEOF() ) {

    Str.Format( "%lu", RecNo );
    Dlg->m_RecNoStat.SetWindowText( Str );

    Str.Format( "%lu", RuleNum );
    Dlg->m_RuleNumStat.SetWindowText( Str );

    while ( Dlg->ProcessState == 1 ) {
      ; // Somebody wants to suspend the process calculations (temporarily)
      }

    if ( Dlg->ProcessState == 0 ) {

      Dlg->SendMessage( WM_COMMAND, MAKEWPARAM(IDC_ENDBTN,BN_CLICKED), (LPARAM) Dlg->m_EndBtn.m_hWnd );
      Dlg->Document->KB->DataSource->Disconnect(); // Disconnect within this thread
      return 0; // Somebody wants to finish the process (usually the End button)
      }

    //
    // Form the vector of coordinates
    // For each field get its value and find the coordinate
    //
    for ( var = 0; var < FieldNum; var++ ) {

      // Get field value
      Dlg->Document->KB->DataSource->Recordset->GetFieldValue( var, Value );

      if ( ((LPVARIANT) Value)->vt == VT_NULL ) {
        // ERROR: Bad value (cannot transform to the string)
        Point[var+1] = RiCoord_Null; // Treat it as no value (NULL)
        break;
        }

      Value.ChangeType( VT_BSTR );

      Str = ((LPVARIANT) Value)->pbVal; // bstrVal

      if ( Str.IsEmpty() ) {
        Point[var+1] = RiCoord_Null; // Treat it as no value (NULL)
        break;
        }

      VarNode = (RiVariable*) Dlg->Space->Coord2Object( var );

      // Try to determine the coordinate for this name
      Point[var+1] = VarNode->Name2Coord( (char*) (LPCSTR) Str );

      }

    //
    // Add this vector to the CNF (elementary induction)
    // Can (and should) be made in parallel with the rest of the procedure, i.e., while the next coordinate vector is being built
    //
    if ( var == FieldNum ) { // Add only full vectors (without Null values)
      Dlg->CNF->AddPoint( Point );
      }

    // Go to the next record
    Dlg->Document->KB->DataSource->Recordset->MoveNext();
    RecNo++;
    RuleNum = Dlg->CNF->Lines;
    }

  Dlg->CNF->EndInduction();

  Str.Format( "%lu", RecNo );
  Dlg->m_RecNoStat.SetWindowText( Str );

  RuleNum = Dlg->CNF->Lines;
  Str.Format( "%lu", RuleNum );
  Dlg->m_RuleNumStat.SetWindowText( Str );

  ::MessageBeep(0xFFFFFFFF);

  Dlg->ProcessState = 0;

  Dlg->SendMessage( WM_COMMAND, MAKEWPARAM(IDC_ENDBTN,BN_CLICKED), (LPARAM) Dlg->m_EndBtn.m_hWnd );
  Dlg->Document->KB->DataSource->Disconnect(); // Disconnect within this thread
*/
  return 0; // thread completed successfully
  }

