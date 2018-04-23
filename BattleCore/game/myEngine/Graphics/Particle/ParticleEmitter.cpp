#include "stdafx.h"
#include "ParticleEmitter.h"
#include "Particle.h"

ID3DXEffect* CParticleEmitter::shaderEffect = NULL;

using namespace std;
CParticleEmitter::CParticleEmitter()
{

}
CParticleEmitter::~CParticleEmitter()
{
	for (const auto& paticl : particleList)
	{
		delete paticl;
	}
	//shaderEffect->Release();
	particleList.clear();
}
void CParticleEmitter::Init(const SParicleEmitParameter& param)
{
	this->param = param;
	HRESULT hr = D3DXCreateTextureFromFileA(g_pd3dDevice, param.texturePath, &texture);
	timer = 0.0f;
	//�G�t�F�N�g�t�@�C���̃��[�h���d���̂ŃG�~�b�^��Init�ō��B
	LPD3DXBUFFER  compileErrorBuffer = NULL;
	if (shaderEffect == NULL)
	{
		hr = D3DXCreateEffectFromFile(
			g_pd3dDevice,
			"Assets/Shader/ColorTexPrim.fx",
			NULL,
			NULL,
#ifdef _DEBUG
			D3DXSHADER_DEBUG,
#else
			D3DXSHADER_SKIPVALIDATION,
#endif
			NULL,
			&shaderEffect,
			&compileErrorBuffer
		);

	}

	if (FAILED(hr)) {
		MessageBox(NULL, (char*)(compileErrorBuffer->GetBufferPointer()), "error", MB_OK);
		std::abort();
	}


}
void CParticleEmitter::Update()
{
	
	if (timer >= param.intervalTime && EndTime <= param.Endtimer)
	{
			//�p�[�e�B�N���𐶐��B
			CParticle* p = new CParticle;
			p->Init(param, texture, *shaderEffect,Position);
			timer = 0.0f;
			particleList.push_back(p);

		/*	p = new CParticle;
			p->Init(param, texture, *shaderEffect, Position);
			timer = 0.0f;
			particleList.push_back(p);*/

	
	}
	for (const auto& p : particleList)
	{
		p->Update();
		if (lockPosition)
		{

			p->SetPosition(Position);

		}
	}

	timer += 1.0f / 60.0f;
	EndTime += 1.0f / 60.0f;

	/*auto IsParticle = particleList.begin();
	while (IsParticle != particleList.end())
	{
	(*IsParticle)->Update();

	if ((*IsParticle)->IsDete())
	{
	//(*IsParticle)->Delete();
	//IsParticle = particleList.erase(IsParticle);
	//(*IsParticle)->Delete();
	IsParticle++;
	}

	else
	{
	//(*IsParticle)->Update();
	IsParticle++;

	}


	}*/

}
void CParticleEmitter::Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix)
{
	
	for (const auto& p : particleList) 
	{

		p->Render(viewMatrix, projMatrix);

	}
}