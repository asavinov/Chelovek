#if !defined(AFX_RIDATASOURCEDLG_H__7E34D2B1_F93C_11D1_AE71_CB1A29191934__INCLUDED_)
#define AFX_RIDATASOURCEDLG_H__7E34D2B1_F93C_11D1_AE71_CB1A29191934__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// RiDataSourceDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRiDataSourceDlg dialog

class CRiDataSourceDlg : public CDialog
{
// Construction
public:
  KbDataSourceCSV *DataSource;
  CRiDoc *Document;

	CRiDataSourceDlg( CRiDoc* pDoc, CWnd* pParent = NULL );   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRiDataSourceDlg)
	enum { IDD = IDD_DATASOURCE };
	CListBox	m_Fields;
	CListBox	m_Tables;
	CButton	m_DBSelect;
	CEdit	m_Database;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRiDataSourceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRiDataSourceDlg)
	afx_msg void OnDsDbselect();
	afx_msg void OnSelchangeDsTables();
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIDATASOURCEDLG_H__7E34D2B1_F93C_11D1_AE71_CB1A29191934__INCLUDED_)
