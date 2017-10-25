#include "stdafx.h"
#include "Enemy.h"

int fg = 10;
Enemy::Enemy()
{


}
Enemy::~Enemy()
{

}

void Enemy::Start(D3DXVECTOR3 pos)
{
	
	position = pos;
	
	PLight.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	PLight.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	PLight.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	PLight.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	PLight.SetDiffuseLightColor(0, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	PLight.SetDiffuseLightColor(1, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	PLight.SetDiffuseLightColor(2, D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));
	PLight.SetDiffuseLightColor(3, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	PLight.SetAmbientLight({ 0.3f,0.3f,0.3f,0.3f });

	skinmodelData.LoadModelData("Assets/modelData/robo1.x", NULL);
	skinmodel.Init(&skinmodelData);
	skinmodel.SetLight(&PLight);
	characterController.Init(10.0f, 5.0f, position);
	characterController.SetGravity(-20.8f); //重力の設定
	IsDete = false;

}

void Enemy::Update()
{

	pad.Update();
	game->GetPlayer()->SetEnemypos(position);
	if (IsDete) { return; }
	EndEnemy();
	
	//EnemyMove();
	
	MoveSpeed = characterController.GetMoveSpeed();
	
	
	characterController.SetMoveSpeed(MoveSpeed);
	characterController.Execute();

	position = characterController.GetPosition();
	skinmodel.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(4.0f, 4.0f, 4.0f));

}

//敵の弾丸処理
void Enemy::EnemyBulletON()
{

	
	//D3DXMATRIX Ahead = skinmodel.GetMatrix(); //プレイヤーの行列を取得
	//D3DXVECTOR3 pPos = game->GetPlayer()->GetPos();
	//D3DXVec3Subtract(&pPos, &pPos, &position);
	////プレイヤーを追う弾
	//D3DXVECTOR3 Pos = game->GetPlayer()->GetPos() - position;
	//D3DXVec3Normalize(&Pos, &Pos);

	//if (D3DXVec3Length(&pPos) < 30 && bulletFireInterval == 0) {
	//	Bullet* bullet = new Bullet();
	//	D3DXVECTOR3 bulletPos = position;
	//	bulletPos.y += 0.5f;
	//	bullet->Start(bulletPos, Pos);//プレイヤーの前方向を渡す
	//	game->AddEnemyBullets(bullet);
	//	bulletFireInterval = 20 * tactics; //秒間3発
	//}
	//bulletFireInterval--;
	//if (bulletFireInterval < 0) {
	//	bulletFireInterval = 0;
	//}

}

void Enemy::EnemyMove()
{

	if (position.x < 1200 && !MoveX)
	{
		position.x += 1.0f;

	}

	else if (position.x > 1200)
	{
		position.x -= 1.0f;
		MoveX = true;
	}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
	////嫌な挙動
	////中・近距離
	//D3DXVECTOR3 pPos = game->GetPlayer()->GetPos();
	//D3DXVec3Subtract(&pPos, &pPos, &position);

	//if (D3DXVec3Length(&pPos) < 1000)
	//{
	//	
	//	position.x = pPos.x *0.05f;
	//	position.z = pPos.z *0.05f;

	//	D3DXVECTOR3 Def;
	//	D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
	//	D3DXVec3Subtract(&Def, &game->GetPlayer()->GetPos(), &position);
	//	D3DXQuaternionRotationAxis(&rotation, &UP, atan2f(Def.x, Def.z));
	//	characterController.SetMoveSpeed(position);
	//	characterController.Execute();

	//}




}

void Enemy::EndEnemy()
{
	auto& Bullets = game->GetPlayerBullet();
	D3DXVECTOR3 Movespeed = { 0.0f,0.0f,0.0f };

	for (auto bullet : Bullets)
	{
		D3DXVECTOR3 DetBullet = position - bullet->Getops();
		DetBullet.y += 8;

		float length = D3DXVec3Length(&DetBullet);
		

		if (length < 100.0f)
		{
			HP -= 10;
			bullet->SetIsHit(true);
			if (HP < 0)
			{
				game->GetPlayer()->EnemyDet();
				IsDete = true;

			}
			//this->characterController.RemoveRigidBoby();
		}

	}

	//characterController.SetMoveSpeed(Movespeed);
}

void Enemy::Render()
{
	if (IsDete) { return; }
	skinmodel.SetShadowMap(false);
	skinmodel.SetShadowRecieve(false);
	/*D3DXMATRIX SVM = g_shadowMap.GetLightViewMatrix();
	D3DXMATRIX SPM = g_shadowMap.GetLightProjectionMatrix();
	skinmodel.Draw(&SVM, &SPM);*/
	skinmodel.Draw(&game->GetCamera()->GetViewMatrix(),&game->GetCamera()->GetProjectionMatrix());


}
void Enemy::LightEyePosRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix)
{
	skinmodel.SetShadowMap(true);
	skinmodel.SetShadowRecieve(false);
	skinmodel.Draw(&lightViewMatrix, &lightProjMatrix);

}