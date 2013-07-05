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

	CParticle* CreateParticle(int nNumOfParticle, int nTextureID);

	void Update(void);
	int Render(void);

};
