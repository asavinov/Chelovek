// CRiListView.h : interface of the CRiListView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_RILISTVIEW_H__9F73E28E_EE63_11D1_AE71_CB1A29191934__INCLUDED_)
#define AFX_RILISTVIEW_H__9F73E28E_EE63_11D1_AE71_CB1A29191934__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CRiListView : public CListView
{
protected: // create from serialization only
	CRiListView();
	DECLARE_DYNCREATE(CRiListView)

// Attributes
public:
	CRiDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRiListView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CRiListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CRiListView)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ChelovekView.cpp
inline CRiDoc* CRiListView::GetDocument()
   { return (CRiDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RILISTVIEW_H__9F73E28E_EE63_11D1_AE71_CB1A29191934__INCLUDED_)
