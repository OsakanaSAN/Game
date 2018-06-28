#include "stdafx.h"
#include "Sea.h"


Sea::Sea()
{
}


Sea::~Sea()
{
}
void Sea::Start(D3DXVECTOR3 pos, D3DXQUATERNION rot)
{

	
	modelData.LoadModelData("Assets/modelData/Wave.X", NULL);
	//ロードしたモデルデータを使ってSkinModelを初期化。
	model.Init(&modelData);

	position = pos;
	rotation = rot;

	light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, -0.707f, 0.0f, 1.0f));
	light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));


	light.SetDiffuseLightColor(0, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	light.SetDiffuseLightColor(1, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	light.SetDiffuseLightColor(2, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	light.SetDiffuseLightColor(3, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));


	light.SetAmbientLight(D3DXVECTOR4(0.0f, 0.0f, 0.5f, 1.0f));

	model.SetLight(&light);

	model.SetWaveTexture();
	model.SetWaveTexture2();
	model.SetSkyTexture();
	model.SetWave(true);

	SParicleEmitParameter SparticleEmit;
	//パーティクルの設定
	SparticleEmit.texturePath = "Assets/Particle/Smoke0.jpg";
	static float w = 10.0f;
	static float h = 10.0f;
	SparticleEmit.w = w;
	SparticleEmit.h = h;
	SparticleEmit.intervalTime = 0.1f;
	SparticleEmit.Endtimer = 50.0f;
	SparticleEmit.brightness = 0.2f;
	SparticleEmit.life = 5.0f;
	static D3DXVECTOR3 randomMargin = { 1.5f,1.5f,1.5f };
	SparticleEmit.initPositionRandomMargin = randomMargin;

	SparticleEmit.initSpeed = D3DXVECTOR3(2.0f, 0.0f, 0.0f);

		m_particleEmit = std::make_unique<CParticleEmitter>();
		m_particleEmit->Init(SparticleEmit);

	

}

void Sea::Update()
{
	//ライトの回転
	



	//太陽ライトの方向を更新。
	D3DXVECTOR3 sundir = game->GetMap()->GetSun()->GetPosition() * -1.0f;
	D3DXVec3Normalize(&sundir, &sundir);
	light.SetDiffuseLightDirection(0, { sundir.x, sundir.y, sundir.z, 1.0f });

	model.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	//m_particleEmit->SetPosition({ 0.0f,50.0f,0.0f });
	//m_particleEmit->Update();
}

void Sea::Render()
{
	model.SetShadowMap(false);
	model.SetShadowRecieve(true);
	model.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
	//m_particleEmit->Render(game->GetGameCamara()->Getcamera()->GetViewMatrix(), game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
}

void Sea::RadarMapRender(D3DXMATRIX&  RederCameraMatrix, D3DXMATRIX&	RederCameraProjMatrix)
{
	model.SetShadowMap(false);
	model.SetShadowRecieve(true);
	model.Draw(&RederCameraMatrix, &RederCameraProjMatrix);
}