//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   GraphicsEngine.h
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-07-08 20:31
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#pragma once
#include "EngineDefine.h"
#include "Camera.h"
#include "DX_Input.h"
#include "DX_Sound.h"
#include "Timer.h"
#include "3DModel.h"
#include "Quad.h"
#include "Billboard.h"
#include "TextureMgr.h"
#include "ParticleMgr.h"

class CGraphicsEngine
{
public:
	
	static const int MAX_QUAD_SIZE		= 64;
	static const int MAX_MODEL_SIZE		= 1024;
	static const int MAX_BILLBOARD_SIZE = 64;

	CTimer*				m_pTimer;
	CCamera*			m_pCamera;
	DX_Input*			m_pDXInput;
	TextureMgr*			m_pTextureMgr;
	CParticleMgr*		m_pParticleMgr;
	DX_SoundManager*	m_pDXSoundMgr;
	
	static CGraphicsEngine* GetInstance(void);

	IDirect3DDevice9* GetD3DDevice(void);

	int  Initialize(void);
	void Direct3DRelease(); 

	void AddQuad(CQuad* pQuad);
	void RemoveQuad(CQuad* pQuad);

	void AddModel(C3DModel* pModel);

	void AddBillboard(CBillboard* pBillboard);

	void BuiltVertexBuffer(void);
	void FreeVertexBuffer(void);

	void SetCameraPosition(float fX, float fY, float fZ);
	void ChangeCameraPosition(float fX, float fY, float fZ);

	void CameraRotate(float fAngle);
	void CameraPointRotate(D3DXVECTOR3 pos, float fAngle);
	void CameraPointRotate(float nPosX, float nPosY, float nPosZ, float fAngle);

	int Update(void);
	int Render(void);

private:
	
	bool		m_bNeedBuiltVertexBuffer;
	D3DXMATRIX	m_mtrIdentity;

	IDirect3D9*				m_pIDirect3D;
	ID3DXSprite*			m_pSprite;
	IDirect3DDevice9*		m_pIDirect3DDevice;
	IDirect3DVertexBuffer9* m_pVertexBuffer;
	
	CQuad* m_vQuads[MAX_QUAD_SIZE];
	int m_nNumOfQuad;

	C3DModel* m_vPoolOfModel[MAX_MODEL_SIZE];
	int m_nNumOfModel;

	CBillboard* m_vBillboards[MAX_BILLBOARD_SIZE];
	int m_nNumOfBillboard;

	CGraphicsEngine(void);
	~CGraphicsEngine(void);

	void SetCamera(void);
	void SetProjection(void);
	int CreateD3DDevice(HWND hWnd,bool bFullScreen = true);
	void LoadResource(void);
};

