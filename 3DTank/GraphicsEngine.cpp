//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   GraphicsEngine.cpp
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-07-08 20:31
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "GraphicsEngine.h"
#include <conio.h>

extern HWND g_hWnd;
extern HINSTANCE g_hInstance;

CGraphicsEngine::CGraphicsEngine(void)
: m_pCamera(NULL)
, m_pDXSoundMgr(NULL)
, m_pTextureMgr(NULL)
, m_pVertexBuffer(NULL)
, m_pIDirect3DDevice(NULL)
, m_nNumOfBillboard(0)
, m_nNumOfModel(0)
, m_nNumOfQuad(0)
, m_bNeedBuiltVertexBuffer(false)
{
	ZeroMemory(m_vPoolOfModel, sizeof(m_vPoolOfModel));
	ZeroMemory(m_vQuads, sizeof(m_vQuads));
	ZeroMemory(m_vBillboards, sizeof(m_vBillboards));

	D3DXMatrixIdentity(&m_mtrIdentity);
}


CGraphicsEngine::~CGraphicsEngine(void)
{
	Direct3DRelease();
}

int CGraphicsEngine::CreateD3DDevice( HWND hWnd, bool bFullScreen /*= true*/ )
{
	int		nResult  = FALSE;
	HRESULT hRetCode = E_FAIL;

	bool bHarwareProcess = false;

	D3DCAPS9				d3dcaps;
	D3DDISPLAYMODE			d3dDisplayMode;
	D3DPRESENT_PARAMETERS	d3dPresent_Param;

	//创建IDirect3D9对象
	m_pIDirect3D = Direct3DCreate9(D3D_SDK_VERSION);
	LOG_FAILD_JUMP(m_pIDirect3D);

	// 检查是否硬件支持顶点运算
	m_pIDirect3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &d3dcaps);

	bHarwareProcess = ((d3dcaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0);

	// 取得显示模式
	hRetCode = m_pIDirect3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3dDisplayMode);
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	// 设置3D设备的渲染参数
	ZeroMemory(&d3dPresent_Param, sizeof(d3dPresent_Param));

	d3dPresent_Param.BackBufferWidth		= SCREEMWIDTH;
	d3dPresent_Param.BackBufferHeight		= SCREEMHEIGH;
	d3dPresent_Param.BackBufferFormat		= d3dDisplayMode.Format;
	d3dPresent_Param.BackBufferCount		= 1;
	d3dPresent_Param.hDeviceWindow			= hWnd;
	d3dPresent_Param.Windowed				= (bFullScreen == false);
	d3dPresent_Param.SwapEffect				= D3DSWAPEFFECT_FLIP;
	d3dPresent_Param.PresentationInterval	= D3DPRESENT_INTERVAL_DEFAULT;
	d3dPresent_Param.EnableAutoDepthStencil = true;
	d3dPresent_Param.AutoDepthStencilFormat = D3DFMT_D16;

	// 创建D3D设备
	if(bHarwareProcess)
	{
		hRetCode = m_pIDirect3D->CreateDevice(
			D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_HARDWARE_VERTEXPROCESSING,
			&d3dPresent_Param,
			&m_pIDirect3DDevice
		); 
		LOG_HRESULT_FAILD_JUMP(hRetCode);
	}
	else
	{
		MessageBox(hWnd, _T("不能使用硬件模式"), NULL, NULL);
		goto Exit0;
	}

	// 初始化摄像机
	m_pCamera = CCamera::GetInstance(CAMERA_TYPE_LANDOBJECT);
	LOG_FAILD_JUMP(m_pCamera);

	m_pCamera->SetPosition(D3DXVECTOR3(20.f,100.f,0.f));
	m_pCamera->SetLook(D3DXVECTOR3(-1.f,0.f,0.f));
	m_pCamera->SetRight(D3DXVECTOR3(0.f,0.f,1.f));
	m_pCamera->SetUp(D3DXVECTOR3(0.f,1.f,0.f));

	SetCamera();

	nResult = TRUE;
Exit0:
	return nResult;
}

// 设置投影变换
void CGraphicsEngine::SetProjection( void )
{
	float fDistance = 1500.f;
	// 投影变换
	D3DXMATRIX projectionMatrix;
	D3DXMatrixPerspectiveFovLH(&projectionMatrix, D3DX_PI / 2, SCREEMWIDTH / SCREEMHEIGH, 1.f, fDistance);

	// 设置投影变换矩阵
	m_pIDirect3DDevice->SetTransform(D3DTS_PROJECTION, &projectionMatrix);

	m_pIDirect3DDevice->SetRenderState(D3DRS_NORMALDEGREE, true);
	// 设置纹理过滤器
	m_pIDirect3DDevice->SetSamplerState(0,D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pIDirect3DDevice->SetSamplerState(0,D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	// 设置多级纹理渐进过滤器
	m_pIDirect3DDevice->SetSamplerState(0,D3DSAMP_MIPFILTER, D3DTEXF_POINT);
	// 不是用光照
	m_pIDirect3DDevice->SetRenderState(D3DRS_LIGHTING, false);
}

void CGraphicsEngine::Direct3DRelease()
{
	SafeRelease(m_pIDirect3DDevice);
}

int CGraphicsEngine::Render( void )
{
	int nResult		= FALSE;
	int nRetCode	= FALSE;
	int nTextureID	= 0;

	IDirect3DTexture9* pTexture = NULL;

	D3DXVECTOR3 v3SpriteCenter(250, 400, 0);
	D3DXMATRIX matView;
	D3DXMATRIX matFaceUser;

	if (m_bNeedBuiltVertexBuffer == true)
	{
		BuiltVertexBuffer();
	}
	m_pIDirect3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.f, 0);
	m_pIDirect3DDevice->BeginScene();

	// 绘制广告牌
	D3DXMatrixScaling(&matFaceUser, 0.1f, -0.1f, 0.1f);
	m_pCamera->GetViewMatrix(&matView);

	for (int i = 0; i < m_nNumOfBillboard; ++i)
	{
		nTextureID = m_vBillboards[i]->m_nTextureID;
		nRetCode = m_pTextureMgr->GetTexture(nTextureID, &pTexture);
		LOG_FAILD_JUMP(nRetCode);
		LOG_FAILD_JUMP(pTexture);

		m_pSprite->SetTransform(&matFaceUser);
		m_pSprite->SetWorldViewLH(&(m_vBillboards[i]->m_matWorld), &matView);
		m_pSprite->Begin(D3DXSPRITE_BILLBOARD | D3DXSPRITE_ALPHABLEND);
		m_pSprite->Draw(pTexture, NULL, &v3SpriteCenter, NULL, 0xffffffff);
		m_pSprite->End();
	}

	// 绘制平面
	// 因为Quad直接采用世界坐标且不会改变，要将变换矩阵设为单位矩阵
	m_pIDirect3DDevice->SetTransform(D3DTS_WORLD,&m_mtrIdentity);	
	m_pIDirect3DDevice->SetStreamSource(0,m_pVertexBuffer,0,sizeof(CQuad::QuadVertex));
	m_pIDirect3DDevice->SetFVF(CQuad::QuadVertex::FVF);
	for (int i = 0; i < m_nNumOfQuad; ++i)
	{
		IDirect3DTexture9* pTexture = NULL;
		nRetCode = m_pTextureMgr->GetTexture(m_vQuads[i]->m_nTextureID, &pTexture);
		LOG_FAILD_JUMP(nRetCode);
		LOG_FAILD_JUMP(pTexture);

		m_pIDirect3DDevice->SetTexture(0, pTexture);
		m_pIDirect3DDevice->DrawPrimitive(D3DPT_TRIANGLELIST, i * 6, 2);
	}

	// 绘制粒子特效
	m_pParticleMgr->Update();
	m_pParticleMgr->Render();

	// 绘制模型
	for(int i = 0; i < m_nNumOfModel; ++i)
	{
		m_pIDirect3DDevice->SetTransform(D3DTS_WORLD,&m_vPoolOfModel[i]->m_mtrWorldMatrix);
		for(int j = 0; j < m_vPoolOfModel[i]->m_nNumOfMaterials; ++j)
		{
			m_pIDirect3DDevice->SetMaterial(&m_vPoolOfModel[i]->m_pD3DMaterialArray[j]);
			m_pIDirect3DDevice->SetTexture(0,m_vPoolOfModel[i]->m_ppDirect3DTextureArray[j]);
			m_vPoolOfModel[i]->m_pD3DXMesh->DrawSubset(j);
		}
	}
	m_pIDirect3DDevice->EndScene();
	m_pIDirect3DDevice->Present(NULL, NULL, NULL, NULL);
	m_nNumOfModel = 0;
	m_nNumOfBillboard = 0;
	
	nResult = TRUE;
Exit0:
	return nResult;
}

CGraphicsEngine* CGraphicsEngine::GetInstance( void )
{
	static CGraphicsEngine* m_pInstance = NULL;
	if (m_pInstance == NULL)
	{
		m_pInstance = new CGraphicsEngine();
	}
	return m_pInstance;
}

void CGraphicsEngine::CameraRotate( float fAngle )
{
	m_pCamera->Yaw(fAngle);
	SetCamera();
}

void CGraphicsEngine::SetCameraPosition( float fX, float fY, float fZ )
{
	m_pCamera->SetPosition(D3DXVECTOR3(fX, fY, fZ));
	SetCamera();
}

void CGraphicsEngine::SetCamera( void )
{
	D3DXMATRIX  viewMatrix;
	m_pCamera->GetViewMatrix(&viewMatrix);
	m_pIDirect3DDevice->SetTransform(D3DTS_VIEW, &viewMatrix);
}

void CGraphicsEngine::ChangeCameraPosition( float fX, float fY, float fZ )
{
	m_pCamera->MovePosition(fX, fY, fZ);
	SetCamera();
}

void CGraphicsEngine::CameraPointRotate( D3DXVECTOR3 pos, float fAngle )
{
	m_pCamera->PointRotate(pos, fAngle);
	
	SetCamera();
}

void CGraphicsEngine::CameraPointRotate( float nPosX, float nPosY, float nPosZ, float fAngle )
{
	D3DXVECTOR3 tmp(nPosX, nPosY, nPosZ);
	CameraPointRotate(tmp, fAngle);
}

int CGraphicsEngine::Initialize( void )
{
	int nResult  = FALSE;
	int nRetCode = FALSE;

	// 初始化输入
	m_pDXInput = DX_Input::GetInstance();
	LOG_FAILD_JUMP(m_pDXInput);

	nRetCode = m_pDXInput->CreateInput(g_hInstance, g_hWnd);
	if (!nRetCode)
	{
		MessageBox(NULL,_T("初始化输入设备失败"),NULL,NULL);
		goto Exit0;
	}

	// 初始化声音
	/*
	m_pDXSoundMgr = DX_SoundManager::GetSoundManager();
	LOG_FAILD_JUMP(m_pDXSoundMgr);

	nRetCode = m_pDXSoundMgr->Init(g_hWnd);
	LOG_FAILD_JUMP(nRetCode);

	nRetCode = m_pDXSoundMgr->SetPrimaryBufferFormat();
	LOG_FAILD_JUMP(nRetCode);
	*/

	// 初始化计时器
	m_pTimer = CTimer::GetInstance();
	LOG_FAILD_JUMP(m_pTimer);

	nRetCode = m_pTimer->InitTime();
	LOG_FAILD_JUMP(nRetCode);

	// 创建3D设备
	nRetCode = CreateD3DDevice(g_hWnd, false);
	LOG_FAILD_JUMP(nRetCode);
	
	// 设置投影矩阵
	SetProjection();

	// 设置视口
	{
		D3DVIEWPORT9 vp = {0, 0, SCREEMWIDTH, SCREEMHEIGH, 0, 1};
		m_pIDirect3DDevice->SetViewport(&vp);

		if (FAILED(D3DXCreateSprite(m_pIDirect3DDevice, &m_pSprite)))
		{
			MessageBox(NULL, "创建精灵接口失败", "警告", MB_OK | MB_ICONINFORMATION);
			goto Exit0;
		}
	}
	
	m_pTextureMgr = TextureMgr::GetInstance();
	LOG_FAILD_JUMP(m_pTextureMgr);

	nRetCode = m_pTextureMgr->Init(m_pIDirect3DDevice);
	LOG_FAILD_JUMP(nRetCode);

	m_pParticleMgr = new CParticleMgr();
	LOG_FAILD_JUMP(m_pParticleMgr);


	nResult = TRUE;
Exit0:
	if (!nResult)
	{
		if (m_pDXInput)
		{
			m_pDXInput->InputRelease();
			m_pDXInput = NULL;
		}
		if (m_pDXSoundMgr)
		{
			m_pDXSoundMgr->Release();
			m_pDXSoundMgr = NULL;
		}
	}
	return nResult;
}

void CGraphicsEngine::BuiltVertexBuffer( void )
{
	m_pIDirect3DDevice->CreateVertexBuffer(
		m_nNumOfQuad * 6 * sizeof(CQuad::QuadVertex),
		D3DUSAGE_WRITEONLY,
		CQuad::QuadVertex::FVF,
		D3DPOOL_MANAGED,
		&m_pVertexBuffer,
		0
	);
	
	CQuad::QuadVertex* pVertexBuf;
	m_pVertexBuffer->Lock(0, 0, (void**)&pVertexBuf, 0);
	for (int i = 0; i < m_nNumOfQuad; ++i)
	{
		pVertexBuf[i*6 + 0] =  m_vQuads[i]->m_vVertex[0];
		pVertexBuf[i*6 + 1] =  m_vQuads[i]->m_vVertex[1];
		pVertexBuf[i*6 + 2] =  m_vQuads[i]->m_vVertex[3];
		pVertexBuf[i*6 + 3] =  m_vQuads[i]->m_vVertex[3];
		pVertexBuf[i*6 + 4] =  m_vQuads[i]->m_vVertex[1];
		pVertexBuf[i*6 + 5] =  m_vQuads[i]->m_vVertex[2];
	}
	m_pVertexBuffer->Unlock();
	m_bNeedBuiltVertexBuffer = false;
}

void CGraphicsEngine::FreeVertexBuffer( void )
{
	SafeRelease(m_pVertexBuffer);
}

void CGraphicsEngine::AddQuad( CQuad* pQuad )
{
	assert(pQuad);
	if (m_nNumOfQuad < MAX_QUAD_SIZE)
	{
		m_vQuads[m_nNumOfQuad++] = pQuad;
		m_bNeedBuiltVertexBuffer = true;
	}
}

void CGraphicsEngine::RemoveQuad( CQuad* pQuad )
{
	assert(pQuad);
	for (int i = 0; i < m_nNumOfQuad; ++i)
	{
		if (m_vQuads[i] == pQuad)
		{
			for (int j = i; j < m_nNumOfQuad - 1; ++j)
			{
				m_vQuads[j] = m_vQuads[j+1];
			}
		}
	}
	m_bNeedBuiltVertexBuffer = true;
}

void CGraphicsEngine::AddModel( C3DModel* pModel )
{
	assert(pModel);
	if (m_nNumOfModel < MAX_MODEL_SIZE)
	{
		m_vPoolOfModel[m_nNumOfModel++] = pModel;
	}
}

void CGraphicsEngine::AddBillboard( CBillboard* pBillboard )
{
	assert(pBillboard);
	if (m_nNumOfBillboard < MAX_BILLBOARD_SIZE)
	{
		m_vBillboards[m_nNumOfBillboard++] = pBillboard;
	}
}

IDirect3DDevice9* CGraphicsEngine::GetD3DDevice( void )
{
	return m_pIDirect3DDevice;
}
