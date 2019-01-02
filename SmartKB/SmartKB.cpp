// SmartKB.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"
#include "SmartKB.h"
#include "SmartKBDlg.h"

//////////////////////////////////////////////////////////////////////////

CSmartKBDlg g_inst;

int SMartKB_Init(SMARTKB_CALLBACK callback)
{
	g_inst.SetCallback(callback);
	return 0;
}

void SMartKB_AddItem(PSMARTKBITEM pItem, int nCount)
{
	g_inst.AddItem(pItem, nCount);
}

HWND SMartKB_Create(HWND hwnd, int nShow)
{
	g_inst.Show(hwnd, nShow);
	return g_inst.GetSafeHwnd();
}

void SMartKB_Destory()
{
	if (g_inst.IsWindow())
	{
		g_inst.DestroyWindow();
	}
}

void SMartKB_Input(LPCTSTR lpszKey)
{
	if (g_inst.IsWindow())
	{
		g_inst.Input(lpszKey);
	}
}

BOOL SMartKB_Control(WPARAM VKey)
{
	if (g_inst.IsWindow())
	{
		return g_inst.Control(VKey);
	}
}

void SMartKB_Term()
{
	SMartKB_Destory();
}
