#include "stdafx.h"
#include "MatchCH.h"

#include <string>
using std::string;

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

void MatchCH::Initialize()
{
	HRSRC hRsrc = ::FindResource(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_PINYIN), _T("PINYIN"));
	if (hRsrc) {
		HGLOBAL hResLoad = LoadResource(_Module.GetResourceInstance(), hRsrc);
		if (hResLoad) {
			DWORD dwRsrcSize = SizeofResource(_Module.GetResourceInstance(), hRsrc);
			LPBYTE lpResLock = (LPBYTE)LockResource(hResLoad);//LockResource(hResLoad)相当于把你的模型文件加载到一块内存里，内存首地址为lpResLock
			if (lpResLock) {
				if (dwRsrcSize > sizeof(tagHead)) {
					memcpy(&m_Head, lpResLock, sizeof(tagHead));
					DWORD dwData = (m_Head.usEnd - m_Head.usStart) * m_Head.filednum;
					m_pData = new char[dwData];
					memcpy(m_pData, lpResLock + sizeof(tagHead), dwData);
				}
			}
			FreeResource(hResLoad);
		}
	}
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



