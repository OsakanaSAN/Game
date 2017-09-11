#include "stdafx.h"
#include "Enemy.h"

int fg = 10;
Enemy::Enemy()
{
	PLight.SetAmbientLight({ 1.0f,1.0f,1.0f,1.0f });
}


Enemy::~Enemy()
{
}

void Enemy::Start()
{
	position.x = fg;
	fg += 10;
	skinmodelData.LoadModelData("Assets/modelData/pship.X", NULL);
	skinmodel.Init(&skinmodelData);
	skinmodel.SetLight(&PLight);
	characterController.Init(0.5f, 1.0f, position);
	characterController.SetGravity(-70.0f); //重力の設定
	IsDete = false;

}

void Enemy::Update()
{
	srand((unsigned int)time(NULL));
	

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

	if (D3DXVec3Length(&pPos) < 30 && bulletFireInterval == 0) {
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

//////////////////////////////////////////////////////////////////////////////////////////////////////////
	//嫌な挙動
	//中・近距離
	D3DXVECTOR3 pPos = game->GetPlayer()->GetPos();
	D3DXVec3Subtract(&pPos, &pPos, &position);
	float spdX = rand() % 50 + 40;
	float spdY = rand() % 50 + 20;
	float spdZ = 50;//rand() % 50 + 40;

	if (D3DXVec3Length(&pPos) <= 60 && D3DXVec3Length(&pPos) > 1)
	{
		tactics = 1;
		spdX  /= 100;
		spdY  /= 100;
		spdZ  /= 100;
		pPos /= D3DXVec3Length(&pPos);
		position.x -= pPos.x *spdX;
		position.y -= pPos.y *spdY;
		position.z -= pPos.z *spdZ;
		int nexthp = HP;
	    if (coolTime == 0)
	    {
			tacticsChange = HP;
			TimeVec.x = spdX;
			TimeVec.y = spdY;
			TimeVec.z = spdZ;
		    coolTime = 180;

	    }

		
		if (coolTime > 0 )
		{
				coolTime--;
				position.x += pPos.x *TimeVec.x;
				position.y += pPos.y *TimeVec.y;
				position.z += pPos.z *TimeVec.z;
			
		}
		
		
		D3DXVECTOR3 Def;
		D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
		D3DXVec3Subtract(&Def, &game->GetPlayer()->GetPos(), &position);
		D3DXQuaternionRotationAxis(&rotation, &UP, atan2f(Def.x, Def.z));

	}


////////////////////////////////////////////////////////////////////////////////////////////////
	//遠距離の挙動
	//早すぎる
	if (D3DXVec3Length(&pPos) <= 100&& D3DXVec3Length(&pPos) > 60)
	{
		tactics = 2;
		spdX /= 50;
		spdY /= 50;
		spdZ /= 50;

		pPos /= D3DXVec3Length(&pPos);
		position.x += pPos.x *spdX;
		position.y += pPos.y *spdY;
		position.z += pPos.z *spdZ;


		if (coolTime == 0)
		{
			TimeVec.x = spdX;
			TimeVec.y = spdY;
			TimeVec.z = spdZ;
			coolTime = 120;

		}

		if (coolTime > 0)
		{
			position.x += pPos.x *TimeVec.x;
			position.y += pPos.y *TimeVec.y;
			position.z += pPos.z *TimeVec.z;

		}



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
		

		if (length < 1.0f)
		{
			HP -= 5;
			//bullet->SetIsHit(true);
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