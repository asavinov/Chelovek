#if !defined(AFX_RIPROCESSANALYSISDLG_H__72C5D1A2_013F_11D2_AE71_CB1A29191934__INCLUDED_)
#define AFX_RIPROCESSANALYSISDLG_H__72C5D1A2_013F_11D2_AE71_CB1A29191934__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RiProcessAnalysisDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRiProcessAnalysisDlg dialog

class CRiProcessAnalysisDlg : public CDialog
{
// Construction
public:
  CRiDoc* Document;
  RiSpace* Space;
  RiOptAnalysis* Options;
  short ProcessState;

	CRiProcessAnalysisDlg( CRiDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRiProcessAnalysisDlg)
	enum { IDD = IDD_PROCESS_ANALYSIS };
	CStatic	m_RecNoStat;
	CStatic	m_ValNumStat;
	CButton	m_OkBtn;
	CButton	m_CancelBtn;
	CButton	m_StartBtn;
	CButton	m_OptionsBtn;
	CButton	m_EndBtn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRiProcessAnalysisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRiProcessAnalysisDlg)
	afx_msg void OnOptionsBtn();
	virtual BOOL OnInitDialog();
	afx_msg void OnStartBtn();
	afx_msg void OnEndBtn();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

UINT ProcessAnalysisProc( LPVOID pParam );

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIPROCESSANALYSISDLG_H__72C5D1A2_013F_11D2_AE71_CB1A29191934__INCLUDED_)
