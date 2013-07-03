#pragma once
#include "EngineDefine.h"
#include "Camera.h"
#include "DX_Input.h"
#include "DX_Sound.h"
#include "Timer.h"
#include "3DModel.h"
#include "Quad.h"
#include "Billboard.h"
#include "ParticleMgr.h"

class CGraphicsEngine
{
public:
	
	static const int MAX_MODEL_SIZE = 1024;
	static const int MAX_QUAD_SIZE = 64;
	static const int MAX_BILLBOARD_SIZE = 64;

public:
	
	IDirect3DTexture9* m_vTextures[TEXTURE_INDEX_SIZE];
	
	CCamera* m_pCamera;
	Timer* m_pTimer;
	DX_Input* m_pDXInput;
	DX_SoundManager* m_pDXSoundMgr;
	CParticleMgr* m_pParticleMgr;
	
	bool Initialize(void);
	static CGraphicsEngine* GetInstance(void);
	IDirect3DDevice9* GetD3DDevice(void){return m_pIDirect3DDevice;}

	void AddQuad(CQuad* pQuad);
	void RemoveQuad(CQuad* pQuad);

	void AddModel(C3DModel* pModel);
	//void RemoveModel(C3DModel* pModel);

	void AddBillboard(CBillboard* pBillboard);

	void BuiltVertexBuffer(void);
	void FreeVertexBuffer(void);

	void SetCameraPosition(float fX,float fY,float fZ);
	void ChangeCameraPosition(float fX,float fY,float fZ);
	void CameraRotate(float fAngle);
	void CameraPointRotate(D3DXVECTOR3 pos,float fAngle);
	void CameraPointRotate(float nPosX,float nPosY,float nPosZ,float fAngle);

	void Direct3DRelease(); 
	void Render(void);

private:
	
	ID3DXSprite* m_pSprite;
	IDirect3D9* m_pIDirect3D;
	IDirect3DDevice9* m_pIDirect3DDevice;
	IDirect3DVertexBuffer9* m_pVertexBuffer;
	D3DXMATRIX m_mtrIdentity;
	bool m_bNeedBuiltVertexBuffer;

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

