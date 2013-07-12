//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   DX_Input.h
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-06-29 11:13
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef DX_INPUT_INCLUDE
#define DX_INPUT_INCLUDE

#define ITEMS_NUM 10

class DX_Input
{
public:
	static DX_Input* GetInstance();

	int  CreateInput(HINSTANCE hInst, HWND hWnd);
	void InputRelease();

	int ReadKeyboard();
	int ReadMouse();
	
	long GetMouseMoveX();
	long GetMouseMoveY();
	
	bool IsKeyPressed(int key);
	bool IsLButtonPressed();
	bool IsRButtonPressed();
	bool IsMButtonPressed();

private:
	DX_Input(void);
	~DX_Input(void);

	int  CreateDirectInput(HINSTANCE hInst);
	int  CreateKeyboard(HWND hInst);
	int  CreateMouse(HWND hInst);
	
	static LPDIRECTINPUT8 ms_pIDirectInput;

	char				m_vKeyBuffer[256];
	DIDEVICEOBJECTDATA	m_vMouseData[ITEMS_NUM];

	LPDIRECTINPUTDEVICE8 m_pKeyboardDevice;
	LPDIRECTINPUTDEVICE8 m_pMouseDevice;

	long m_lMouseMoveX;
	long m_lMouseMoveY;
};

#endif
