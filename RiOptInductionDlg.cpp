// RiOptInductionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lk\Lk.h"
#include "Kb\Kb.h"

#include "Chelovek.h"
#include "RiDoc.h"
#include "RiOptInductionDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRiOptInductionDlg dialog


CRiOptInductionDlg::CRiOptInductionDlg( CRiDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CRiOptInductionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRiOptInductionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

  Document = pDoc;
  Options = & Document->KB->OptInduction; // Save the pointer to the options

}


void CRiOptInductionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRiOptInductionDlg)
	DDX_Control(pDX, IDC_GOALVARCOMBO, m_GoalVarCombo);
	DDX_Control(pDX, IDC_GOALVARCHECK, m_GoalVarCheck);
	DDX_Control(pDX, IDC_MAXNEWLINESSPIN, m_MaxNewLinesSpin);
	DDX_Control(pDX, IDC_MAXNEWLINES, m_MaxNewLinesEdit);
	DDX_Control(pDX, IDC_MAXLINESSPIN, m_MaxLinesSpin);
	DDX_Control(pDX, IDC_MAXLINES, m_MaxLinesEdit);
	DDX_Control(pDX, IDC_MAXRANKSPIN, m_MaxRankSpin);
	DDX_Control(pDX, IDC_MAXRANK, m_MaxRankEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRiOptInductionDlg, CDialog)
	//{{AFX_MSG_MAP(CRiOptInductionDlg)
	ON_BN_CLICKED(IDC_GOALVARCHECK, OnGoalVarCheck)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRiOptInductionDlg message handlers

BOOL CRiOptInductionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
  //
  // Set initial parameters of controls
  //

  RiSpace* Space = Document->KB->Space;
  RiCoord VarNum = Space->VarNumber;

  // Range of possible maxiaml values
  m_MaxRankSpin.SetRange( 1, VarNum );
  // Initial position
  m_MaxRankSpin.SetPos( Options->MaxRank );

  // Range of possible maxiaml values
  m_MaxLinesSpin.SetRange( 10, 10000 );
  // Initial position
  m_MaxLinesSpin.SetPos( Options->MaxLines );

  // Range of possible maxiaml values
  m_MaxNewLinesSpin.SetRange( 20, 20000 );
  // Initial position
  m_MaxNewLinesSpin.SetPos( Options->MaxNewLines );

  // Fill the combo box with variables
  m_GoalVarCombo.ResetContent();
  char VarName[10000];
  RiCoord var;
  for ( var = 0; var < VarNum; var++ ) {
    Space->VarGetName( var, VarName );
    m_GoalVarCombo.AddString( VarName );
    }

  if ( Options->GoalVar == RiCoord_Null ) {
    m_GoalVarCheck.SetCheck( 0 );
    }
  else {
    m_GoalVarCheck.SetCheck( 1 );

    m_GoalVarCombo.SetCurSel( Options->GoalVar );

    }

  OnGoalVarCheck(); // and set the state of the combo box


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRiOptInductionDlg::OnGoalVarCheck() 
{
	// TODO: Add your control notification handler code here
	
  //
  // Depending on the check box state, enable or disable the combo box
  //

  if ( m_GoalVarCheck.GetCheck() == 0 ) {
    m_GoalVarCombo.EnableWindow( FALSE );
    }
  else {
    m_GoalVarCombo.EnableWindow( TRUE );
    }

}

void CRiOptInductionDlg::OnOK() 
{
	// TODO: Add extra validation here
	
  //
  // (Validate and) save new options
  //
  RiSpace* Space = Document->KB->Space;
  RiCoord VarNum = Space->VarNumber;
  int NewPos;

  NewPos = m_MaxRankSpin.GetPos();
  if ( HIWORD(NewPos) ) {
    return; // ERROR: Wrong value or range
    }
  Options->MaxRank = LOWORD(NewPos);

  NewPos = m_MaxLinesSpin.GetPos();
  if ( HIWORD(NewPos) ) {
    return; // ERROR: Wrong value or range
    }
  Options->MaxLines = LOWORD(NewPos);

  NewPos = m_MaxNewLinesSpin.GetPos();
  if ( HIWORD(NewPos) ) {
    return; // ERROR: Wrong value or range
    }
  Options->MaxNewLines = LOWORD(NewPos);

  if ( m_GoalVarCheck.GetCheck() == 0 || m_GoalVarCombo.GetCurSel() == CB_ERR ) {
    Options->GoalVar = RiCoord_Null;
    }
  else {
    Options->GoalVar = m_GoalVarCombo.GetCurSel();
    }

	CDialog::OnOK();
}
