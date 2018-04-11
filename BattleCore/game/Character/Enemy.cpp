#include "stdafx.h"
#include "Enemy.h"


int fg = 10;
SkinModelData*	CEnemy::m_SkinmodelData = nullptr;
SkinModelData*	CEnemy::m_MapSkinmodelData = nullptr;
CEnemy::CEnemy()
{


}
CEnemy::~CEnemy()
{

	m_CharacterController.RemoveRigidBoby();
	m_CharacterController.GetRigidBody()->Release();
	delete m_GUI;
}

void CEnemy::Start()
{
	m_GUI = new CGUI;
	//一本目のライトを太陽からのライトにする。
	m_Light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	m_Light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	m_Light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	m_Light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	m_Light.SetDiffuseLightColor(0, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	m_Light.SetDiffuseLightColor(1, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	m_Light.SetDiffuseLightColor(2, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	m_Light.SetDiffuseLightColor(3, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	m_Light.SetAmbientLight({ 0.2f,0.2f,0.2f,1.0f });
	m_minimaplight.SetAmbientLight({ 1.0f,1.0f,1.0f,1.0f });

	if(m_SkinmodelData == NULL){
		m_SkinmodelData = new SkinModelData;
		m_SkinmodelData->LoadModelData("Assets/modelData/robo4.x", NULL);
	}
	m_Skinmodel.Init(m_SkinmodelData);
	m_Skinmodel.SetLight(&m_Light);
	if (m_MapSkinmodelData == NULL) {
		m_MapSkinmodelData = new SkinModelData;
		m_MapSkinmodelData->LoadModelData("Assets/modelData/MiniMap_Enemy.x", NULL);
	}
	m_MapSkinmodel.Init(m_MapSkinmodelData);
	m_MapSkinmodel.SetLight(&m_minimaplight);


	m_CharacterController.Init(1.0f, 1.0f, m_Position);
	m_CharacterController.SetGravity(-9.8f); //重力の設定
	m_IsDete = false;
	RouteSearch();
	
	m_GUI->Start({ m_HP /100,0.5f }, "Assets/sprite/Enemy_HP.png");
	
	SParicleEmitParameter SparticleEmit;
	//パーティクルの設定
	SparticleEmit.texturePath = "Assets/Particle/P_Fire.png";
	static float w = 2.0f;
	static float h = 2.0f;
	SparticleEmit.w = w;
	SparticleEmit.h = h;
	SparticleEmit.intervalTime = 0.05f;
	SparticleEmit.Endtimer = 20.0f;
	SparticleEmit.brightness = 0.2f;
	SparticleEmit.life = 2.0f;
	static D3DXVECTOR3 randomMargin = { 1.5f,1.5f,1.5f };
	SparticleEmit.initPositionRandomMargin = randomMargin;

	SparticleEmit.initSpeed = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_particleEmit = std::make_unique<CParticleEmitter>();
	m_particleEmit->Init(SparticleEmit);

}

void CEnemy::Update()
{

	switch (m_State)
	{

	case e_EnemyUpdate:

		//太陽ライトの方向を更新。
		D3DXVECTOR3 sundir = game->GetMap()->GetSun()->GetPosition() * -1.0f;
		D3DXVec3Normalize(&sundir, &sundir);
		m_Light.SetDiffuseLightDirection(0, { sundir.x, sundir.y, sundir.z, 1.0f });

		m_GUI->SetPosition({ m_Position.x,m_Position.y + 8,m_Position.z });
		m_GUI->Update();

		m_MoveSpeed = m_CharacterController.GetMoveSpeed();
		EnemyMove();
		EnemyBulletON();
		EndEnemy();
		if (m_HP <= 200.0f)
		{

			m_particleEmit->SetPosition({ m_Position.x,m_Position.y + 2.5f,m_Position.z });
			m_particleEmit->Update();

		}
		m_Position = m_CharacterController.GetPosition();
		break;

	}

}

//敵の弾丸処理
void CEnemy::EnemyBulletON()
{

	
	D3DXMATRIX Ahead = m_Skinmodel.GetMatrix(); //プレイヤーの行列を取得
	D3DXVECTOR3 pPos = game->GetPlayer()->GetPos();
	D3DXVec3Subtract(&pPos, &pPos, &m_Position);
	//プレイヤーを追う弾
	D3DXVECTOR3 Pos = game->GetPlayer()->GetPos() - m_Position;
	//D3DXVec3Normalize(&Pos, &Pos);

	if (D3DXVec3Length(&pPos) <= 200.0f)
	{

		m_BulletTime += GameTime().GetFrameDeltaTime();  //タイマーの所得

		if (!m_rando)
		{
			//乱数によるインターバルの設定
			m_randoTime = g_Random->GetRandInt() % 1 + 3;
			m_rando = true;
		}

		if (m_BulletTime > m_randoTime) {
			Bullet* bullet = new Bullet();
			D3DXVECTOR3 bulletPos = m_Position;
			bulletPos.y += 3.0f;
			bullet->Start(bulletPos, Pos);//プレイヤーの前方向を渡す
			game->AddEnemyBullets(bullet);
			m_BulletTime = 0.0f;
			m_rando = false;


		}


	}
}

//敵の移動関数
void CEnemy::EnemyMove()
{
	

	//if (m_CharacterController.IsJump()) {
	//	
	//	m_CharacterController.SetPosition(m_Position);
	//	m_CharacterController.Execute();
	//	return;
	//
	//}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 SubVect,NewPlayerposition;
	m_SubPosition.y = game->GetPlayer()->GetPos().y;
	//m_SubPosition.y = 10.0f;

	SubVect = m_SubPosition - m_Position;

	//D3DXVec3Normalize(&NewPlayerposition, &NewPlayerposition);
	//敵の移動
	if (D3DXVec3Length(&SubVect) >= 20 )
	{
		D3DXVec3Normalize(&SubVect, &SubVect);
		m_MoveSpeed = SubVect * 20.0f;
		m_CharacterController.Jump();
		m_CharacterController.SetGravity(0.0f);
		m_CharacterController.SetPosition(m_Position);
		m_CharacterController.SetMoveSpeed(m_MoveSpeed);
		m_CharacterController.Execute();
	}
	
	if(D3DXVec3Length(&NewPlayerposition) >= 30)
	{
		RouteSearch();
	}

	if (m_CharacterController.IsHitWall())
	{
		BranchRoute();

	}

	D3DXVECTOR3 Def;
	D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
	D3DXVec3Subtract(&Def, &game->GetPlayer()->GetPos(), &m_Position);
	D3DXQuaternionRotationAxis(&m_Rotation, &UP, atan2f(Def.x, Def.z));
	
}

void CEnemy::EndEnemy()
{
	//プレイヤーの弾を取得
	auto& Bullets = game->GetPlayerBullet();
	D3DXVECTOR3 Movespeed = { 0.0f,0.0f,0.0f };

	if (m_Position.y <= -50)
	{
		m_HP = 0;
	}
	//プレイヤーとの弾の当たり判定の計算
	for (const auto& bullet : Bullets)
	{
		D3DXVECTOR3 DetBullet = m_Position - bullet->Getops();
		DetBullet.y += 8;
		

		float length = D3DXVec3Length(&DetBullet);
		

		if (length < 8.0f && !bullet->GetIsHit())
		{
			m_HP -= 100.0f;
			float PercentHp = (float)100.0f / 400.0f ;
			m_GUI->setSize({ PercentHp,0.5f });
			bullet->SetIsHit(true);
			
		
			
			
		}

	}

	if (m_HP <= 0)
	{

		m_State = e_EnemyDelte;
		m_CharacterController.RemoveRigidBoby();
		game->GetScorecheckre()->SetScore();
		m_IsDete = true;

	}
}

void CEnemy::BranchRoute()
{

	m_SubPosition = m_AdjacentRoute[m_ZNumber][m_XNumber].m_RoutePos;
	m_XNumber++;
	if (m_XNumber == 3)
	{
		m_ZNumber++;
		m_XNumber = 0;
		if (m_ZNumber == 3)
		{
			m_ZNumber = 0;
		}
	}


}

void CEnemy::RouteSearch()
{
	D3DXVECTOR3 TargetPosition = game->GetPlayer()->GetPos();	//敵から見たターゲット
	D3DXVECTOR3 MyPosition = m_Position;						//現在位置
	MyPosition.x += 80.0f;
	MyPosition.z += 80.0f;
	int Numbeer = 1;
	D3DXVECTOR3 Length = {0,10000,0};
	D3DXVECTOR3 SubVect = {0,0,0};



	/*
	エネミーを中心に座標を格納
	□□□
	□□□
	□□□
	プレイヤーに一番近い地点をサブポイントとして移動させる。

	*/
	for (int Route_Z = 0;Route_Z < 3;Route_Z++)
	{

		for (int Route_X = 0;Route_X < 3;Route_X++)
		{
			m_AdjacentRoute[Route_Z][Route_X].m_RoutePos = MyPosition;
			m_AdjacentRoute[Route_Z][Route_X].m_RouteNumber = Numbeer;
			D3DXVec3Subtract(&SubVect,&TargetPosition,& m_AdjacentRoute[Route_Z][Route_X].m_RoutePos);
			if (D3DXVec3Length(&Length) > D3DXVec3Length(&SubVect))
			{
				Length = SubVect;
				m_SubPosition = m_AdjacentRoute[Route_Z][Route_X].m_RoutePos;
				m_AdjacentRoute[Route_Z][Route_X].m_RouteNumber = Numbeer;
				Numbeer += 1;
				//m_XNumber = Route_X;
				//m_ZNumber = Route_Z;
			}

			
			MyPosition.x -= 80.0f;
		}

		MyPosition.x += 240.0f;
		MyPosition.z -= 80.0f;
	}



}

void CEnemy::Render()
{
	switch (m_State)
	{

	case e_EnemyUpdate:

		m_Skinmodel.UpdateWorldMatrix(m_Position, m_Rotation, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
		m_Skinmodel.SetShadowMap(false);
		m_Skinmodel.SetShadowRecieve(false);

		m_Skinmodel.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
		m_particleEmit->Render(game->GetGameCamara()->Getcamera()->GetViewMatrix(),game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
		break;


	}
	

}
void CEnemy::Render2D()
{
	switch (m_State)
	{

	case e_EnemyUpdate:
		m_GUI->Render(game->GetGameCamara()->Getcamera()->GetViewMatrix(), game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
		//m_ParticleEmitter.Render(game->GetCamera()->GetViewMatrix(), game->GetCamera()->GetProjectionMatrix());
		break;

	}
}
void CEnemy::LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix)
{
	switch (m_State)
	{

	case e_EnemyUpdate:
		if (m_IsDete) { return; };
		m_Skinmodel.SetShadowMap(true);
		m_Skinmodel.SetShadowRecieve(false);
		m_Skinmodel.Draw(&lightViewMatrix, &lightProjMatrix);

		break;


	}
}

void CEnemy::MiniMapRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix)
{
	switch (m_State)
	{

	case e_EnemyUpdate:
		if (m_IsDete) { return; };


		m_MapSkinmodel.UpdateWorldMatrix({ m_Position.x ,0.0f,m_Position.z }, { 0.0f,0.0f,0.0f,1.0f }, D3DXVECTOR3(5.0f, 1.0f, 5.0f));
		m_MapSkinmodel.SetShadowMap(false);
		m_MapSkinmodel.SetShadowRecieve(false);

		m_MapSkinmodel.Draw(&lightViewMatrix, &lightProjMatrix);

		break;


	}
}

void CEnemy::SetRandomPosition(D3DXVECTOR3 setpos)
{
	m_Position.x += (((float)g_Random->GetRandDouble() - 0.5f) * 2.0f) * 10;
	m_Position.y += (((float)g_Random->GetRandDouble() - 0.5f) * 2.0f) * 10;
	m_Position.z += (((float)g_Random->GetRandDouble() - 0.5f) * 2.0f) * 10;
}