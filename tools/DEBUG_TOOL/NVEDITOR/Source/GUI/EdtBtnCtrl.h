#if !defined(AFX_EDTBTNCTRL_H__6ED0ED90_223C_49A4_98F3_E816DB8341AE__INCLUDED_)
#define AFX_EDTBTNCTRL_H__6ED0ED90_223C_49A4_98F3_E816DB8341AE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CEdtBtnCtrl.h : header file
//

#include "EditEx.h"

/////////////////////////////////////////////////////////////////////////////
// CEdtBtnCtrl dialog

class CEdtBtnCtrl : public CDialog
{
// Construction
public:
	CEdtBtnCtrl(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEdtBtnCtrl)
	enum { IDD = IDD_EDT_BTN_DLG };
	CEditEx	m_edtText;
	CButton	m_btnSel;
	CString	m_strText;
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEdtBtnCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	
public:
		int m_nItem;
		int m_nSubItem;
		
		void EnableFileSelButton(BOOL bEnable) { 	
			m_bBtnEnable = bEnable; 
		}
		void SetBtnOpenFlag(BOOL bOpen) { 
			m_bBtnOpenFlag = bOpen;
		}
		
		void SetEditFmtMode(UINT nMode) {
			m_edtText.SetFmtMode(nMode);
		}
		void SetFileFilterString(LPCTSTR lpszFilter);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEdtBtnCtrl)
	afx_msg void OnBtnSel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChangeEdtText();
	afx_msg void OnDestroy();
	afx_msg void OnKillfocusEdtText();
	virtual BOOL OnInitDialog();
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL m_bBtnEnable;
	BOOL m_bBtnOpenFlag;
	CString m_strFileFilter;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDTBTNCTRL_H__6ED0ED90_223C_49A4_98F3_E816DB8341AE__INCLUDED_)
