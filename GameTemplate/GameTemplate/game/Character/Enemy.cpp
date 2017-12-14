#include "stdafx.h"
#include "Enemy.h"

int fg = 10;
CEnemy::CEnemy()
{


}
CEnemy::~CEnemy()
{

}

void CEnemy::Start()
{
	m_Light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	m_Light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	m_Light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	m_Light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	m_Light.SetDiffuseLightColor(0, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	m_Light.SetDiffuseLightColor(1, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	m_Light.SetDiffuseLightColor(2, D3DXVECTOR4(0.3f, 0.3f, 0.3f, 1.0f));
	m_Light.SetDiffuseLightColor(3, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	m_Light.SetAmbientLight({ 0.3f,0.3f,0.3f,0.3f });

	m_SkinmodelData.LoadModelData("Assets/modelData/robo4.x", NULL);
	m_Skinmodel.Init(&m_SkinmodelData);
	m_Skinmodel.SetLight(&m_Light);
	m_CharacterController.Init(1.0f, 1.0f, m_Position);
	m_CharacterController.SetGravity(-9.8f); //重力の設定
	m_IsDete = false;

}

void CEnemy::Update()
{
	m_MoveSpeed = m_CharacterController.GetMoveSpeed();
	EnemyMove();
	//EnemyBulletON();
	EndEnemy();
	

	m_Position = m_CharacterController.GetPosition();
	m_Skinmodel.UpdateWorldMatrix(m_Position, m_Rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));

}

//敵の弾丸処理
void CEnemy::EnemyBulletON()
{

	
	D3DXMATRIX Ahead = m_Skinmodel.GetMatrix(); //プレイヤーの行列を取得
	D3DXVECTOR3 pPos = game->GetPlayer()->GetPos();
	D3DXVec3Subtract(&pPos, &pPos, &m_Position);
	//プレイヤーを追う弾
	D3DXVECTOR3 Pos = game->GetPlayer()->GetPos() - m_Position;
	D3DXVec3Normalize(&Pos, &Pos);

	m_BulletTime += GameTime().GetFrameDeltaTime();  //60分の1秒
	if (D3DXVec3Length(&pPos) < 200 && m_BulletTime > 2.0f) {
		Bullet* bullet = new Bullet();
		D3DXVECTOR3 bulletPos = m_Position;
		bulletPos.y += 1.0f;
		bullet->Start(bulletPos, Pos);//プレイヤーの前方向を渡す
		game->AddEnemyBullets(bullet);
		//bulletFireInterval = 20 ; //秒間3発
		m_BulletTime = 0.0f;
	}

	m_BulletFireInterval--;
	if (m_BulletFireInterval < 0) {
		m_BulletFireInterval = 0;
	}

}

//敵の移動関数
void CEnemy::EnemyMove()
{
	if (m_CharacterController.IsJump()) {
		
		m_CharacterController.SetPosition(m_Position);
		m_CharacterController.Execute();
		return;
	
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 pPos = game->GetPlayer()->GetPos(); //プレイヤーの現在地
	int pSpeeed = game->GetPlayer()->GetMoveSpeed();//プレイヤの移動速度

	D3DXVec3Subtract(&pPos, &pPos, &m_Position);

	//敵の移動
	if (D3DXVec3Length(&pPos) > 1)
	{
		
		m_MoveSpeed.x = pPos.x * 0.01f;
		m_MoveSpeed.z = pPos.z * 0.01f;
		//MoveSpeed.y = pPos.y * 0.5f;

		D3DXVECTOR3 Def;
		D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
		D3DXVec3Subtract(&Def, &pPos, &m_MoveSpeed);
		D3DXQuaternionRotationAxis(&m_Rotation, &UP, atan2f(Def.x, Def.z));
	}
	m_CharacterController.SetPosition(m_Position);
	m_CharacterController.SetMoveSpeed(m_MoveSpeed);
	m_CharacterController.Execute();
	
}

void CEnemy::EndEnemy()
{
	//プレイヤーの弾を取得
	auto& Bullets = game->GetPlayerBullet();
	D3DXVECTOR3 Movespeed = { 0.0f,0.0f,0.0f };

	//プレイヤーとの弾の当たり判定の計算
	for (auto bullet : Bullets)
	{
		D3DXVECTOR3 DetBullet = m_Position - bullet->Getops();
		DetBullet.y += 8;

		float length = D3DXVec3Length(&DetBullet);
		

		if (length < 10.0f)
		{
			m_HP -= 10;
			bullet->SetIsHit(true);
			if (m_HP < 0)
			{
				
				m_IsDete = true;

			}
			//this->characterController.RemoveRigidBoby();
		}

	}

	//characterController.SetMoveSpeed(Movespeed);
}

void CEnemy::Render()
{
	if (m_IsDete) { return; };
	m_Skinmodel.SetShadowMap(false);
	m_Skinmodel.SetShadowRecieve(false);
	m_Skinmodel.Draw(&game->GetCamera()->GetViewMatrix(),&game->GetCamera()->GetProjectionMatrix());


}
void CEnemy::LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix)
{
	if (m_IsDete) { return; };
	m_Skinmodel.SetShadowMap(true);
	m_Skinmodel.SetShadowRecieve(false);
	m_Skinmodel.Draw(&lightViewMatrix,&lightProjMatrix);

}