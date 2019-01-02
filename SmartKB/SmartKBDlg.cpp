#include "stdafx.h"
#include "SmartKBDlg.h"

MatchCH::MatchCH()
{
	memset(&m_Head, 0, sizeof(m_Head));
	m_pData = NULL;
	Initialize();
}

MatchCH::~MatchCH()
{
	Finishlize();
}

bool MatchCH::Initialize()
{
	TCHAR szAppPath[MAX_PATH] = { 0 };
	TCHAR szPinYinFile[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, szAppPath, MAX_PATH);
	PathRemoveFileSpec(szAppPath);
	_stprintf(szPinYinFile, _T("%s\\pinyin.py"), szAppPath);

	//¶ÁÈ¡Æ´ÒôÓ³Éä±í
	HANDLE hPinYin = CreateFile(szPinYinFile,
		GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if (hPinYin && hPinYin != INVALID_HANDLE_VALUE) {
		SetFilePointer(hPinYin, 0L, NULL, SEEK_SET);
		DWORD dwSize = GetFileSize(hPinYin, NULL);
		if (dwSize > sizeof(tagHead)) {
			DWORD dwRead = 0;
			if (ReadFile(hPinYin, &m_Head, sizeof(tagHead), &dwRead, NULL)) {
				dwSize -= sizeof(tagHead);
				DWORD dwData = (m_Head.usEnd - m_Head.usStart) * m_Head.filednum;
				if (dwData <= dwSize) {
					m_pData = new char[dwData];
					ReadFile(hPinYin, m_pData, dwData, &dwRead, NULL);
				}
			}
		}
		CloseHandle(hPinYin);
	}

	return true;
}

void MatchCH::Finishlize()
{
	memset(&m_Head, 0, sizeof(MatchCH::tagHead));
	if (m_pData) {
		delete[]m_pData;
		m_pData = NULL;
	}
}

bool MatchCH::MatchPinYin(TCHAR* name, TCHAR* key)
{
	bool ret = false;
	while (*name && *key) {
		if (!(ret = MatchPinYin(*name++, *key++))) {
			break;
		}
	}
	return ret;
}

bool MatchCH::MatchPinYin(TCHAR name, TCHAR key)
{
	bool ret = false;
	unsigned short usName = name;
	if (usName >= m_Head.usStart && usName < m_Head.usEnd) {
		for (int i = usName - m_Head.usStart, j = 0; j < m_Head.filednum; j++) {
			if (m_pData[i * m_Head.filednum + j] == 0) {
				break;
			}
			else if (m_pData[i * m_Head.filednum + j] == key) {
				ret = true;
				break;
			}
		}
	}
	return ret;
}

TCHAR* MatchCH::Match(const TCHAR* name, const TCHAR* key)
{
	TCHAR* str = (TCHAR*)name;
	do 
	{
		if (MatchPinYin((TCHAR*)str, (TCHAR*)key))
		{
			return str;
			break;
		}
		str++;
	} while (*str);
	return nullptr;
}

CSmartKBDlg::CSmartKBDlg():callback_(NULL), max_results_(50), stop_flag_(false), best_results_(0)
{
	
}

CSmartKBDlg::~CSmartKBDlg()
{

}

void CSmartKBDlg::SetInputText(const CString& strText)
{
	std::lock_guard<std::mutex> lock(input_mutex_);
	input_text_ = strText;
	input_changed_ = true;
}

void CSmartKBDlg::GetInputText(CString& strText)
{
	std::lock_guard<std::mutex> lock(input_mutex_);
	strText = input_text_;
	strText.TrimLeft();
	strText.TrimRight();
	strText.MakeUpper();
	input_changed_ = false;
	input_results_.clear();
	best_results_ = 0;
}

void CSmartKBDlg::NotifyResult()
{
	PostMessage(WM_SEARCH_RESULT);
}

void CSmartKBDlg::ShowResult()
{
	std::vector<INPUTRESULT> results;
	GetInputResults(results);
	/*int Count = listbox_results_.GetCount();
	for (int i = Count;i >= 0; i--)
	{
		listbox_results_.DeleteString(i);
	}*/
	listbox_results_.ResetContent();
	for (size_t i = 0; i < results.size() && i < 2*max_results_; i++)
	{
		CString strResult;
		strResult.Format(_T("%s %s %s"), items_[results[i].pos].szKey1, items_[results[i].pos].szKey2, items_[results[i].pos].szName);
		listbox_results_.AddString(strResult);
		listbox_results_.SetItemData(i, results[i].pos);
	}
	if (!results.empty())
	{
		listbox_results_.SetCurSel(0);
	}
	listbox_results_.UpdateWindow();
}

void CSmartKBDlg::AddInputResult(size_t pos, int result)
{
	std::lock_guard<std::mutex> lock(input_mutex_);
	if (result==0)
	{
		best_results_++;
	}
	input_results_.emplace_back(pos, result);
}

void CSmartKBDlg::MergeInputResult()
{
	std::lock_guard<std::mutex> lock(input_mutex_);
	// sort INPUTRESULT so we can find the duplicates 
	if (!input_changed_ && !stop_flag_)
	{
		std::sort(input_results_.begin(), input_results_.end(), INPUTRESULTResultLess());
	}
	//if (!input_changed_ && !stop_flag_)
	//{
	//	std::stable_sort(input_results_.begin(), input_results_.end(), INPUTRESULTPosLess());
	//}
	//if (!input_changed_ && !stop_flag_)
	//{
	//	/* eliminate duplicate INPUTRESULT:
	//	* unique reorders INPUTRESULT so that each INPUTRESULT appears once in the
	//	* front portion of words and returns an iterator one past the unique range;
	//	* erase uses a vector operation to remove the nonunique elements
	//	*/
	//	std::vector<INPUTRESULT>::iterator ite = std::unique(input_results_.begin(), input_results_.end(), INPUTRESULTPosEqual());
	//	input_results_.erase(ite, input_results_.end());
	//}
}

void CSmartKBDlg::GetInputResults(std::vector<INPUTRESULT>& results)
{
	std::lock_guard<std::mutex> lock(input_mutex_);
	results = input_results_;
}

void CSmartKBDlg::Search()
{
	while (!stop_flag_)
	{
		if (input_changed_)
		{
			CString strText;
			GetInputText(strText);
			if (!strText.IsEmpty())
			{
				if (!input_changed_ && !stop_flag_)
				{
					SearchKey(strText);
				}
				if (!input_changed_ && !stop_flag_)
				{
					MergeInputResult();
				}
			}
			if (stop_flag_)
			{
				break;
			}
			if (!input_changed_)
			{
				NotifyResult();
			}
		}
		else
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(200));
		}
	}
}

LPCTSTR CSmartKBDlg::StrKey(LPCTSTR name, LPCTSTR key)
{
	LPCTSTR str = name;
	LPCTSTR src = name;
	LPCTSTR dst = key;
	while (*src && *dst)
	{
		if (*src == *dst)
		{
			src++; dst++;
		}
		else if (matchch_ptr_->MatchPinYin(*src, *dst))
		{
			src++; dst++;
		}
		else
		{
			src++; str = src; dst = key;
		}
	}
	if (!*dst)
	{
		return str;
	}
	return nullptr;
}

void CSmartKBDlg::SearchKey(const CString& strText)
{
	for (size_t i=0; i < items_.size() && !input_changed_ && !stop_flag_; i++)
	{
		int result1 = -1, result2 = -1;
		LPCTSTR str = nullptr;
		str = StrKey(items_[i].szKey1, (LPCTSTR)strText);
		if (str) {
			result1 = str - items_[i].szKey1;
			ASSERT(result1 >= 0);
		}
		if (result1 != 0) {
			str = StrKey(items_[i].szKey2, (LPCTSTR)strText);
			if (str) {
				result2 = str - items_[i].szKey2;
				ASSERT(result2 >= 0);
			}
		}
		if (result1 >= 0 && result2 >= 0) {
			AddInputResult(i, std::min<>(result1, result2));
		}
		else if (result1 >= 0) {
			AddInputResult(i, result1);
		}
		else if (result2 >= 0) {
			AddInputResult(i, result2);
		}
		if (best_results_ >= max_results_) {
			break;
		}
	}
}

void CSmartKBDlg::Show(HWND hwnd, int nShow)
{
	Create(hwnd);
	ShowWindow(nShow);
}

void CSmartKBDlg::Input(LPCTSTR lpszKey)
{
	SetInputText(lpszKey);
}

BOOL CSmartKBDlg::Control(WPARAM VKey)
{
	int nCurSel = listbox_results_.GetCurSel();
	int nCount = listbox_results_.GetCount();
	switch (VKey)
	{
	case VK_UP:
	{
		if(nCurSel > 0)
			listbox_results_.SetCurSel(nCurSel - 1);
		return TRUE;
	}
	break;
	case VK_DOWN:
	{
		if (nCurSel < (nCount - 1))
			listbox_results_.SetCurSel(nCurSel + 1);
		return TRUE;
	}
	break;
	case VK_RETURN:
	{
		if (callback_ && nCurSel >= 0)
		{
			callback_(&items_[(int)listbox_results_.GetItemData(nCurSel)]);
			return TRUE;
		}
	}
	break;
	default:
		break;
	}
	return FALSE;
}

void CSmartKBDlg::OnOK()
{
	Base::OnOK();
}

void CSmartKBDlg::OnCancel()
{
	Base::OnCancel();
}

void CSmartKBDlg::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	if (lpDrawItemStruct->itemID < 0)
		return;
	if (lpDrawItemStruct->hDC == NULL)
		return;

	if (lpDrawItemStruct->hwndItem != listbox_results_.GetSafeHwnd())
	{
		return;
	}
	if (listbox_results_.GetCount() <= 0)
	{
		return;
	}

	CDCHandle dc(lpDrawItemStruct->hDC);

	//HGDIOBJ hOldFont = NULL;
	//if (m_hFont)
	//	hOldFont = ::SelectObject(dc.m_hDC, m_hFont);

	if (lpDrawItemStruct->itemState & ODS_SELECTED)
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(192, 192, 192));
	else
		dc.FillSolidRect(&lpDrawItemStruct->rcItem, RGB(255, 255, 255));

	int nOldBkMode = dc.SetBkMode(TRANSPARENT);
	COLORREF nTextColor = GetSysColor(COLOR_MENUTEXT);
	COLORREF nOldTextColor = dc.SetTextColor(nTextColor);

	//CString strText;
	//listbox_results_.GetText(lpDrawItemStruct->itemID, strText);
	int nItem = (int)listbox_results_.GetItemData(lpDrawItemStruct->itemID);

	CRect txtRect(lpDrawItemStruct->rcItem);
	//dc.SetTextColor(RGB(0, 0, 0));
	txtRect.left += 2;
	dc.DrawText(items_[nItem].szKey1, _tcslen(items_[nItem].szKey1), &txtRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	//dc.SetTextColor(nTextColor);
	dc.DrawText(items_[nItem].szKey2, _tcslen(items_[nItem].szKey2), &txtRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	//dc.SetTextColor(nTextColor);
	dc.DrawText(items_[nItem].szName, _tcslen(items_[nItem].szName), &txtRect, DT_RIGHT | DT_VCENTER | DT_SINGLELINE);

	dc.SetTextColor(nOldTextColor);
	dc.SetBkMode(nOldBkMode);

	//if (m_hFont)
	//	::SelectObject(dc.m_hDC, hOldFont);
}

void CSmartKBDlg::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	return OwnerDraw::MeasureItem(lpMeasureItemStruct);
}

LRESULT CSmartKBDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	DoDataExchange();

	EnableMove();

	CenterWindow();

	matchch_ptr_ = std::make_shared<MatchCH>();

	thread_ = std::make_shared<std::thread>(std::bind(&CSmartKBDlg::Search, this));

	return 0L;
}

LRESULT CSmartKBDlg::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;

	if (thread_)
	{
		stop_flag_ = true;
		thread_->join();
		thread_.reset();
	}

	matchch_ptr_.reset();

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

LRESULT CSmartKBDlg::OnSearchResult(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	ShowResult();
	return bHandled;
}


