#pragma once

#include "Include.h"

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual HRESULT		InsertTexture(const TCHAR* pFilePath, 
										const TCHAR* pStateKey = L"",	// ��Ƽ �ؽ�ó�� ���� ���
										const int& iCount = 0) PURE;

	virtual void		Release(void)PURE;
};

