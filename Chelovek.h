// Chelovek.h : main header file for the CHELOVEK application
//

#if !defined(AFX_CHELOVEK_H__9F73E286_EE63_11D1_AE71_CB1A29191934__INCLUDED_)
#define AFX_CHELOVEK_H__9F73E286_EE63_11D1_AE71_CB1A29191934__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CChelovekApp:
// See Chelovek.cpp for the implementation of this class
//

class CChelovekApp : public CWinApp
{
public:
	CChelovekApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChelovekApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CChelovekApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHELOVEK_H__9F73E286_EE63_11D1_AE71_CB1A29191934__INCLUDED_)
