#include "stdafx.h"
#include "Player.h"
#include "game.h"
#define  TURBOTIME 5
#define  NomalSpeed 30
#define	 BULLETTIME 0.1f


enum
{
	Run_anim,		//ブースト
	Stand_anim,		//待機中
	Battle_anim,	//攻撃時
	walk_anim,		//歩行

};

CPlayer::CPlayer()
{
	
}


CPlayer::~CPlayer()
{
	m_PlayerSE->Release();
	delete m_PlayerSE;

	m_CharacterController.RemoveRigidBoby();
	m_SkinmodelData.Release();

}
void CPlayer::Start()
{
	m_rederMap = new CRadarMap;
	m_rederMap->InitRender();
	m_TagetUI = new CGUI;
	m_TagetUI->Start({ 10.0f,10.0f }, "Assets/sprite/HUD2.png");
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
	m_SkinmodelData.LoadModelData("Assets/modelData/robo7.X",&m_Animation);
	m_Skinmodel.Init(&m_SkinmodelData);
	m_Skinmodel.SetLight(&m_Light);
	//ミニマップのモデルの初期化
	m_MapSkinmodelData.LoadModelData("Assets/modelData/MiniMap_Player.X",NULL);
	m_MapSkinmodel.Init(&m_MapSkinmodelData);
	m_MapSkinmodel.SetLight(&m_Light);

	
	//アニメーションの初期化
	//m_Animation.Init();
	m_Animation.PlayAnimation(0);
	m_Animation.SetAnimetionSpeed(1.0f);
	m_Animation.SetAnimetionLoopFlag(0, false);
	m_Animation.SetAnimetionLoopFlag(1, false);
	m_Animation.SetAnimetionLoopFlag(2, false);

	
	/*
	animation.SetAnimetionLoopFlag(Run_anim, false);
	animation.SetAnimationEndTime(Stand_anim, -1.0f);
	animation.SetAnimationEndTime(walk_anim, 0.54f);
	animation.SetAnimetionLoopFlag(Stand_anim,true);
	animation.SetAnimetionLoopFlag(Battle_anim,false);*/
	
	m_CharacterController.Init(1.0f, 1.0f, m_Position);
	m_CharacterController.SetGravity(-20.0f); //重力の設定

	m_PlayerSE = new CSoundSource;
	m_PlayerSE->Init("Assets/Sound/SE/LoockOnSound.wav");
	m_EnemyNo = 0;
	m_PlayerBoothtSE = new CSoundSource;
	m_PlayerBoothtSE->Init("Assets/Sound/SE/BoothtSE1.wav");
	m_PlayerBoothtSE->SetVolume(1.0f);
	

	m_SparticleEmit.texturePath = "Assets/Particle/P_Fire.png";
	m_SparticleEmit.w = 1.5f;
	m_SparticleEmit.h = 2.5f;
	m_SparticleEmit.intervalTime = 0.01f;
	m_SparticleEmit.Endtimer = 100.0f;
	m_SparticleEmit.brightness = 0.2f;
	m_SparticleEmit.life = 5.0f;
	m_SparticleEmit.initSpeed = D3DXVECTOR3(0.0f, 2.0f, 0.0f);
	static D3DXVECTOR3 randomMargin = { 1.5f,1.5f,1.5f };
	m_SparticleEmit.initPositionRandomMargin = randomMargin;
	m_ParticleEmitter.Init(m_SparticleEmit);
	//m_ParticleEmitter.SetPosition({ m_Position.x,m_Position.y + 5.0f,m_Position.z });




	m_lockTarget = new CEnemy;

}


void CPlayer::Update()
{
	m_rederMap->Update(m_Position);
	//太陽ライトの方向を更新。
	D3DXVECTOR3 sundir = game->GetMap()->GetSun()->GetPosition() * -1.0f;
	D3DXVec3Normalize(&sundir, &sundir);
	m_Light.SetDiffuseLightDirection(0, { sundir.x, sundir.y, sundir.z, 1.0f });
	Animetion();  //アニメーション
	MovePlayer();//プレイヤーの移動
	OnLock(); //ロックオン
	
	m_Pad.Update(); //パッドのアプデ

	//m_ParticleEmitter.SetPosition({ m_Position.x,m_Position.y + 5.0f,m_Position.z });
	//m_ParticleEmitter.Update();
	

	for (auto EnemyBullt : game->GetEnemyBullet())
	{
		D3DXVECTOR3 DetBullet = m_Position - EnemyBullt->Getops();
		float length = D3DXVec3Length(&DetBullet);


		if (length < 5.0f && !EnemyBullt->GetIsHit())
		{
			m_Hp = m_Hp - 10;
			EnemyBullt->SetIsHit(true);
		}
	}
	/*D3DXMATRIX Ahead = m_Skinmodel.GetMatrix();
	D3DXVECTOR3 rot = {- Ahead.m[2][0],-Ahead.m[2][1],-Ahead.m[2][2] };
	D3DXVECTOR3 particlepos = m_Position + rot;
	*/
	//m_ParticleEmitter.SetPosition({ m_Position.x,m_Position.y + 5.0f,m_Position.z });
	m_Skinmodel.UpdateWorldMatrix(m_Position, m_Rotation, D3DXVECTOR3(2.0f, 2.0f, 2.0f));
	m_MapSkinmodel.UpdateWorldMatrix({ m_Position.x,0.0f,m_Position.z }, { 0.0f,0.0f,0.0f,1.0f }, D3DXVECTOR3(5.0f, 1.0f, 5.0f));

}

//ロックオン
void CPlayer::OnLock()
{
	if (m_NoEenemy) { return; }
	
	if (m_Pad.IsTrigger(Pad::enButtonLB1))
	{
		if (!m_ZAttent)
		{
			m_PlayerSE->Init("Assets/Sound/SE/LoockOnSound.wav");
			m_PlayerSE->SetVolume(0.1f);
			m_PlayerSE->Play(true);
			m_PlayerSE->Update();
			m_ZAttent = true;
			
		}
		else {
			
			m_ZAttent = false;
			m_siyakakuON = false;
			game->GetGameCamara()->LookOnCamera(m_Enemypos, false);
			
		}
		
	}

	D3DXMATRIX rot = game->GetGameCamara()->GetSpringGamera()->GetCamera()->GetViewMatrix();
	D3DXVECTOR3 Length = { 10000,10000,10000 };
	D3DXVECTOR3 Lenght2;
	bool        LockEnemy = false;
	int			Box = 0;				//敵の数を入れる  
	float		angle = 0.0f;

	//距離が一番近い敵にロックオンをする
	if (game->GetMap()->GetnomalEnemyCount() == 0)
	{
		for (const auto& enemy : game->GetMap()->GetGroupenemy())
		{
			//敵が死んでいるか確認する
			if (enemy->IsEnd()) 
			{
				Box++;
				m_MaxEnemy = Box;
			}

			else
			{
				//カメラの前方向を取得
				m_FrontPosition = game->GetGameCamara()->GetSpringGamera()->GetCamera()->GetFront();;
				//PlayerからEnemyに向かうベクトルを計算する。
				D3DXVECTOR3 toPlayer = enemy->Getpos() - m_Position;
				//正規化
				D3DXVec3Normalize(&toPlayer, &toPlayer);
				D3DXVec3Normalize(&m_FrontPosition, &m_FrontPosition);

				angle = D3DXVec3Dot(&toPlayer, &m_FrontPosition);
				angle = acos(angle);

				//敵かプレイヤーに向かうベクトルを取得
				Lenght2 = m_Position - enemy->Getpos();

				if (fabsf(angle) < D3DXToRadian(60.0f) && m_ZAttent && D3DXVec3Length(&Length) > D3DXVec3Length(&Lenght2))
				{

					LockEnemy = true;
					m_EnemyBox[Box].Position = enemy->Getpos();
					Length = m_Position - m_EnemyBox[Box].Position;
					m_EnemyBox[Box].Number = Box;
					m_EnemyNo = Box;

				}


				Box++;
				m_MaxEnemy = Box;
			}
		}
	}
	else
	{
		for (const auto& enemy : game->GetEnemys())
		{

			//敵が死んでいるか確認する
			if (enemy->IsEnd())
			{
				Box++;
				m_MaxEnemy = Box;
			}

			else
			{
				//カメラの前方向
				m_FrontPosition = game->GetGameCamara()->GetSpringGamera()->GetCamera()->GetFront();
				//PlayerからEnemyに向かうベクトルを計算する。
				D3DXVECTOR3 toPlayer = enemy->Getpos() - m_Position;
				//正規化
				D3DXVec3Normalize(&toPlayer, &toPlayer);
				D3DXVec3Normalize(&m_FrontPosition, &m_FrontPosition);

				angle = D3DXVec3Dot(&toPlayer, &m_FrontPosition);
				angle = acos(angle);

				//敵かプレイヤーに向かうベクトルを取得
				Lenght2 = m_Position - enemy->Getpos();



				if (fabsf(angle) < D3DXToRadian(60.0f) && m_ZAttent && D3DXVec3Length(&Length) > D3DXVec3Length(&Lenght2))
				{

					LockEnemy = true;
					m_lockTarget = enemy;
					m_EnemyBox[Box].Position = enemy->Getpos();
					Length = m_Position - m_EnemyBox[Box].Position;
					m_EnemyBox[Box].Number = Box;
					m_EnemyNo = Box;

				}


				Box++;
				m_MaxEnemy = Box;
			}
		}
	}
	
	
	if (m_Pad.IsTrigger(Pad::enButtonUp) && m_EnemyNo < m_MaxEnemy-1)
	{

		m_EnemyNo += 1;

	}

	else if (m_Pad.IsTrigger(Pad::enButtonDown) && m_EnemyNo > 0)
	{

		m_EnemyNo -= 1;

	}

	if (!LockEnemy)
	{
		m_ZAttent = false;
		Box = 0;
	}


	if (m_ZAttent) {

		D3DXVECTOR3 Def;
		D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
		D3DXVECTOR3 topos = m_EnemyBox[m_EnemyNo].Position - m_Position;
		D3DXVec3Normalize(&topos, &topos);
		D3DXQUATERNION mul;
		D3DXVECTOR3 rotAxis;
		D3DXVec3Cross(&rotAxis, &UP, &topos);

		D3DXVec3Subtract(&Def, &m_EnemyBox[m_EnemyNo].Position, &m_Position);
		D3DXQuaternionRotationAxis(&m_Rotation, &UP, atan2f(Def.x, Def.z));
		D3DXQuaternionRotationAxis(&mul, &-rotAxis, topos.y);
		D3DXQuaternionMultiply(&m_Rotation, &m_Rotation, &mul);
		game->GetGameCamara()->LookOnCamera(m_EnemyBox[m_EnemyNo].Position,true);
		m_TagetUI->SetPosition({m_EnemyBox[m_EnemyNo].Position.x, m_EnemyBox[m_EnemyNo].Position.y + 2.0f, m_EnemyBox[m_EnemyNo].Position.z});
		m_TagetUI->Update();
		game->GetHud()->SetNoRenderCircle(true);
		m_siyakakuON = true;

	}
	else{
		game->GetGameCamara()->LookOnCamera({ m_EnemyBox[m_EnemyNo].Position }, false);
		game->GetHud()->SetNoRenderCircle(false);
		return;

	}

}


void CPlayer::MovePlayer()
{

	///////////////////////////////////////////////////////////////////////////////////
		//キャラの向きと移動速度の計算

	D3DXVECTOR3 AnglemoveSpeed = m_CharacterController.GetMoveSpeed();
	//AnglemoveSpeed.y = 0;
	D3DXVECTOR3 moveDirLocal;
	moveDirLocal.y = 1.0f;
	moveDirLocal.x = m_Pad.GetLStickXF();
	moveDirLocal.z = m_Pad.GetLStickYF();
	D3DXMATRIX mViveInv = game->GetGameCamara()->Getcamera()->GetViewMatrixInv();

	D3DXVECTOR3 cameraZ;
	cameraZ.x = mViveInv.m[2][0];
	cameraZ.y = 0.0f;
	cameraZ.z = mViveInv.m[2][2];
	D3DXVec3Normalize(&cameraZ, &cameraZ);

	D3DXVECTOR3 cameraX;
	cameraX.x = mViveInv.m[0][0];
	cameraX.y = 0.0f;
	cameraX.z = mViveInv.m[0][2];
	D3DXVec3Normalize(&cameraX, &cameraX);

	D3DXVECTOR3 moveDir;

	moveDir.x = cameraX.x * moveDirLocal.x + cameraZ.x * moveDirLocal.z;
	moveDir.z = cameraX.z * moveDirLocal.x + cameraZ.z * moveDirLocal.z;



/////////////////////////////////////////////////////////////////////////////////////////

	//上昇下降の制御部分
	{

		if (m_Pad.IsPress(Pad::enButtonY) && AnglemoveSpeed.y <= 30.0f)
		{

			AnglemoveSpeed.y += 2.0f;
			m_CharacterController.Jump();

		}
		//下降
		else if (m_Pad.IsPress(Pad::enButtonA) && AnglemoveSpeed.y >= -30.0f)
		{

			AnglemoveSpeed.y -= 4.0f;
			m_CharacterController.Jump();
		}
		else if (AnglemoveSpeed.y > 0)
		{
			AnglemoveSpeed.y -= 1.0;
		}
		else if (AnglemoveSpeed.y < 0)
		{
			AnglemoveSpeed.y += 1.0;
		}



		if (AnglemoveSpeed.y <= -30.0f)
		{
			AnglemoveSpeed.y = -30.0f;
		}
		else if (AnglemoveSpeed.y >= 30.0f)
		{
			AnglemoveSpeed.y = 30.0f;
		}

		if (m_Position.y > 120)
		{
			AnglemoveSpeed.y = -1.0f;


		}
	}

	//移動量の計算
	if (m_Pad.GetLStickXF() != 0 && m_Pad.GetLStickYF() != 0  && !m_Oveline)
	{
		//浮いている時の移動方法
		if (!m_CharacterController.IsOnGround() && m_Inertia < NomalSpeed)
		{

			m_Inertia += 1.5f;

		}

		if (m_X_input != m_Pad.GetLStickXF())
		{

			m_X_input = m_Pad.GetLStickXF();
			m_Y_input = m_Pad.GetLStickYF();
		}
		
		m_Oveline = false;
	}

	else if (m_Pad.GetLStickXF() == 0 && m_Pad.GetLStickYF() == 0 && m_Inertia > 0)
	{
		m_Oveline = true;


	}

	//パットの入力が無ければ慣性を徐々に弱める
	if (m_Oveline)
	{
		m_X_input_old = m_X_input;
		m_Y_input_old = m_Y_input;
		m_Inertia -= 3.5f;
			if (m_Pad.IsPress(Pad::enButtonA))
			{
				m_Inertia -= 15.0f;
			}
			if (m_Inertia < 1)
			{
				m_Inertia = 0;
				m_Oveline = false;
			}
			moveDir.x = cameraX.x * m_X_input + cameraZ.x * m_Y_input;
			moveDir.y = 0.0f;
			moveDir.z = cameraX.z * m_X_input + cameraZ.z * m_Y_input;

	}
	


///////////////////////////////////////////////////////////////////////////////////

	AnglemoveSpeed = InitBootht(AnglemoveSpeed); //ブーストの処理

	
////////////////////////////////////////////////////////////////////////////////////////////

	InitBullet(); //プレイヤーの弾の処理

	///////////////////////////////////////////////////////////////////////////////////

	if (AnglemoveSpeed == 0)
	{
		m_AnimationNo = Stand_anim;
	}
	
	if (m_CharacterController.IsOnGround())
	{
		m_CharacterController.SetGravity(0.0f);
	}

	//浮いている時の移動量
	if (!m_CharacterController.IsOnGround())
	{
		AnglemoveSpeed.x = moveDir.x * m_Inertia;
		AnglemoveSpeed.z = moveDir.z * m_Inertia;
	}

	//地面についている時の移動量
	else
	{
		AnglemoveSpeed.x = moveDir.x * 20;
		AnglemoveSpeed.z = moveDir.z * 20 ;
	}

	m_CharacterController.SetMoveSpeed(AnglemoveSpeed);
	m_CharacterController.SetPosition(m_Position);
	m_CharacterController.Execute();
	m_Position = m_CharacterController.GetPosition();
	

	///////////////////////////////////////////////////////////////////////////////////
	

	TransformAngle();//キャラの向きを変える処理
	
}

//ブーストの処理
const D3DXVECTOR3& CPlayer::InitBootht(D3DXVECTOR3& MoveSpeed)
{
	//ダッシュできる時間の計算
	if (m_Pad.IsPress(Pad::enButtonRB2) && m_DashTime < TURBOTIME && !m_NoBoothtInput)
	{
		m_PlayerBoothtSE->Play(true);
		//ダッシュできる時間を格納
		m_DashTime += GameTime().GetFrameDeltaTime();
		m_PlayerBoothtSE->Update();

		//パッドの入力が有ればダッシュする。
		if (m_Pad.GetLStickXF() != 0 && m_Pad.GetLStickYF() != 0)
		{
			m_Inertia = NomalSpeed;
			m_Inertia *= 3.5;
			m_ZoomBlurOn = true;
			m_CharacterController.SetGravity(0.0f);
			
			
			

		}
		
		if (m_DashTime == TURBOTIME)
		{
			m_NoBoothtInput = true;
			m_ZoomBlurOn = false;
		}


	}

	//R2が押されていなけば緩める
	else if (m_DashTime > 0)
	{
		if (m_PlayerBoothtSE->GetVolume() <= 0.0f)
		{
			m_PlayerBoothtSE->SetVolume(1.0f);
			m_PlayerBoothtSE->Stop();
		}
		m_PlayerBoothtSE->SetVolumeDown(GameTime().GetFrameDeltaTime() * 2);
		m_CharacterController.SetGravity(0.0f);
		m_ZoomBlurOn = false;
		m_DashTime -= GameTime().GetFrameDeltaTime();
		if (m_DashTime < 0)
		{
			m_DashTime = 0;

		}

		if (m_Inertia > NomalSpeed)
		{
			
			m_Inertia = NomalSpeed;
		}
	}

	//再ブースト解除
	if (!m_Pad.IsPress(Pad::enButtonRB2) && m_NoBoothtInput)
	{
		m_NoBoothtInput = false;
		m_Inertia = NomalSpeed;
		/*MoveSpeed.x = 0;
		MoveSpeed.z = 0;*/

	}


	if (m_DashTime >= TURBOTIME)
	{
		m_Inertia = NomalSpeed;
		m_ZoomBlurOn = false;
		//characterController.SetGravity(-9.8f); //重力の設定
	}

	m_MoveSpeed = m_Inertia;

	return MoveSpeed;
}

//プレイヤーの向き計算
void CPlayer::TransformAngle()
{
	if (m_ZAttent) { return; }
	D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };

	D3DXVECTOR3 topos = game->GetGameCamara()->Getcamera()->GetLookatPt() - game->GetGameCamara()->Getcamera()->GetEyePt();
	D3DXVec3Normalize(&topos, &topos);
	D3DXQUATERNION mul , rot;
	D3DXVECTOR3 rotAxis;
	D3DXVec3Cross(&rotAxis, &UP, &topos);
	D3DXMATRIX Ahead = m_Skinmodel.GetMatrix(); //プレイヤーの行列を取得
	
	if (m_Pad.IsPress(Pad::enButtonRB2) && m_Pad.GetLStickYF() != 0)
	{
		m_Radian = 30.0f;
		D3DXQuaternionRotationAxis(&m_Rotation, &UP, atan2(topos.x, topos.z));
		D3DXQuaternionRotationAxis(&mul, &-rotAxis, topos.y);
		D3DXQuaternionRotationAxis(&rot, &D3DXVECTOR3{ Ahead.m[0][0],Ahead.m[0][1],Ahead.m[0][2] }, D3DXToRadian(m_Radian * m_Pad.GetLStickYF()));
		D3DXQuaternionMultiply(&m_Rotation, &m_Rotation, &mul);
		D3DXQuaternionMultiply(&m_Rotation, &m_Rotation, &rot);

	}
	else
	{
		D3DXQuaternionRotationAxis(&m_Rotation, &UP, atan2(topos.x, topos.z));
		D3DXQuaternionRotationAxis(&mul, &-rotAxis, topos.y);
		D3DXQuaternionMultiply(&m_Rotation, &m_Rotation, &mul);
		if (m_Radian <= 0) { return; }
		m_Radian -= 1.0f;
		D3DXQuaternionRotationAxis(&rot, &D3DXVECTOR3{ Ahead.m[0][0],Ahead.m[0][1],Ahead.m[0][2] }, D3DXToRadian(m_Radian));
	
	}

	

}


//プレイヤーの弾の処理
void CPlayer::InitBullet()
{
	
	//リロードの処理
	if (m_IsReload)
	{
		m_bulletreload -= GameTime().GetFrameDeltaTime();
		if (m_bulletreload <= 0.0f)
		{
			m_reloadCount = 0;
			m_IsReload = false;
		}

	}

	if (m_Pad.IsPress(Pad::enButtonRB1) && m_bulletIntervalTime >= BULLETTIME &&m_bulletreload <= 0.0f) 
	{

		bool Isfree = false;	//インスタンを新たに生成するかの判定
		
								//弾丸を作成する
		if (m_reloadCount == 30)
		{
			m_bulletreload = 3.0f;
			m_IsReload = true;

		}

		if (m_IsReload) { return; }

		m_reloadCount++;
		//弾の初期化

		D3DXMATRIX Ahead = game->GetGameCamara()->GetSpringGamera()->GetCamera()->GetCameraRotation();/*/m_Skinmodel.GetMatrix();//*/ //プレイヤーの行列を取得
		D3DXVECTOR3 Epos = { 0,0,0 };
		D3DXVECTOR3 Length;
		m_Mat =  Ahead;

		D3DXVECTOR3 bulletPos = m_Position;
		bulletPos.y += 5.0f;
		for (auto enemy : game->GetEnemys())
		{
			if (D3DXVec3Length(&Length) > D3DXVec3Length(&(enemy->Getpos() - bulletPos)))
			{

				Epos = enemy->Getpos();
				Epos.y += 10.0f;
				Length = Epos - bulletPos;
			}
		}

		Epos = m_EnemyBox[m_EnemyNo].Position;
		Epos.y += 5.0f;
		Epos = m_EnemyBox[m_EnemyNo].Position - m_Position;
		game->GetGameCamara()->Getcamera()->Update();
		D3DXVECTOR3 PlayerFront = { Ahead.m[2][0],Ahead.m[2][1],Ahead.m[2][2] };
		D3DXVec3Add(&bulletPos, &bulletPos, &(PlayerFront * 2.0f));

		
			

			//使われてない弾を探して使われえないのが有ればそのインスタンスを使いまわす
			for (auto& PlayerBullet : game->GetPlayerBullet())
			{

				if (PlayerBullet->GetFree())
				{
					Isfree = true;

					if (m_ZAttent)
					{
						//PlayerBullet->Start(bulletPos, Epos);
						PlayerBullet->homingStart(*m_lockTarget, bulletPos, Epos);
					}
					else
					{

						PlayerBullet->Start(bulletPos, PlayerFront);
					}
					break;

				}

			}

			//使えるものが無ければ新たに追加する
			if (!Isfree)
			{
				Bullet* bullet = new Bullet();

				if (m_ZAttent)
				{
					//bullet->Start(bulletPos, Epos);
					bullet->homingStart(*m_lockTarget, bulletPos, Epos);
				}

				else
				{

					bullet->Start(bulletPos, PlayerFront);//プレイヤーの前方向を渡す
				}
				game->AddPlayerBullets(bullet);

			}
		
		m_bulletIntervalTime = 0.0f;
		m_PlayerSE->Init("Assets/Sound/SE/BulletSound2.wav");
		m_PlayerSE->SetVolume(0.1f);
		m_PlayerSE->Play(true);
		m_PlayerSE->Update();

	}

	if (m_bulletIntervalTime <= BULLETTIME) {
		m_bulletIntervalTime += GameTime().GetFrameDeltaTime();
	}

	


}

//アニメーションの処理
void CPlayer::Animetion()
{
	if (m_Pad.IsTrigger(Pad::enButtonRB1) && m_Animation.GetAnimNo() !=1)
	{
		m_Animation.PlayAnimation(1);
	}

	
	else if (m_Pad.IsTrigger(Pad::enButtonRB2) && m_Animation.GetAnimNo() != 2)
	{
		m_Animation.PlayAnimation(2);
	}

	else if (m_Pad.GetLStickXF() != 0 && m_Pad.GetLStickYF() != 0 && m_CharacterController.IsOnGround())
	{
		if (!m_Animation.IsPlay())
		{

			m_Animation.PlayAnimation(0);
		}
		
	}
	else
	{
		m_Animation.SetAnimetionLoopFlag(0, false);
	}

	
	m_Animation.Update(1.0f / 30.0f);
}

//描画処理
void CPlayer::Render()
{
	m_Skinmodel.SetShadowMap(false);
	m_Skinmodel.SetShadowRecieve(false);
	
	m_Skinmodel.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
	if (m_ZAttent)
	{
		m_TagetUI->Render(game->GetGameCamara()->Getcamera()->GetViewMatrix(), game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
		
	}
	m_ParticleEmitter.Render(game->GetGameCamara()->Getcamera()->GetViewMatrix(), game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
	m_rederMap->Draw(&m_MapSkinmodel);
}

//シャドウマップ生成用の処理
void CPlayer::LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix)
{
	if (this == NULL) { return; }
	m_Skinmodel.SetShadowMap(true);
	m_Skinmodel.SetShadowRecieve(false);
	m_Skinmodel.Draw(&lightViewMatrix, &lightProjMatrix);
	
}

