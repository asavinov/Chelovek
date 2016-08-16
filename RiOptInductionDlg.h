#if !defined(AFX_RIOPTINDUCTIONDLG_H__0A307A81_0790_11D2_AE71_CB1A29191934__INCLUDED_)
#define AFX_RIOPTINDUCTIONDLG_H__0A307A81_0790_11D2_AE71_CB1A29191934__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RiOptInductionDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRiOptInductionDlg dialog

class CRiOptInductionDlg : public CDialog
{
// Construction
public:
  CRiDoc* Document;
  RiOptInduction* Options; // Options being changed

	CRiOptInductionDlg( CRiDoc* pDoc, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRiOptInductionDlg)
	enum { IDD = IDD_OPT_INDUCTION };
	CComboBox	m_GoalVarCombo;
	CButton	m_GoalVarCheck;
	CSpinButtonCtrl	m_MaxNewLinesSpin;
	CEdit	m_MaxNewLinesEdit;
	CSpinButtonCtrl	m_MaxLinesSpin;
	CEdit	m_MaxLinesEdit;
	CSpinButtonCtrl	m_MaxRankSpin;
	CEdit	m_MaxRankEdit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRiOptInductionDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRiOptInductionDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnGoalVarCheck();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIOPTINDUCTIONDLG_H__0A307A81_0790_11D2_AE71_CB1A29191934__INCLUDED_)
