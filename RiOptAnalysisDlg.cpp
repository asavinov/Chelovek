// RiOptAnalysisDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Lk\Lk.h"
#include "Kb\Kb.h"

#include "Chelovek.h"
#include "RiDoc.h"
#include "RiOptAnalysisDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRiOptAnalysisDlg dialog


CRiOptAnalysisDlg::CRiOptAnalysisDlg( CRiDoc* pDoc, CWnd* pParent /*=NULL*/)
	: CDialog(CRiOptAnalysisDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRiOptAnalysisDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

  Document = pDoc;
  Options = & Document->KB->OptAnalysis; // Save the pointer to the options

}


void CRiOptAnalysisDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRiOptAnalysisDlg)
	DDX_Control(pDX, IDC_MAXVALSPIN, m_MaxValSpin);
	DDX_Control(pDX, IDC_MAXVALEDIT, m_MaxValEdit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRiOptAnalysisDlg, CDialog)
	//{{AFX_MSG_MAP(CRiOptAnalysisDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRiOptAnalysisDlg message handlers

BOOL CRiOptAnalysisDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
  //
  // Set initial parameters of controls
  //

  // Range of possible maxiaml values
  m_MaxValSpin.SetRange( 2, 1000 );

  // Initial position
  m_MaxValSpin.SetPos( Options->MaxValues );

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRiOptAnalysisDlg::OnOK() 
{
	// TODO: Add extra validation here
	
  //
  // (Validate and) save new options
  //
  int NewPos = m_MaxValSpin.GetPos();

  if ( HIWORD(NewPos) ) {
    // ERROR: Wrong value or range
    return;
    }

  Options->MaxValues = LOWORD(NewPos);

	CDialog::OnOK();
}
