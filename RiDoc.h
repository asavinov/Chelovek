// RiDoc.h : interface of the CRiDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIDOC_H__9F73E28C_EE63_11D1_AE71_CB1A29191934__INCLUDED_)
#define AFX_RIDOC_H__9F73E28C_EE63_11D1_AE71_CB1A29191934__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CRiDoc : public CDocument
{
protected: // create from serialization only
	CRiDoc();
	DECLARE_DYNCREATE(CRiDoc)

// Attributes
public:

  KbKB* KB;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRiDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRiDoc)
	afx_msg void OnInsertDatasource();
	afx_msg void OnProcessAnalysis();
	afx_msg void OnProcessInduction();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RIDOC_H__9F73E28C_EE63_11D1_AE71_CB1A29191934__INCLUDED_)
