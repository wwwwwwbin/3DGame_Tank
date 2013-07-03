#pragma once

#include "Particle.h"
#include <list>

class CParticleMgr
{
private:
	typedef std::list<CParticle*> ParticleList;
	ParticleList m_ParticlsList;

public:
	CParticleMgr(void);
	~CParticleMgr(void);

	CParticle* CreateParticle(int nNumOfParticle, IDirect3DTexture9* pParticleTexture);

	void Update(void);
	void Render(void);

};
