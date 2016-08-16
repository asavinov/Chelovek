#if !defined(AFX_RIOPTANALYSISDLG_H__72C5D1A1_013F_11D2_AE71_CB1A29191934__INCLUDED_)
#define AFX_RIOPTANALYSISDLG_H__72C5D1A1_013F_11D2_AE71_CB1A29191934__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RiOptAnalysisDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRiOptAnalysisDlg dialog

class CRiOptAnalysisDlg : public CDialog
{
// Construction
public:
  CRiDoc* Document;
  RiOptAnalysis* Options; // Options being changed

	CRiOptAnalysisDlg( CRiDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRiOptAnalysisDlg)
	enum { IDD = IDD_OPT_ANALYSIS };
	CSpinButtonCtrl	m_MaxValSpin;
	CEdit	m_MaxValEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRiOptAnalysisDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRiOptAnalysisDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIOPTANALYSISDLG_H__72C5D1A1_013F_11D2_AE71_CB1A29191934__INCLUDED_)
