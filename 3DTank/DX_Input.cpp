#include "StdAfx.h"
#include "DX_Input.h"

LPDIRECTINPUT8 DX_Input::ms_pIDirectInput = NULL;

DX_Input::DX_Input(void)
: m_pKeyboardDevice(NULL)
, m_pMouseDevice(NULL)
, m_lMouseMoveX(0)
, m_lMouseMoveY(0)
{
}

DX_Input::~DX_Input(void)
{
	InputRelease();
}

int DX_Input::CreateInput( HINSTANCE hInst, HWND hWnd )
{
	int nResult  = FALSE;
	int nRetCode = FALSE;

	// int iMin=-100,int iMax=100,int iDeadZone=20,bool bUseJoystick=false
	nRetCode = CreateDirectInput(hInst);
	CHECK_FAILD_JUMP(nRetCode);

	nRetCode = CreateKeyboard(hWnd);
	CHECK_FAILD_JUMP(nRetCode);

	nRetCode = CreateMouse(hWnd);
	CHECK_FAILD_JUMP(nRetCode);
	
	nResult = TRUE;
Exit0:
	return nResult;
}

int DX_Input::CreateDirectInput( HINSTANCE hInst )
{
	int		nResult  = FALSE;
	HRESULT hRetCode = E_FAIL;

	hRetCode = DirectInput8Create(hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&ms_pIDirectInput, NULL);

	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "建立IDIRECTINPUT8接口对象失败", "警告", MB_OK || MB_ICONINFORMATION);
		goto Exit0;
	}

	nResult = TRUE;
Exit0:
	return nResult;
}

int DX_Input::CreateKeyboard( HWND hWnd)
{
	int		nResult  = FALSE;
	HRESULT hRetCode = E_FAIL;

	//创建键盘设备
	hRetCode = ms_pIDirectInput->CreateDevice(GUID_SysKeyboard, &m_pKeyboardDevice, NULL);
	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "建立键盘输入设备对象失败", "警告", MB_OK | MB_ICONINFORMATION);
		goto Exit0;
	}

	//设置键盘输入设备格式
	hRetCode = m_pKeyboardDevice->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "设置键盘读取数据格式失败","警告", MB_OK | MB_ICONINFORMATION);
		goto Exit0;
	}

	//设置协调级别
	hRetCode = m_pKeyboardDevice->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "设置协调级别失败", "警告", MB_OK | MB_ICONINFORMATION);
		goto Exit0;
	}

	//获取键盘输入设备的访问权
	hRetCode = m_pKeyboardDevice->Acquire();
	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "获取键盘输入设备的访问权失败", "警告", MB_OK|MB_ICONINFORMATION);
		goto Exit0;
	}

	//键盘缓冲区清零
	ZeroMemory(m_vKeyBuffer, sizeof(m_vKeyBuffer));
	
	nResult = TRUE;
Exit0:
	if (!nResult)
	{
		InputRelease();
	}
	return nResult;
}

int DX_Input::CreateMouse( HWND hWnd )
{
	int		nResult  = FALSE;
	HRESULT hRetCode = E_FAIL;

	//鼠标输入设备
	hRetCode = ms_pIDirectInput->CreateDevice(GUID_SysMouse, &m_pMouseDevice, NULL);
	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "建立鼠标输入设备失败", "警告", MB_OK | MB_ICONINFORMATION);
		goto Exit0;
	}

	//设置鼠标设备的数据格式
	hRetCode = m_pMouseDevice->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "建立鼠标设备的数据格式失败", "警告", MB_OK | MB_ICONINFORMATION);
		goto Exit0;
	}

	//设置鼠标设备的协调级别
	hRetCode = m_pMouseDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "建立鼠标设备的协调级别失败", "警告", MB_OK | MB_ICONINFORMATION);
		goto Exit0;
	}

	//设置鼠标设备的属性（使用缓冲区读数据）??
	DIPROPDWORD dipROPDWORD;
	dipROPDWORD.diph.dwSize			= sizeof(DIPROPDWORD);
	dipROPDWORD.diph.dwHeaderSize	= sizeof(DIPROPHEADER);
	dipROPDWORD.diph.dwObj			= 0;
	dipROPDWORD.diph.dwHow			= DIPH_DEVICE;
	dipROPDWORD.dwData				= ITEMS_NUM;

	hRetCode = m_pMouseDevice->SetProperty(DIPROP_BUFFERSIZE, &dipROPDWORD.diph);
	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "设置鼠标设备属性失败", "警告", MB_OK | MB_ICONINFORMATION);
		goto Exit0;
	}

	//获取鼠标访问权
	hRetCode = m_pMouseDevice->Acquire();
	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "获取鼠标设备的访问权失败", "警告", MB_OK | MB_ICONINFORMATION);
		goto Exit0;
	}

	nResult = TRUE;
Exit0:
	if (!nResult)
	{
		InputRelease();
	}
	return nResult;
}

int DX_Input::ReadMouse()
{
	int		nResult   = FALSE;
	HRESULT hRetCode  = E_FAIL;
	DWORD	dwReadNum = 1;

	ZeroMemory(m_vMouseData, sizeof(m_vMouseData));

	for (int i = 0 ; i < ITEMS_NUM ; ++i)
	{
		hRetCode = m_pMouseDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &m_vMouseData[i], &dwReadNum, 0);
		if (hRetCode == DIERR_INPUTLOST)
		{
			m_pMouseDevice->Acquire();

			hRetCode = m_pMouseDevice->GetDeviceData(sizeof(DIDEVICEOBJECTDATA), &m_vMouseData[i], &dwReadNum, 0);
			if (FAILED(hRetCode))
			{
				MessageBox(NULL, "取得鼠标缓冲区数据失败", "警告", MB_OK | MB_ICONINFORMATION);
				goto Exit0;
			}
		}
		if (m_vMouseData[i].dwOfs == DIMOFS_X)
		{
			m_lMouseMoveX += m_vMouseData[i].dwData;
		}
		if (m_vMouseData[i].dwOfs == DIMOFS_Y)
		{
			m_lMouseMoveY += m_vMouseData[i].dwData;
		}
	}

	nResult = TRUE;
Exit0:
	return nResult;
}

bool DX_Input::IsLButtonPressed()
{
	for (int i = 0; i < ITEMS_NUM; ++i)
	{
		if (m_vMouseData[i].dwOfs == DIMOFS_BUTTON0 && m_vMouseData[i].dwData & 0x80)
		{
			return true;
		}
	}
	return false;
}

bool DX_Input::IsRButtonPressed()
{
	for (int i = 0; i < ITEMS_NUM ; ++i)
	{
		if (m_vMouseData[i].dwOfs == DIMOFS_BUTTON1 && m_vMouseData[i].dwData & 0x80)
		{
			return true;
		}
	}
	return false;
}

bool DX_Input::IsMButtonPressed()
{
	for (int i = 0; i < ITEMS_NUM ; ++i)
	{
		if (m_vMouseData[i].dwOfs == DIMOFS_BUTTON2 && m_vMouseData[i].dwData & 0x80)
		{
			return true;
		}
	}
	return false;
}

void DX_Input::InputRelease()
{
	if (m_pKeyboardDevice)
	{
		m_pKeyboardDevice ->Unacquire();
	}
	if (m_pMouseDevice)
	{
		m_pMouseDevice -> Unacquire();
	}

	SafeRelease(m_pKeyboardDevice);
	SafeRelease(m_pMouseDevice);
	SafeRelease(ms_pIDirectInput);
}

int DX_Input::ReadKeyboard()
{
	int		nResult  = FALSE;
	HRESULT hRetCode = E_FAIL;

	hRetCode = m_pKeyboardDevice->GetDeviceState(sizeof(m_vKeyBuffer),(LPVOID)m_vKeyBuffer);
	if (hRetCode == DIERR_INPUTLOST)
	{
		m_pKeyboardDevice->Acquire();
	}

	hRetCode = m_pKeyboardDevice->GetDeviceState(sizeof(m_vKeyBuffer),(LPVOID)m_vKeyBuffer);
	CHECK_FAILD_JUMP(!FAILED(hRetCode));

	nResult = TRUE;
Exit0:
	return nResult;
}

bool DX_Input::IsKeyPressed( int key )
{
	return ((m_vKeyBuffer[key] & 0x80) ? true : false);
}

long DX_Input::GetMouseMoveX()
{
	return m_lMouseMoveX;	
}

long DX_Input::GetMouseMoveY()
{
	return m_lMouseMoveY;
}

DX_Input* DX_Input::GetInstance()
{
	static DX_Input* pInstance = NULL;
	if (!pInstance)
	{
		pInstance = new DX_Input();
	}
	return pInstance;
}
