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
	particleList.clear();
}
void CParticleEmitter::Init(const SParicleEmitParameter& param)
{
	this->param = param;
	HRESULT hr = D3DXCreateTextureFromFileA(g_pd3dDevice, param.texturePath, &texture);
	timer = 0.0f;
	EndTime = 0.0f;
	//エフェクトファイルのロードが重いのでエミッタのInitで作る。
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
			//パーティクルを生成。
		bool skip = false;
		for (auto* Paticle : particleList)
		{
			if (Paticle->IsDete())
			{

				Paticle->Init(param, texture, *shaderEffect, Position);
				skip = true;
				break;

			}

		}

		if (!skip)
		{

			CParticle* p = new CParticle;
			p->Init(param, texture, *shaderEffect, Position);
			particleList.push_back(p);

		}
		timer = 0.0f;
	}

	for (const auto& p : particleList)
	{
		p->Update();
		if (lockPosition)
		{

			p->SetPosition(Position);

		}
	}

	timer += GameTime().GetFrameDeltaTime();
	EndTime += GameTime().GetFrameDeltaTime();

	

}
void CParticleEmitter::Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix)
{
	
	for (const auto& p : particleList) 
	{

		p->Render(viewMatrix, projMatrix);

	}
}
