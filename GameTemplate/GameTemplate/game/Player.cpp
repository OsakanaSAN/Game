#include "stdafx.h"
#include "Player.h"
#include "game.h"
#define  TURBO 600
#define  NomalSpeed 200



enum
{
	Run_anim,		//ブースト
	Stand_anim,		//待機中
	Battle_anim,	//攻撃時
	walk_anim,		//歩行

};

Player::Player()
{
	for (int i = 0;i < 3;i++)
	{
		EnemyIt[i] = true;
	}
	
}


Player::~Player()
{
}
void Player::Start()
{
	PLight.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	PLight.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	PLight.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	PLight.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	PLight.SetDiffuseLightColor(0, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	PLight.SetDiffuseLightColor(1, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	PLight.SetDiffuseLightColor(2, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	PLight.SetDiffuseLightColor(3, D3DXVECTOR4(0.2f, 0.2f, 0.2f, 1.0f));
	PLight.SetAmbientLight({ 0.5f,0.5f,0.5f,1.0f });
	skinmodelData.LoadModelData("Assets/modelData/robo3.X",&animation);
	skinmodel.Init(&skinmodelData);
	

	skinmodel.SetLight(&PLight);
	animation.PlayAnimation(Stand_anim);
	animation.SetAnimetionLoopFlag(Run_anim, false);
	animation.SetAnimationEndTime(Stand_anim, -1.0f);
	animation.SetAnimationEndTime(walk_anim, 0.54f);
	animation.SetAnimetionLoopFlag(Stand_anim,true);
	animation.SetAnimetionLoopFlag(Battle_anim,false);
	
	characterController.Init(1.0f, 1.0f, position);
	characterController.SetGravity(-98.0f); //重力の設定

}


void Player::Update()
{
	camera = game->GetCamera();
	characterController.Execute();
	PlayerAnimation(); //アニメーションの設定
	Animetion();  //アニメーション
	AnglePlayer();//プレイヤーの向き
	LockOn(); //ロックオン
	pad.Update(); //パッドのアプデ
	skinmodel.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

//ロックオン
void Player::LockOn()
{
	if (NoEenemy) { return; }
	
	if (pad.IsTrigger(Pad::enButtonLB1))
	{
		if (!ZAttent)
		{
			ZAttent = true;
		}
		else {
			ZAttent = false;
			++LockNo;
		}
		
	}
	int oldLockNo = LockNo;
	while (!EnemyIt[LockNo])
	{
		++LockNo;
		if (LockNo == 3)
		{
			LockNo = 0;
		}
		if (LockNo == oldLockNo)
		{
			NoEenemy = true;
			ZAttent = false;
			break;
		}

	}
	
	if (NoEenemy) { return; }
	
	D3DXVECTOR3 pPos = Enemypos[LockNo];
	D3DXVec3Subtract(&pPos, &pPos, &position);

	

	if (D3DXVec3Length(&pPos) > 5000)
	{
		ZAttent = false;
	}

	if (ZAttent) {
		D3DXVECTOR3 Def;
		D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
		D3DXVec3Subtract(&Def, &Enemypos[LockNo], &position);
		D3DXQuaternionRotationAxis(&rotation, &UP, atan2f(Def.x, Def.z));
	}

}

void Player::PlayerAnimation()
{

	float R_stickX = pad.GetLStickXF() *2.0f;
	float R_stickZ = pad.GetLStickYF() *2.0f;
	



	if (fabs(R_stickX) > 0 || fabs(R_stickZ) > 0)
	{
		
		AnimationNo = Run_anim;
		animation.SetAnimationEndTime(Run_anim, -1.0f);
	}

	else
	{
		AnimationNo = Stand_anim;
	}

	if (pad.IsPress(Pad::enButtonRB1)&&!Battle_Motion)
	{
		AnimationNo = Battle_anim;
		Battle_Motion = true;
		
	}

	
	
}

void Player::AnglePlayer()
{

	///////////////////////////////////////////////////////////////////////////////////
		//キャラの向きと移動速度の計算

	D3DXVECTOR3 AnglemoveSpeed = characterController.GetMoveSpeed();
	AnglemoveSpeed.y = 0;
	D3DXVECTOR3 moveDirLocal;
	moveDirLocal.y = 1.0f;
	moveDirLocal.x = pad.GetLStickXF();
	moveDirLocal.z = pad.GetLStickYF();
	D3DXMATRIX mViveInv = game->GetCamera()->GetViewMatrixInv();

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
	//慣性
	//上昇
	if (pad.IsPress(Pad::enButtonY)&& position.y <= 2190)
	{
		AnglemoveSpeed.y = 50;

		characterController.Jump();
	}
	//下降
	if (pad.IsPress(Pad::enButtonA) )
	{
	
	   AnglemoveSpeed.y = -50;
		characterController.Jump();
	}


	if (pad.GetLStickXF() != 0 && pad.GetLStickYF() != 0 && inertia <= NomalSpeed && !oveline)
	{
		inertia += 7.5f;
		if (X_input != pad.GetLStickXF())
		{

			X_input = pad.GetLStickXF();
			Y_input = pad.GetLStickYF();
		}
		oveline = false;
	}

	else if (pad.GetLStickXF() == 0 && pad.GetLStickYF() == 0 && inertia > 0)
	{
		oveline = true;


	}

	//パットの入力が無ければ慣性を徐々に弱める
	if (oveline)
	{

			X_input_old = X_input;
			Y_input_old = Y_input;
			inertia -= 25.5f;
			if (pad.IsPress(Pad::enButtonA))
			{
				inertia -= 15.0f;
			}
			if (inertia < 1)
			{
				inertia = 0;
				oveline = false;
			}
			moveDir.x = cameraX.x * X_input + cameraZ.x * Y_input;
			moveDir.y = 0.0f;
			moveDir.z = cameraX.z * X_input + cameraZ.z * Y_input;
			
			if (pad.GetLStickXF() != 0 && pad.GetLStickYF() != 0)
			{
				
				inertia -= 25.5f;

			}

	}
	


///////////////////////////////////////////////////////////////////////////////////

	AnglemoveSpeed = BoothtInit(AnglemoveSpeed); //ブーストの処理

	
////////////////////////////////////////////////////////////////////////////////////////////

	BulletInit(); //プレイヤーの弾の処理

	///////////////////////////////////////////////////////////////////////////////////

	if (AnglemoveSpeed == 0)
	{
		AnimationNo = Stand_anim;
	}
	
	if (characterController.IsOnGround())
	{
		characterController.SetGravity(-9.8f);
	}

	AnglemoveSpeed.x = moveDir.x * inertia;
	AnglemoveSpeed.z = moveDir.z * inertia;

	characterController.SetMoveSpeed(AnglemoveSpeed);
	characterController.Execute();
	

	///////////////////////////////////////////////////////////////////////////////////
	

	ChangeAngle();//キャラの向きを変える処理
	
}

//ブーストの処理
D3DXVECTOR3 Player::BoothtInit(D3DXVECTOR3 MoveSpeed)
{
	//ブースト
	BoothtTime = DashTime;
	if (pad.IsPress(Pad::enButtonRB2) && DashTime < TURBO && !NoBoothtInput)
	{


		if (pad.GetLStickXF() != 0 && pad.GetLStickYF() != 0)
		{
			inertia = NomalSpeed;
			inertia *= 2;

		}

		DashTime++;
		if (DashTime == TURBO)
		{
			NoBoothtInput = true;
		}


	}

	//
	else if (DashTime > 0)
	{
		DashTime--;
		//characterController.SetGravity(-9.8f); //重力の設定
		if (inertia > NomalSpeed)
		{
			
			inertia -= 50.0f;
		}
	}

	//再ブースト解除
	if (!pad.IsPress(Pad::enButtonRB2) && NoBoothtInput)
	{
		NoBoothtInput = false;
		inertia = 10;
		/*MoveSpeed.x = 0;
		MoveSpeed.z = 0;*/

	}


	if (DashTime >= TURBO)
	{
		//characterController.SetGravity(-9.8f); //重力の設定
	}

	MOVESpeed = inertia;

	return MoveSpeed;
}

//プレイヤーの向き計算
void Player::ChangeAngle()
{
	D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };

	D3DXVECTOR3 topos = game->GetCamera()->GetLookatPt() - game->GetCamera()->GetEyePt();
	D3DXVec3Normalize(&topos, &topos);
	D3DXQUATERNION mul;
	D3DXVECTOR3 rotAxis;
	D3DXVec3Cross(&rotAxis, &UP, &topos);

	D3DXQuaternionRotationAxis(&rotation, &UP, atan2(topos.x, topos.z));
	D3DXQuaternionRotationAxis(&mul, &-rotAxis, topos.y);
	D3DXQuaternionMultiply(&rotation, &rotation, &mul);

	position = characterController.GetPosition();

}


//プレイヤーの弾の処理
void Player::BulletInit()
{
	//弾の生成
	D3DXMATRIX Ahead = skinmodel.GetMatrix(); //プレイヤーの行列を取得
	D3DXVECTOR3 Epos = Enemypos[LockNo] - position;
	D3DXVec3Normalize(&Epos, &Epos);
	if (pad.IsPress(Pad::enButtonRB1) && bulletFireInterval == 0) {
		Bullet* bullet = new Bullet();
		D3DXVECTOR3 bulletPos = position;
		bulletPos.y += 70.0f;
		game->GetCamera()->Update();
		bullet->Start(bulletPos, { Ahead.m[2][0],Ahead.m[2][1],Ahead.m[2][2] });//プレイヤーの前方向を渡す

		if (ZAttent)
		{
			bullet->Start(bulletPos, Epos);
		}
		game->AddPlayerBullets(bullet);
		bulletFireInterval = 10;
	}
	bulletFireInterval--;
	if (bulletFireInterval < 0) {
		bulletFireInterval = 0;
	}

}

//アニメーションの処理
void Player::Animetion()
{
	if (AnimationNo == Battle_anim && !Battle_Motion)
	{
		animation.PlayAnimation(Battle_anim, 0.4f);
		Battle_Motion = true;
	}

	else if (inertia != 0 && !IsMove)
	{
		animation.PlayAnimation(Run_anim,0.6f);
		AnimationNo = Run_anim;
		IsMove = true;
	}

	else if (inertia == 0 && IsMove)
	{

		animation.PlayAnimation(Stand_anim);
		IsMove = false;
	}
	animation.Update(1.0f / 60.0f);
}

//描画処理
void Player::Render()
{
	skinmodel.SetShadowMap(false);
	skinmodel.SetShadowRecieve(false);
	skinmodel.Draw(&game->GetCamera()->GetViewMatrix(), &game->GetCamera()->GetProjectionMatrix());
}

void Player::LightEyePosRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix)
{
	
	skinmodel.SetShadowMap(true);
	skinmodel.SetShadowRecieve(false);
	skinmodel.Draw(&lightViewMatrix, &lightProjMatrix);
	
}

