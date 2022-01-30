#include "stdafx.h"
#include "Device.h"

IMPLEMENT_SINGLETON(CDevice)

CDevice::CDevice() 
	: m_p3D(nullptr), m_pDevice(nullptr)
{
}


CDevice::~CDevice()
{
	Release();
}

HRESULT CDevice::InitDevice(void)
{
	
	// 1. m_p3D��ü ����
	// Direct3DCreate9 : SDK������ �´� �İ�ü�� �����ϴ� �Լ�
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	// 2. ��ġ ����(���� ������ ����)
	// HAL�̶�� �ϵ���� �߻�ȭ ������ �����Ͽ� �����簡 �ٸ����� Ư�� �÷����� ���� �����ϰ� ������ �� �ִ��� ���縦 �Ѵ�.

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : ��ġ�� ���� ������ ������ �Լ�
	// 1���� :D3DADAPTER_DEFAULT�� ��� ������ �������� �⺻ �׷��� ī�带 �ǹ�
	
	if (FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"GetDeviceCaps Failed!");
		return E_FAIL;
	}
	// 3. ���ؽ� ���μ���(������ ��ȯ + ���� ����)�� �ϵ��� �����ϴ��� �����Ѵ�
	DWORD	vp = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	// �ϵ��� ����

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;	// ����ڰ� ���� ����


	// 4. �׸��� �İ�ü ����

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	
	// SetParameters �Լ� ���ο��� ����ü ��� �������� ���� �ʱ�ȭ ����
	SetParameters(d3dpp);

	if (FAILED(m_p3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, g_hWnd, vp, &d3dpp, &m_pDevice)))
	{
		AfxMessageBox(L"CreateDevice Failed");
		return E_FAIL;
	}	
	//return E_FAIL;

	return S_OK;

}

void CDevice::Render_Begin(void)
{
	m_pDevice->Clear(0,			// rect�� ����()
					nullptr,	// rect�� ������ �迭�� �̸�(nullptr�� ��� ��ü ȭ�� ������ �ǹ�)
					D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
					D3DCOLOR_ARGB(255, 125, 125, 125), // �ĸ� ���� ����
					1.f,	// ���� ������ �ʱ�ȭ ��
					0);		// ���ٽ� ������ �ʱ�ȭ ��

	m_pDevice->BeginScene(); // �ĸ� ���ۿ� �׸��� ������ �˷��ִ� �Լ�

}

void CDevice::Render_End(HWND hWnd)
{
	m_pDevice->EndScene(); // �ĸ� ���ۿ� �׸����� ���Ḧ �˷��ִ� �Լ�

	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
	// �ĸ� ���ۿ� ���� ���۸� ��ȯ�ϴ� �Լ�
	// 1, 2���� : ��Ʈ�� �ּ�, D3DSWAPEFFECT_COPY�� �ۼ����� �ʴ��� NULL
	// 3���� : ��� ��� �������� �ڵ�, ���� NULL�� ��� d3dpp.hDeviceWindow�� ������ �ڵ鰪�� �״�� ��
	// 4���� : D3DSWAPEFFECT_COPY�� �ۼ����� �ʴ��� NULL
}

void CDevice::Release(void)
{
	//m_pDevice->Release();
	//m_p3D->Release();	// �İ�ü�� �����ϴ� �Լ�
	Safe_Release(m_pDevice);
	Safe_Release(m_p3D);

}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	// �ĸ� ����
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1; // �ĸ���� ����

	// �ȼ��� �� ���̴� ��� ������ ������ �ε巴�� ó���ϴ� ����� ��Ƽ ���ø��̶� �Ѵ�.
	// �׷��� �������ϰ� ���ϱ� ������ ������� �ʴ´�.

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;


	// D3DSWAPEFFECT_DISCARD : ���� ü�� ���
	// D3DSWAPEFFECT_FLIP : ���� �ϳ��� �������鼭 ����ϴ� ���
	// D3DSWAPEFFECT_COPY : ���� ���۸��� ������ ���� ���

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// ����̽� ��ġ�� ����� ������ â�� �ڵ�
	d3dpp.hDeviceWindow = g_hWnd;
	
	// â���� ����� ���ΰ� ��üȭ������ ����� ���ΰ�?
	d3dpp.Windowed = TRUE;	// TRUE�϶� â��� ���� FALSE�� ��ü ȭ�� ����

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// ����� ������� ���� ȭ�� ���(�ÿ�)�� ���� ����

	// ��ü ȭ�� ��ȯ �� ������ �ɼ�
	// ��üȭ�� ����� ��쿡�� ����̽� ��ġ�� ����, â ����� ���� OS�� ������� ������

	// ��ü ȭ�� ���� ���� ����� ����� ��� �˾Ƽ� ����
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;


	// ����� ������� �ÿ��� ������ �߻����� ��� ��� �� ���ΰ�

	// D3DPRESENT_INTERVAL_DEFAULT : ������ ������ dx �˾Ƽ� ����, ���� ����� ������� ����
	// D3DPRESENT_INTERVAL_IMMEDIATE : �� ��� ������� ���� ���

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
