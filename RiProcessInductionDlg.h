#if !defined(AFX_RIPROCESSINDUCTIONDLG_H__8F8CC301_07A9_11D2_AE71_CB1A29191934__INCLUDED_)
#define AFX_RIPROCESSINDUCTIONDLG_H__8F8CC301_07A9_11D2_AE71_CB1A29191934__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RiProcessInductionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRiProcessInductionDlg dialog

class CRiProcessInductionDlg : public CDialog
{
// Construction
public:
  CRiDoc* Document;
  RiSpace* Space;
  RiOptInduction* Options;
  RiCNF_B* CNF;
  short ProcessState;

	CRiProcessInductionDlg( CRiDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRiProcessInductionDlg)
	enum { IDD = IDD_PROCESS_INDUCTION };
	CStatic	m_RuleNumStat;
	CStatic	m_RecNoStat;
	CButton	m_OkBtn;
	CButton	m_CancelBtn;
	CButton	m_StartBtn;
	CButton	m_OptionsBtn;
	CButton	m_EndBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRiProcessInductionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRiProcessInductionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnOptionsBtn();
	afx_msg void OnStartBtn();
	afx_msg void OnEndBtn();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

UINT ProcessInductionProc( LPVOID pParam );

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIPROCESSINDUCTIONDLG_H__8F8CC301_07A9_11D2_AE71_CB1A29191934__INCLUDED_)
