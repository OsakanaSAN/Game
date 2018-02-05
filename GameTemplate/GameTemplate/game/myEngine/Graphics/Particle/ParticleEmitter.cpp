#include "stdafx.h"
#include "ParticleEmitter.h"
#include "Particle.h"


using namespace std;
CParticleEmitter::CParticleEmitter()
{

}
CParticleEmitter::~CParticleEmitter()
{
}
void CParticleEmitter::Init(const SParicleEmitParameter& param)
{
	this->param = param;
	timer = 0.0f;
}
void CParticleEmitter::Update()
{
	if (timer >= param.intervalTime) {
		//パーティクルを生成。
		CParticle* p = new CParticle;
		p->Init(param);
		timer = 0.0f;
		particleList.push_back(p);
		p->SetPosition(Position);
	}
	timer += 1.0f / 60.0f;

	auto IsParticle = particleList.begin();
	while (IsParticle != particleList.end())
	{
		if ((*IsParticle)->IsDete())
		{
			(*IsParticle)->Delete();
			IsParticle = particleList.erase(IsParticle);

		}

		else
		{
			(*IsParticle)->Update();
			IsParticle++;
		
		}


	}

	/*for (auto p : particleList) {
		p->Update();
		if ((*p->IsDete))
		{
			p = particleList.erase(p);
		}
		
	}*/
}
void CParticleEmitter::Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix)
{
	for (auto p : particleList) {
		p->Render(viewMatrix, projMatrix);
	}
}
