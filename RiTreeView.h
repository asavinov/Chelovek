// CRiTreeView.h : interface of the CRiTreeView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RITREEVIEW_H__9F73E28E_EE63_11D1_AE71_CB1A29191934__INCLUDED_)
#define AFX_RITREEVIEW_H__9F73E28E_EE63_11D1_AE71_CB1A29191934__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CRiTreeView : public CTreeView
{
protected: // create from serialization only
	CRiTreeView();
	DECLARE_DYNCREATE(CRiTreeView)

// Attributes
public:
	CRiDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRiTreeView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRiTreeView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRiTreeView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ChelovekView.cpp
inline CRiDoc* CRiTreeView::GetDocument()
   { return (CRiDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RITREEVIEW_H__9F73E28E_EE63_11D1_AE71_CB1A29191934__INCLUDED_)
