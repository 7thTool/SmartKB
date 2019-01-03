#ifndef _H_MATCHCH_H_
#define _H_MATCHCH_H_

#include "resource.h"

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
	void Initialize();
	void Finishlize();
	bool MatchPinYin(TCHAR name, TCHAR key);
	bool MatchPinYin(TCHAR* name, TCHAR* key);
	TCHAR* Match(const TCHAR* name, const TCHAR* key);

protected:
	tagHead m_Head;
	char*  m_pData;
};

#endif//_H_MATCHCH_H_
