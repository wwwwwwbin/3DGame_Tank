#include "StdAfx.h"
#include "Particle.h"
#include "GraphicsEngine.h"

inline DWORD FToW(float f){return *((DWORD*)&f);}

CParticle::CParticle(void)
: m_bIsDead(false)
{
}

CParticle::~CParticle(void)
{
	UnInit();
}

bool CParticle::Init( int nNumOfParticle, IDirect3DTexture9* pParticleTexture )
{
	m_nNumOfParticles = nNumOfParticle;
	m_pParticles = new PARTICLE[m_nNumOfParticles];
	if (!m_pParticles)
		return false;
	m_pParticleTexture = pParticleTexture;
	m_lVertexBufOffset = 0;
	m_lBolckSize = 500;

	CGraphicsEngine* pEngine = CGraphicsEngine::GetInstance();
	if (FAILED(pEngine->GetD3DDevice()->CreateVertexBuffer(
		m_nNumOfParticles * sizeof(PARTICLE_RENDERSTRUCT),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		PARTICLE_FVF, 
		D3DPOOL_DEFAULT, 
		&m_pParticleVertexBuf,
		NULL
	)))
	{
		MessageBox(NULL, "创建粒子缓冲区失败", "error", NULL);
		return false;
	}

	SetPosition(0.f, 20.f, 40.f);
	SetColor(255, 255, 0);
	SetFriction(0.01f);
	SetGravity(0.f, -0.001f, 0.f);
	SetMass(5.25f);
	SetRadius(0.1f);

	return true;
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

	m_pParticles[nDeadIndex].m_fLife = m_fLife;
	m_pParticles[nDeadIndex].m_vPosition = m_vPosition;
	m_pParticles[nDeadIndex].m_fFriction = m_fFriction;
	m_pParticles[nDeadIndex].m_Color = m_Color;
	m_pParticles[nDeadIndex].m_fMass = m_fMass;

	m_pParticles[nDeadIndex].m_vVelocity = D3DXVECTOR3(fVx, fVy, fVz);
}

void CParticle::UpdataParticle( void )
{
	int nRandom = 0;
	bool bHasParticle = false;

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

			bHasParticle = true;
		}
	}

	if (!bHasParticle)
		m_bIsDead = true;
}

void CParticle::SetParticleRS( void )
{
	IDirect3DDevice9* pDevice = NULL;

	pDevice = CGraphicsEngine::GetInstance()->GetD3DDevice();

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

	pDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
	pDevice->SetRenderState(D3DRS_POINTSCALEENABLE, false);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
}

void CParticle::Render( void )
{
	IDirect3DDevice9* pDevice = NULL;

	pDevice = CGraphicsEngine::GetInstance()->GetD3DDevice();

	pDevice->SetFVF(PARTICLE_FVF);
	if (m_pParticleTexture)
	{
		pDevice->SetTexture(0, m_pParticleTexture);
	}
	pDevice->SetStreamSource(0, m_pParticleVertexBuf, 0, sizeof(PARTICLE_RENDERSTRUCT));
	if (m_lVertexBufOffset >= (long)(m_nNumOfParticles * sizeof(PARTICLE_RENDERSTRUCT)))
		m_lVertexBufOffset = 0;

	PARTICLE_RENDERSTRUCT* pVertexBuf =NULL;
	m_pParticleVertexBuf->Lock( m_lVertexBufOffset * sizeof(PARTICLE_RENDERSTRUCT), 
								m_lBolckSize * sizeof(PARTICLE_RENDERSTRUCT), 
								(void**)&pVertexBuf, D3DLOCK_DISCARD);
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
				pDevice->DrawPrimitive(D3DPT_POINTLIST, m_lVertexBufOffset, m_lBolckSize);
				m_lVertexBufOffset += m_lBolckSize;
				m_pParticleVertexBuf->Lock( m_lVertexBufOffset * sizeof(PARTICLE_RENDERSTRUCT), 
										   m_lBolckSize * sizeof(PARTICLE_RENDERSTRUCT), 
										   (void**)&pVertexBuf, D3DLOCK_DISCARD);
				lNumInBlock = 0;
			}
		}
	}
	m_pParticleVertexBuf->Unlock();
	if (lNumInBlock)
	{
		pDevice->DrawPrimitive(D3DPT_POINTLIST, m_lVertexBufOffset, lNumInBlock);
	}
	m_lVertexBufOffset = 0;
}

void CParticle::Explode( D3DXVECTOR3 vPosition )
{
	float fAlfa = 0.f;
	float fBeta = 0.f;

	m_fLife = 10.f;
	m_fRadius = 0.05f;
	m_vPosition = vPosition;

	int nNumOfParticles = m_nNumOfParticles;

	while(nNumOfParticles > 0)
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
		--nNumOfParticles;
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
	delete[] m_pParticles;
	SafeRelease(m_pParticleVertexBuf);
}
