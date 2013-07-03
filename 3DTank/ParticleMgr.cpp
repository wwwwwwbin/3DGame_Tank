#include "StdAfx.h"
#include "ParticleMgr.h"

CParticleMgr::CParticleMgr(void)
{
	m_ParticlsList.clear();
}

CParticleMgr::~CParticleMgr(void)
{
}

CParticle* CParticleMgr::CreateParticle( int nNumOfParticle, IDirect3DTexture9* pParticleTexture )
{
	CParticle* pNewParticle = NULL;

	pNewParticle = new CParticle();
	if (!pNewParticle)
		return NULL;

	pNewParticle->Init(nNumOfParticle, pParticleTexture);

	

	m_ParticlsList.push_back(pNewParticle);

	return pNewParticle;
}

void CParticleMgr::Update( void )
{
	CParticle* pParticle = NULL;

	ParticleList::iterator itDelete;
	for (ParticleList::iterator it = m_ParticlsList.begin(); it != m_ParticlsList.end();)
	{
		pParticle = *it;
		pParticle->UpdataParticle();

		if (pParticle->IsDead())
		{
			itDelete = it;
			++it;
			delete pParticle;
			m_ParticlsList.erase(itDelete);
		}
		else
			++it;
	}

}

void CParticleMgr::Render( void )
{
	CParticle* pParticle = NULL;

	CParticle::SetParticleRS();
	
	for (ParticleList::iterator it = m_ParticlsList.begin(); it != m_ParticlsList.end(); ++it)
	{
		pParticle = *it;
		pParticle->Render();
	}

	CParticle::UnSetParticleRS();
}
