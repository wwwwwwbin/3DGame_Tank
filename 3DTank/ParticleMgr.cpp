#include "StdAfx.h"
#include "ParticleMgr.h"

CParticleMgr::CParticleMgr(void)
{
	m_ParticlsList.clear();
}

CParticleMgr::~CParticleMgr(void)
{
}

CParticle* CParticleMgr::CreateParticle( int nNumOfParticle, int nTextureID )
{
	CParticle* pNewParticle = NULL;

	pNewParticle = new CParticle();
	if (!pNewParticle)
		return NULL;

	pNewParticle->Init(nNumOfParticle, nTextureID);

	m_ParticlsList.push_back(pNewParticle);

	return pNewParticle;
}

void CParticleMgr::Update( void )
{
	CParticle* pParticle = NULL;
	for (ParticleList::iterator it = m_ParticlsList.begin(); it != m_ParticlsList.end();)
	{
		pParticle = *it;
		pParticle->UpdataParticle();

		if (pParticle->IsDead())
		{
			delete pParticle;

			it = m_ParticlsList.erase(it);
			continue;
		}
		++it;
	}
}

int CParticleMgr::Render( void )
{
	int nResult  = FALSE;
	int nRetCode = FALSE;

	CParticle* pParticle = NULL;

	{
		CSetParticleRS cRs;
		for (ParticleList::iterator it = m_ParticlsList.begin(); it != m_ParticlsList.end(); ++it)
		{
			pParticle = *it;
			LOG_FAILD_JUMP(pParticle);

			nRetCode = pParticle->Render();
			LOG_FAILD_JUMP(nRetCode);
		}
	}
	

	nResult = TRUE;
Exit0:
	return nResult;
}
