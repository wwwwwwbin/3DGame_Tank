//////////////////////////////////////////////////////////////////////////////////////
//
//  FileName    :   Particle.cpp
//  Version     :   1.0
//  Creator     :   weibin Huang
//  Date        :   2013-06-29 11:13
//  Comment     :   
//
//////////////////////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "Particle.h"
#include "GraphicsEngine.h"
#include "TextureMgr.h"

inline DWORD FToW(float f){return *((DWORD*)&f);}

CParticle::CParticle(void)
: m_nTextureID(0)
, m_bIsDead(false)
{
}

CParticle::~CParticle(void)
{
	UnInit();
}

int CParticle::Init( int nNumOfParticle, int nTextureID )
{
	int		nResult  = FALSE;
	HRESULT hRetCode = E_FAIL;

	CGraphicsEngine* pEngine = NULL;

	pEngine = CGraphicsEngine::GetInstance();
	LOG_FAILD_JUMP(pEngine);
	
	m_nNumOfParticles = nNumOfParticle;

	m_pParticles = new PARTICLE[m_nNumOfParticles];
	LOG_FAILD_JUMP(m_pParticles);

	m_nTextureID = nTextureID;

	m_lVertexBufOffset	= 0;
	m_lBolckSize		= 500;

	hRetCode = pEngine->GetD3DDevice()->CreateVertexBuffer(
		m_nNumOfParticles * sizeof(PARTICLE_RENDERSTRUCT),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		PARTICLE_FVF, 
		D3DPOOL_DEFAULT, 
		&m_pParticleVertexBuf,
		NULL
	);

	if (FAILED(hRetCode))
	{
		MessageBox(NULL, "创建粒子缓冲区失败", "error", NULL);
		goto Exit0;
	}

	SetPosition(0.f, 20.f, 40.f);
	SetColor(255, 255, 0);
	SetFriction(0.01f);
	SetGravity(0.f, -0.001f, 0.f);
	SetMass(5.25f);
	SetRadius(0.1f);

	nResult = TRUE;
Exit0:
	if (!nResult)
	{
		UnInit();
	}
	return nResult;
}

void CParticle::CreateParticle( float fVx, float fVy, float fVz )
{
	int nDeadIndex = -1;
	for (int i = 0; i < m_nNumOfParticles; ++i)
	{
		if (m_pParticles[i].m_fLife <= 0.f)
		{
			nDeadIndex = i;
			break;
		}
	}
	if (nDeadIndex == -1)
		return ;

	m_pParticles[nDeadIndex].m_fLife		= m_fLife;
	m_pParticles[nDeadIndex].m_vPosition	= m_vPosition;
	m_pParticles[nDeadIndex].m_fFriction	= m_fFriction;
	m_pParticles[nDeadIndex].m_Color		= m_Color;
	m_pParticles[nDeadIndex].m_fMass		= m_fMass;
	m_pParticles[nDeadIndex].m_vVelocity	= D3DXVECTOR3(fVx, fVy, fVz);
}

void CParticle::UpdataParticle( void )
{
	int  nRandom  = 0;
	bool bDead	  = true;

	for (int i = 0; i < m_nNumOfParticles; ++i)
	{
		m_pParticles[i].m_fLife -= 1;
		if (m_pParticles[i].m_fLife > 0.f)
		{
			m_pParticles[i].m_vPosition += ( m_pParticles[i].m_vVelocity * m_pParticles[i].m_fMass);
			m_pParticles[i].m_vVelocity *= ( 1 - m_pParticles[i].m_fFriction );
			m_pParticles[i].m_vVelocity += m_vGravity;

			nRandom = rand()%3;
			if (nRandom == 0)
				m_pParticles[i].m_Color = COLOR_A;
			else if (nRandom == 1)
				m_pParticles[i].m_Color = COLOR_B;
			else
				m_pParticles[i].m_Color = COLOR_C;

			bDead = false;
		}
	}

 	m_bIsDead = bDead;
}

void CParticle::SetParticleRS( void )
{
	IDirect3DDevice9* pDevice = NULL;

	pDevice = CGraphicsEngine::GetInstance()->GetD3DDevice();
	assert(pDevice);

	pDevice->SetRenderState(D3DRS_LIGHTING, false);
	pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	pDevice->SetRenderState(D3DRS_POINTSIZE, FToW(0.2f));
	pDevice->SetRenderState(D3DRS_POINTSIZE_MIN, FToW(0.f));
	pDevice->SetRenderState(D3DRS_POINTSCALE_A, FToW(0.f));
	pDevice->SetRenderState(D3DRS_POINTSCALE_B, FToW(0.f));
	pDevice->SetRenderState(D3DRS_POINTSCALE_C, FToW(1.f));

	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

void CParticle::UnSetParticleRS( void )
{
	IDirect3DDevice9* pDevice = NULL;

	pDevice = CGraphicsEngine::GetInstance()->GetD3DDevice();
	assert(pDevice);

	pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

int CParticle::Render( void )
{
	int		nResult  = FALSE;
	int		nRetCode = FALSE;
	HRESULT hRetCode = E_FAIL;
	
	IDirect3DDevice9*		pDevice		= NULL;
	IDirect3DTexture9*		pTexture	= NULL;
	PARTICLE_RENDERSTRUCT*	pVertexBuf	= NULL;

	pDevice = CGraphicsEngine::GetInstance()->GetD3DDevice();
	LOG_FAILD_JUMP(pDevice);

	pDevice->SetFVF(PARTICLE_FVF);

	nRetCode = TextureMgr::GetInstance()->GetTexture(m_nTextureID, &pTexture);
	LOG_FAILD_JUMP(nRetCode);
	LOG_FAILD_JUMP(pTexture);

	hRetCode = pDevice->SetTexture(0, pTexture);
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	hRetCode = pDevice->SetStreamSource(0, m_pParticleVertexBuf, 0, sizeof(PARTICLE_RENDERSTRUCT));
	LOG_HRESULT_FAILD_JUMP(hRetCode);

	if (m_lVertexBufOffset >= (long)(m_nNumOfParticles * sizeof(PARTICLE_RENDERSTRUCT)))
	{
		m_lVertexBufOffset = 0;
	}

	hRetCode = m_pParticleVertexBuf->Lock(
		m_lVertexBufOffset * sizeof(PARTICLE_RENDERSTRUCT), 
		m_lBolckSize * sizeof(PARTICLE_RENDERSTRUCT), 
		(void**)&pVertexBuf, D3DLOCK_DISCARD
	);
	//LOG_FAILD_JUMP(hRetCode == D3D_OK);

	long lNumInBlock = 0;
	for (int i = 0; i < m_nNumOfParticles; ++i)
	{
		if (m_pParticles[i].m_fLife > 0.f)
		{
			pVertexBuf->m_vPosition = m_pParticles[i].m_vPosition;
			pVertexBuf->m_Color = m_pParticles[i].m_Color;
			++pVertexBuf;
			++lNumInBlock;
			if (lNumInBlock >= m_lBolckSize)
			{
				m_pParticleVertexBuf->Unlock();
				hRetCode = pDevice->DrawPrimitive(D3DPT_POINTLIST, m_lVertexBufOffset, m_lBolckSize);
				LOG_HRESULT_FAILD_JUMP(hRetCode);

				m_lVertexBufOffset += m_lBolckSize;
				hRetCode = m_pParticleVertexBuf->Lock(
					m_lVertexBufOffset * sizeof(PARTICLE_RENDERSTRUCT), 
					m_lBolckSize * sizeof(PARTICLE_RENDERSTRUCT), 
					(void**)&pVertexBuf, 
					D3DLOCK_DISCARD
				);
				//LOG_FAILD_JUMP(hRetCode == D3D_OK);

				lNumInBlock = 0;
			}
		}
	}
	m_pParticleVertexBuf->Unlock();
	if (lNumInBlock)
	{
		hRetCode = pDevice->DrawPrimitive(D3DPT_POINTLIST, m_lVertexBufOffset, lNumInBlock);
		LOG_HRESULT_FAILD_JUMP(hRetCode);
	}
	m_lVertexBufOffset = 0;

	nResult = TRUE;
Exit0:
	return nResult;
}

void CParticle::Explode( D3DXVECTOR3 vPosition )
{
	float fAlfa = 0.f;
	float fBeta = 0.f;

	m_fLife		= 10.f;
	m_fRadius	= 0.05f;
	m_vPosition = vPosition;

	for (int i = 0; i < m_nNumOfParticles;  ++i)
	{
		fBeta = fRANDOM * D3DX_PI/2.f;
		fAlfa = fRANDOM * 2.f * D3DX_PI;

		// 		cosf(fBeta) * sinf(fAlfa) * (m_fRadius * fRANDOM), 
		// 		cosf(fBeta) * cosf(fAlfa) * (m_fRadius * fRANDOM),
		// 		sinf(fBeta) * (m_fRadius * fRANDOM)

		CreateParticle( 
			cosf(fBeta) * sinf(fAlfa) * (m_fRadius * fRANDOM), 
			cosf(fBeta) * cosf(fAlfa) * (m_fRadius * fRANDOM),
			cosf(fBeta) * sinf(fRANDOM * 2.f * D3DX_PI) * (m_fRadius * fRANDOM)
		);
	}
}


void CParticle::Shoot( D3DXVECTOR3 vStart, D3DXVECTOR3 vDirection )
{
	D3DXVECTOR3 vPosition = vStart;
	D3DXVECTOR3 vDelta = vDirection * 0.8f;

	float fAlfa = 0.f;
	float fBeta = 0.f;

	for (int i = 0; i < m_nNumOfParticles; ++i)
	{
		m_pParticles[i].m_fLife = 5;
		m_pParticles[i].m_vPosition = vPosition;
		m_pParticles[i].m_fFriction = m_fFriction;
		m_pParticles[i].m_Color = m_Color;
		m_pParticles[i].m_fMass = m_fMass;

		fBeta = fRANDOM * D3DX_PI/2.f;
		fAlfa = fRANDOM * 2.f * D3DX_PI;

		m_pParticles[i].m_vVelocity = D3DXVECTOR3(
			cosf(fBeta) * sinf(fAlfa) * (m_fRadius * fRANDOM), 
			cosf(fBeta) * cosf(fAlfa) * (m_fRadius * fRANDOM),
			cosf(fBeta) * sinf(fRANDOM * 2.f * D3DX_PI) * (m_fRadius * fRANDOM)
		);

		vPosition += vDelta;
	}
}

void CParticle::Fire( D3DXVECTOR3 vStart, D3DXVECTOR3 vDirection )
{
	float fAlfa = 0.f;
	float fBeta = 0.f;
	float fLenth = 5;

	D3DXVECTOR3 vPosition = vStart + vDirection * fLenth;
	D3DXVECTOR3 vDelta = vDirection * (fLenth/(float)m_nNumOfParticles);

	m_fRadius = 0.05f;

	for (int i = 0; i < m_nNumOfParticles; ++i)
	{
		m_pParticles[i].m_fLife = 10;
		m_pParticles[i].m_vPosition = vPosition;
		m_pParticles[i].m_fFriction = m_fFriction;
		m_pParticles[i].m_Color = m_Color;
		m_pParticles[i].m_fMass = m_fMass;

		fBeta = fRANDOM * D3DX_PI/2.f;
		fAlfa = fRANDOM * 2.f * D3DX_PI;

		m_pParticles[i].m_vVelocity = D3DXVECTOR3(
			cosf(fBeta) * sinf(fAlfa) * (m_fRadius * fRANDOM), 
			cosf(fBeta) * cosf(fAlfa) * (m_fRadius * fRANDOM),
			cosf(fBeta) * sinf(fRANDOM * 2.f * D3DX_PI) * (m_fRadius * fRANDOM)
		);

		vPosition -= vDelta;
	}
}

void CParticle::UnInit( void )
{
	if (m_pParticles)
	{
		delete[] m_pParticles;
	}
	SafeRelease(m_pParticleVertexBuf);
}

void CParticle::SetLife( float fLife )
{
	m_fLife = fLife;
}

void CParticle::SetMass( float fMass )
{
	m_fMass = fMass;
}

void CParticle::SetPosition( float fX, float fY, float fZ )
{
	m_vPosition = D3DXVECTOR3(fX, fY, fZ);
}

void CParticle::SetColor( int r, int g, int b )
{
	m_Color = D3DCOLOR_XRGB(r, g, b);
}

void CParticle::SetFriction( float fFrition )
{
	m_fFriction = fFrition;
}

void CParticle::SetGravity( float fX, float fY, float fZ )
{
	m_vGravity = D3DXVECTOR3(fX, fY, fZ);
}

void CParticle::SetRadius( float fRadius )
{
	m_fRadius = fRadius;
}

bool CParticle::IsDead( void )
{
	return m_bIsDead;
}

CSetParticleRS::CSetParticleRS( void )
{
	CParticle::SetParticleRS();
}

CSetParticleRS::~CSetParticleRS( void )
{
	CParticle::UnSetParticleRS();
}
