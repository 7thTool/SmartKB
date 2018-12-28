#include "stdafx.h"
#include "SmartKBDlg.h"

CSmartKBDlg::CSmartKBDlg()
{
	
}

CSmartKBDlg::~CSmartKBDlg()
{
	
}

void CSmartKBDlg::Show(HWND hwnd, int nShow)
{
	Create(hwnd);
	ShowWindow(nShow);
}

void CSmartKBDlg::Input(TCHAR Key)
{

}

void CSmartKBDlg::Control(WPARAM VKey)
{

}

void CSmartKBDlg::OnOK()
{
	Base::OnOK();
}

void CSmartKBDlg::OnCancel()
{
	Base::OnCancel();
}

LRESULT CSmartKBDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	EnableMove();

	CenterWindow();

	return 0L;
}

LRESULT CSmartKBDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	return bHandled;
}

LRESULT CSmartKBDlg::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	return 0L;
}

LRESULT CSmartKBDlg::OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	WORD wNotifyCode = HIWORD(wParam); 
	WORD wID = LOWORD(wParam); 
	HWND hwndCtrl = (HWND)lParam;
	if (hwndCtrl) {
		switch(wNotifyCode)
		{
		case BN_CLICKED:
			/*if(m_bnSmartKB == hwndCtrl) {
				OnOK();
			}*/
			break;
		}
	} else {
		switch(wID)
		{
		case IDOK:
			OnOK();
			break;
		}
	}
	return bHandled;
}
