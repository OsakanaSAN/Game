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

	

}

void Sea::Update()
{
	//ライトの回転
	float angle = 0.0f;
	if (GetAsyncKeyState('Q')) {
		angle = D3DXToRadian(1.0f);
	}
	else if (GetAsyncKeyState('E')) {
		angle = D3DXToRadian(-1.0f);
	}
	//ライトを回転させる行列を作成する。
	D3DXMATRIX mRot;
	D3DXVECTOR3 UP = { 0.0,1.0,0.0 };
	D3DXMatrixRotationAxis(&mRot, &UP, angle);
	//回転させる。
	for (int i = 0; i < 4; i++) {
		D3DXVECTOR4 dir;
		D3DXVec4Transform(&dir, &light.GetDiffuseLightDirection()[i], &mRot);
		light.SetDiffuseLightDirection(i, dir);
	}

	model.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

}

void Sea::Render()
{
	model.SetShadowMap(false);
	model.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
}