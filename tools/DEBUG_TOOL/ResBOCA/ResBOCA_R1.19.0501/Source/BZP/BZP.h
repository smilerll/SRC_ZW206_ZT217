// BZP.h : main header file for the BZP DLL
//

#if !defined(AFX_BZP_H__CBB6B537_6ED5_4F77_87B7_12A2FE54A8FD__INCLUDED_)
#define AFX_BZP_H__CBB6B537_6ED5_4F77_87B7_12A2FE54A8FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBZPApp
// See BZP.cpp for the implementation of this class
//

class CBZPApp : public CWinApp
{
public:
	CBZPApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBZPApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CBZPApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BZP_H__CBB6B537_6ED5_4F77_87B7_12A2FE54A8FD__INCLUDED_)
