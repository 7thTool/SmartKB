// main.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "../SmartKB/SmartKB.h"
#include <vector>

SMARTKBITEM g_items[] = {
	{ _T("1"), _T("1"), _T("1"), 1 }
	, { _T("22"), _T("22"), _T("22"), 2 }
	, { _T("333"), _T("333"), _T("333"), 3 }
};

void my_callback(PSMARTKBITEM pItem)
{
	if (pItem)
	{
		_tprintf(_T("%s %s %s\n"), pItem->szKey1, pItem->szKey2, pItem->szName);
	}
}

int main()
{
	MSG msg;
	// force message queue to be created
	::PeekMessage(&msg, NULL, WM_USER, WM_USER, PM_NOREMOVE);

	SMartKB_Init(&my_callback);
	SMartKB_AddItem(g_items, _countof(g_items));
	SMartKB_Create(NULL, SW_SHOW);
	BOOL bRet;
	for (;;)
	{
		bRet = ::GetMessage(&msg, NULL, 0, 0);
		if (bRet == -1)
		{
			continue;   // error, don't process
		}
		else if (!bRet)
		{
			break;   // WM_QUIT, exit message loop
		}

		::TranslateMessage(&msg);
		::DispatchMessage(&msg);

		if (msg.message == WM_KEYDOWN)
		{
			if (msg.wParam==VK_ESCAPE)
			{
				PostQuitMessage(0);
			}
		}
		if (msg.message == WM_COMMAND)
		{
			PostQuitMessage(0);
		}
		if (msg.message == WM_CHAR)
		{
			TCHAR ch = (TCHAR)msg.wParam;
			SMartKB_Input(_T("1"));
		}
	}
	SMartKB_Term();
    return bRet;
}

