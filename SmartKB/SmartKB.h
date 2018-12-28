#ifndef _H_SMARTKB_H_
#define _H_SMARTKB_H_

#ifdef SMARTKB_EXPORTS
#define SMARTKB_API __declspec(dllexport)
#else
#define SMARTKB_API __declspec(dllimport)
#endif

extern "C"
{
#define MAX_KEY_LENGTH 64
#define MAX_NAME_LENGTH 64

	typedef struct  tagSmartKBItem
	{
		TCHAR szKey1[MAX_KEY_LENGTH + 1];
		TCHAR szKey2[MAX_KEY_LENGTH + 1];
		TCHAR szName[MAX_NAME_LENGTH + 1];
		LPARAM lParam;
	}SMARTKBITEM,*PSMARTKBITEM;

	typedef void(*SMARTKB_CALLBACK)(PSMARTKBITEM pItem);

SMARTKB_API int  SMartKB_Init(SMARTKB_CALLBACK callback);
SMARTKB_API void SMartKB_AddItem(PSMARTKBITEM pItem, int nCount);
SMARTKB_API HWND SMartKB_Show(HWND hwnd, int nShow);
SMARTKB_API void SMartKB_Destory();
SMARTKB_API void SMartKB_Input(TCHAR Key);
SMARTKB_API void SMartKB_Control(WPARAM VKey);
SMARTKB_API void SMartKB_Term();
}

#ifndef SMARTKB_EXPORTS
#pragma comment(lib, "SmartKB.lib")
#endif//

#endif//_H_SMARTKB_H_