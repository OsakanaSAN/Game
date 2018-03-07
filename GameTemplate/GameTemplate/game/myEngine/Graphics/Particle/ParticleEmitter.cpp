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
	
	if (timer >= param.intervalTime) {
			//パーティクルを生成。
			CParticle* p = new CParticle;
			p->Init(param, texture, *shaderEffect);
			timer = 0.0f;
			particleList.push_back(p);
			p->SetPosition(Position);
	
	}
	timer += 1.0f / 60.0f;

	
	//auto IsParticle = particleList.begin();
	//while (IsParticle != particleList.end())
	//{
	//	(*IsParticle)->Update();

	//	if ((*IsParticle)->IsDete())
	//	{
	//		//(*IsParticle)->Delete();
	//		//IsParticle = particleList.erase(IsParticle);
	//		//(*IsParticle)->Delete();
	//		IsParticle++;
	//	}

	//	else
	//	{
	//		//(*IsParticle)->Update();
	//		IsParticle++;
	//	
	//	}


	//}

	for (const auto& p : particleList)
	{
		p->Update();
		p->SetPosition(Position);
	}
}
void CParticleEmitter::Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix)
{
	int count = 0;
	for (const auto& p : particleList) {
		count++;
		p->Render(viewMatrix, projMatrix);
	}
}
