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
	
	// 1. m_p3D객체 생성
	// Direct3DCreate9 : SDK버전에 맞는 컴객체를 생성하는 함수
	m_p3D = Direct3DCreate9(D3D_SDK_VERSION);

	// 2. 장치 조사(지원 수준을 조사)
	// HAL이라는 하드웨어 추상화 계층을 접근하여 제조사가 다를지라도 특정 플랫폼에 대해 동일하게 동작할 수 있는지 조사를 한다.

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	// GetDeviceCaps : 장치에 대한 정보를 얻어오는 함수
	// 1인자 :D3DADAPTER_DEFAULT인 경우 정보를 얻으려는 기본 그래픽 카드를 의미
	
	if (FAILED(m_p3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		AfxMessageBox(L"GetDeviceCaps Failed!");
		return E_FAIL;
	}
	// 3. 버텍스 프로세싱(정점의 변환 + 조명 연산)을 하드웨어가 지원하는지 조사한다
	DWORD	vp = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp |= D3DCREATE_HARDWARE_VERTEXPROCESSING;	// 하드웨어가 연산

	else
		vp |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;	// 사용자가 직접 연산


	// 4. 그리기 컴객체 생성

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(D3DPRESENT_PARAMETERS));
	
	// SetParameters 함수 내부에서 구조체 멤버 변수들의 값을 초기화 진행
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
	m_pDevice->Clear(0,			// rect의 개수()
					nullptr,	// rect를 저장한 배열의 이름(nullptr인 경우 전체 화면 영역을 의미)
					D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER,
					D3DCOLOR_ARGB(255, 125, 125, 125), // 후면 버퍼 색상
					1.f,	// 깊이 버퍼의 초기화 값
					0);		// 스텐실 버퍼의 초기화 값

	m_pDevice->BeginScene(); // 후면 버퍼에 그리기 시점을 알려주는 함수

}

void CDevice::Render_End(HWND hWnd)
{
	m_pDevice->EndScene(); // 후면 버퍼에 그리기의 종료를 알려주는 함수

	m_pDevice->Present(nullptr, nullptr, hWnd, nullptr);
	// 후면 버퍼와 전면 버퍼를 전환하는 함수
	// 1, 2인자 : 렉트의 주소, D3DSWAPEFFECT_COPY로 작성되지 않는한 NULL
	// 3인자 : 출력 대상 윈도우의 핸들, 만약 NULL인 경우 d3dpp.hDeviceWindow에 지정한 핸들값이 그대로 들어감
	// 4인자 : D3DSWAPEFFECT_COPY로 작성되지 않는한 NULL
}

void CDevice::Release(void)
{
	//m_pDevice->Release();
	//m_p3D->Release();	// 컴객체를 삭제하는 함수
	Safe_Release(m_pDevice);
	Safe_Release(m_p3D);

}

void CDevice::SetParameters(D3DPRESENT_PARAMETERS& d3dpp)
{
	// 후면 버퍼
	d3dpp.BackBufferWidth = WINCX;
	d3dpp.BackBufferHeight = WINCY;

	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1; // 후면버퍼 개수

	// 픽셀이 모여 보이는 계단 형식을 뭉개서 부드럽게 처리하는 기법은 멀티 샘플링이라 한다.
	// 그러나 성능저하가 심하기 때문에 사용하지 않는다.

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;


	// D3DSWAPEFFECT_DISCARD : 스왑 체인 방식
	// D3DSWAPEFFECT_FLIP : 버퍼 하나로 뒤집으면서 사용하는 방식
	// D3DSWAPEFFECT_COPY : 더블 버퍼링과 유사한 복사 방식

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	// 디바이스 장치를 사용할 윈도우 창의 핸들
	d3dpp.hDeviceWindow = g_hWnd;
	
	// 창모드로 출력할 것인가 전체화면으로 출력할 것인가?
	d3dpp.Windowed = TRUE;	// TRUE일때 창모드 설정 FALSE면 전체 화면 설정

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	// 모니터 재생율과 실제 화면 출력(시연)의 간격 설정

	// 전체 화면 전환 시 설정할 옵션
	// 전체화면 모드일 경우에는 디바이스 장치가 제어, 창 모드일 때는 OS가 재생률을 관리함

	// 전체 화면 시의 현재 모니터 재생률 대로 알아서 수행
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;


	// 모니터 재생률과 시연의 간격이 발생했을 경우 어떻게 할 것인가

	// D3DPRESENT_INTERVAL_DEFAULT : 적절한 간격을 dx 알아서 결정, 보통 모니터 재생률을 따라감
	// D3DPRESENT_INTERVAL_IMMEDIATE : 그 즉시 계산하지 말고 출력

	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}
