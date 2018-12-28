// aboutdlg.h : interface of the CSmartKBDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _H_SMARTKBDLG_H_
#define _H_SMARTKBDLG_H_

#include <UI/UICtrl.h>

#include "resource.h"
#include "SmartKB.h"

class CSmartKBDlg 
	: public UIDialogImpl<CSmartKBDlg> 
	, public UIEnableMove<CSmartKBDlg>
{
	typedef CSmartKBDlg This;
	typedef UIDialogImpl<CSmartKBDlg> Base;
private:
	SMARTKB_CALLBACK callback_;
	std::vector<SMARTKBITEM> items_;
public:
	enum { IDD = IDD_SMARTKB };

	CSmartKBDlg();
	~CSmartKBDlg();

	void SetCallback(SMARTKB_CALLBACK callback) { callback_ = callback; }
	void AddItem(PSMARTKBITEM pItem, int nCount) { items_.insert(items_.end(), pItem, pItem + nCount); }
	void Show(HWND hwnd, int nShow);
	void Input(TCHAR Key);
	void Control(WPARAM VKey);

	virtual void OnOK();
	virtual void OnCancel();

	BEGIN_MSG_MAP(This)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		CHAIN_MSG_MAP(UIEnableMove<CSmartKBDlg>)
		CHAIN_MSG_MAP(Base)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif//_H_SMARTKBDLG_H_
