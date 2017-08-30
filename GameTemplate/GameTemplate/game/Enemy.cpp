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
		HP = 200;
		IsDete = false;
	}
	pad.Update();
	if (IsDete) { return; }
	EndEnemy();
	EnemyMove();
	EnemyBulletON();
	characterController.Execute();
	//position = characterController.GetPosition();
	animation.Update(1.0f / 60.0f);
	skinmodel.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

}

//敵の弾丸処理
void Enemy::EnemyBulletON()
{
	D3DXMATRIX Ahead = skinmodel.GetMatrix(); //プレイヤーの行列を取得
	D3DXVECTOR3 pPos = game->GetPlayer()->GetPos();
	D3DXVec3Subtract(&pPos, &pPos, &position);
	//プレイヤーを追う弾
	D3DXVECTOR3 Pos = game->GetPlayer()->GetPos() - position;
	D3DXVec3Normalize(&Pos, &Pos);

	if (D3DXVec3Length(&pPos) < 100 && bulletFireInterval == 0) {
		Bullet* bullet = new Bullet();
		D3DXVECTOR3 bulletPos = position;
		bulletPos.y += 0.5f;
		bullet->Start(bulletPos, Pos);//プレイヤーの前方向を渡す
		game->AddEnemyBullets(bullet);
		bulletFireInterval = 20 * tactics; //秒間3発
	}
	bulletFireInterval--;
	if (bulletFireInterval < 0) {
		bulletFireInterval = 0;
	}

}

void Enemy::EnemyMove()
{

	D3DXVECTOR3 pPos = game->GetPlayer()->GetPos();
	D3DXVec3Subtract(&pPos, &pPos, &position);

	if (D3DXVec3Length(&pPos) <= 50 && D3DXVec3Length(&pPos) > 1)
	{
		tactics = 1;
		pPos /= D3DXVec3Length(&pPos);
		position.x += pPos.x *0.2f;
		position.y += pPos.y *0.2f;
		position.z += pPos.z *0.2f;
		D3DXVECTOR3 Def;
		D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
		D3DXVec3Subtract(&Def, &game->GetPlayer()->GetPos(), &position);
		D3DXQuaternionRotationAxis(&rotation, &UP, atan2f(Def.x, Def.z));

	}

	if (D3DXVec3Length(&pPos) <= 100&& D3DXVec3Length(&pPos) > 50)
	{
		tactics = 2;
		pPos /= D3DXVec3Length(&pPos);
		position.x += pPos.x *0.5f;
		position.y += pPos.y *0.5f;
		position.z += pPos.z *0.5f;
		D3DXVECTOR3 Def;
		D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
		D3DXVec3Subtract(&Def, &game->GetPlayer()->GetPos(), &position);
		D3DXQuaternionRotationAxis(&rotation, &UP, atan2f(Def.x, Def.z));
	}
	

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
			HP -= 5;
			bullet->SetIsHit(true);
			if (HP < 0)
			{
				IsDete = true;

			}
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