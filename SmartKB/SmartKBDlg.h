// aboutdlg.h : interface of the CSmartKBDlg class
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _H_SMARTKBDLG_H_
#define _H_SMARTKBDLG_H_

#include <UI/UICtrl.h>
#include <wtl/atlddx.h>
#include "resource.h"
#include "SmartKB.h"
#include <algorithm>
#include <thread>         // std::thread
#include <mutex>          // std::mutex

class MatchCH
{
protected:
	typedef struct
	{
		char filedlen;
		char filednum;
		unsigned short usStart;
		unsigned short usEnd;
	}tagHead;
	typedef struct
	{
		TCHAR	src;
		TCHAR	dst;
	}tagMohuYin;
public:
	MatchCH();
	~MatchCH();

public:
	bool Initialize();
	void Finishlize();
	bool MatchPinYin(TCHAR name, TCHAR key);
	bool MatchPinYin(TCHAR* name, TCHAR* key);
	TCHAR* Match(const TCHAR* name, const TCHAR* key);

protected:
	tagHead m_Head;
	char*  m_pData;
};

class CSmartKBDlg 
	: public UIDialogImpl<CSmartKBDlg> 
	, public UIEnableMove<CSmartKBDlg>
	, public CWinDataExchange<CSmartKBDlg>
	, public COwnerDraw<CSmartKBDlg>
{
	typedef CSmartKBDlg This;
	typedef UIDialogImpl<CSmartKBDlg> Base;
	typedef COwnerDraw<CSmartKBDlg> OwnerDraw;
private:
	std::shared_ptr<MatchCH> matchch_ptr_;
	SMARTKB_CALLBACK callback_;
	std::vector<SMARTKBITEM> items_;
	size_t max_results_;
	bool stop_flag_;
	std::shared_ptr<std::thread> thread_;
	bool input_changed_;
	CString input_text_;
	typedef struct tagInputResult
	{
		size_t pos;
		int result;
		tagInputResult() :pos(0), result(-1) {}
		tagInputResult(size_t p, int r):pos(p), result(r) {}
	}INPUTRESULT,*PINPUTRESULT;
	struct INPUTRESULTPosLess
	{
		bool operator()(const INPUTRESULT& x, const INPUTRESULT& y) const
		{
			return x.pos < y.pos;
		}
	};
	struct INPUTRESULTPosEqual
	{
		bool operator()(const INPUTRESULT& x, const INPUTRESULT& y) const
		{
			return x.pos == y.pos;
		}
	};
	struct INPUTRESULTResultLess
	{
		bool operator()(const INPUTRESULT& x, const INPUTRESULT& y) const
		{
			return x.result < y.result;
		}
	};
	std::vector<INPUTRESULT> input_results_;
	size_t best_results_;
	std::mutex input_mutex_;
	enum
	{
		WM_SEARCH_RESULT = WM_USER + 1
	};
public:
	enum { IDD = IDD_SMARTKB };

	CSmartKBDlg();
	~CSmartKBDlg();

	void SetInputText(const CString& strText);
	void GetInputText(CString& strText);
	void NotifyResult();
	void ShowResult();
	void AddInputResult(size_t pos, int result);
	void MergeInputResult();
	void GetInputResults(std::vector<INPUTRESULT>& results);
	void Search();
	LPCTSTR StrKey(LPCTSTR name, LPCTSTR key);
	void SearchKey(const CString& strText);

	void SetCallback(SMARTKB_CALLBACK callback) { callback_ = callback; }
	void AddItem(PSMARTKBITEM pItem, int nCount) { items_.insert(items_.end(), pItem, pItem + nCount); }
	void Show(HWND hwnd, int nShow);
	void Input(LPCTSTR lpszKey);
	BOOL Control(WPARAM VKey);

	UIListBox listbox_results_;
	BEGIN_DDX_MAP(This)
		DDX_CONTROL_HANDLE(IDC_LIST1, listbox_results_)
	END_DDX_MAP()

	virtual void OnOK();
	virtual void OnCancel();

	void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);

	BEGIN_MSG_MAP(This)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		//MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
		MESSAGE_HANDLER(WM_COMMAND, OnCommand)
		MESSAGE_HANDLER(WM_SEARCH_RESULT, OnSearchResult)
		CHAIN_MSG_MAP(UIEnableMove<CSmartKBDlg>)
		CHAIN_MSG_MAP(OwnerDraw)
		CHAIN_MSG_MAP(Base)
	END_MSG_MAP()

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnSearchResult(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif//_H_SMARTKBDLG_H_
