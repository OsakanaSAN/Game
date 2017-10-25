#include "stdafx.h"
#include "Bullet.h"
#include "game.h"

SkinModelData* Bullet::modelData = NULL;


Bullet::Bullet()
{
	position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	moveSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
}


Bullet::~Bullet()
{
	delete modelData;
	modelData = NULL;
}
void Bullet::Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed)
{
	position = pos;
	D3DXVECTOR3 Nomal = moveSpeed;
	D3DXVec3Normalize(&Nomal,&Nomal);
	
	this->moveSpeed = Nomal * 100.0f;//1.5f; //弾速調整
	//ライトを初期化。
	light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	light.SetDiffuseLightColor(0, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));
	light.SetDiffuseLightColor(3, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	light.SetAmbientLight(D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));

	if (modelData == NULL) {
		//モデルをロード。
		modelData = new SkinModelData;
		modelData->LoadModelData("Assets/modelData/bullet.X", NULL);
	}
	rot = game->GetPlayer()->GetRot(); //プレイヤーの回転
	model.Init(modelData);
	model.SetLight(&light);
	life = 120;
}
bool Bullet::Update()
{
	
	life--;
	if (life < 0 || IsHit) {
		//死亡。
		return false;
	}

	position += moveSpeed;
	
	return true;
}
void Bullet::LockOn(D3DXVECTOR3 lock)
{
	D3DXVECTOR3 old = lock - moveSpeed;
	D3DXVec3Normalize(&old, &old);
	moveSpeed = old *0.8f;

}

void Bullet::Render()
{
	
	model.UpdateWorldMatrix(position, rot, D3DXVECTOR3(50.0f, 50.0f, 50.0f));
	Camera* camera = game->GetCamera();
	model.Draw(&camera->GetViewMatrix(), &camera->GetProjectionMatrix());
}

void Bullet::LightEyePosRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix)
{
	model.SetShadowMap(true);
	model.SetShadowRecieve(false);
	model.Draw(&lightViewMatrix, &lightProjMatrix);

}


