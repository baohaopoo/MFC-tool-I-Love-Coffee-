#pragma once

#include "Include.h"

class CDevice
{
	DECLARE_SINGLETON(CDevice)

private:
	CDevice();
	~CDevice();

public:
	LPDIRECT3DDEVICE9		Get_Device() { return m_pDevice; }

public:
	HRESULT		InitDevice(void);

	void		Render_Begin(void);
	void		Render_End(HWND hWnd = nullptr);

	void		Release(void);

private:
	void		SetParameters(D3DPRESENT_PARAMETERS& d3dpp);

private:
	LPDIRECT3D9				m_p3D;			// com ��ü : �� �༮�� ��ġ�� sdk ������ �˻��ϰ� �� ���� �ȿ� �ִ� ��ü ������ ����ϴ� ��ü�� �ش�
	LPDIRECT3DDEVICE9		m_pDevice;		// �׸���� ���õ� ���� ����� ž���� ��ü


};

