#include "stdafx.h"
#include "Enemy.h"


Enemy::Enemy()
{
	PLight.SetAmbientLight({ 1.0f,1.0f,1.0f,1.0f });
}


Enemy::~Enemy()
{
}

void Enemy::Start()
{

	skinmodelData.LoadModelData("Assets/modelData/Unity.X", &animation);
	skinmodel.Init(&skinmodelData);
	skinmodel.SetLight(&PLight);
	characterController.Init(0.5f, 1.0f, position);
	characterController.SetGravity(-70.0f); //重力の設定
	IsDete = false;

}

void Enemy::Update()
{
	if (pad.IsPress(Pad::enButtonLB1))
	{
		//position = { 0.0f,1.0f,4.0f };
		//characterController.Init(0.5f, 1.0f, position);
		IsDete = false;
	}
	pad.Update();
	if (IsDete) { return; }
	EndEnemy();
	//EnemyMove();
	//D3DXVECTOR3 Movespeed = { 0.0f,0.0f,0.0f };
	//characterController.SetMoveSpeed(Movespeed);
	characterController.Execute();
	position = characterController.GetPosition();
	animation.Update(1.0f / 60.0f);
	skinmodel.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

}
void Enemy::EnemyMove()
{

	position = position - game->GetPlayer()->GetPos();
	//Y軸周りの回転行列を作成。
	D3DXMATRIX mRot;
	//単位行列を作成。
	D3DXMatrixIdentity(&mRot);
	D3DXMatrixRotationY(&mRot, 0.05f*2);
	D3DXQuaternionRotationMatrix(&rotation, &mRot);
	//toEyePosを回す。
	D3DXQuaternionRotationAxis(&rotation, &position, 0.5f);


}

void Enemy::EndEnemy()
{
	auto& Bullets = game->GetPlayerBullet();
	D3DXVECTOR3 Movespeed = { 0.0f,0.0f,0.0f };

	for (auto bullet : Bullets)
	{
		D3DXVECTOR3 DetBullet = position - bullet->Getops();

		float length = D3DXVec3Length(&DetBullet);
		

		if (length < 0.8f)
		{
			IsDete = true;
			//this->characterController.RemoveRigidBoby();
		}

	}

	characterController.SetMoveSpeed(Movespeed);
}

void Enemy::Render()
{
	if (IsDete) { return; }
	camera = game->GetCamera();
	skinmodel.Draw(&camera->GetViewMatrix(), &camera->GetProjectionMatrix());


}