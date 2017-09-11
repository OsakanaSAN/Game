#include "stdafx.h"
#include "Player.h"
#include "game.h"
#define  TURBO 1000



enum
{
	Stand_anim,
	Walk_anim,
	Run_anim,
	Attack_anim,
	Dame_anim,

};

Player::Player()
{
	
	PLight.SetAmbientLight({ 1.0f,1.0f,1.0f,1.0f });
}


Player::~Player()
{
}
void Player::Start()
{
	skinmodelData.LoadModelData("Assets/modelData/Unity.X",&animation);
	skinmodel.Init(&skinmodelData);
	skinmodel.SetLight(&PLight);
	characterController.Init(0.5f, 1.0f, position);
	characterController.SetGravity(-9.8f); //重力の設定
	
	

}


void Player::Update()
{
	camera = game->GetCamera();
	characterController.Execute();
	MovePlayer(); //入力系のやつ
	Animetion();  //アニメーション
	AnglePlayer();//プレイヤーの向き
	Attention(); //ロックオン
	pad.Update(); //パッドのアプデ
	skinmodel.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

//ロックオン
void Player::Attention()
{
	D3DXVECTOR3 pPos = game->GetEnemy()->Getpos();
	D3DXVec3Subtract(&pPos, &pPos, &position);
	if (pad.IsTrigger(Pad::enButtonLB2) == true)
	{
		
			ZAttent = true;

	}

	else if (pad.IsTrigger(Pad::enButtonLB2) == false)
	{
		ZAttent = false;
	}

	if (D3DXVec3Length(&pPos) > 150)
	{
		ZAttent = false;
	}

	if (ZAttent) {
		D3DXVECTOR3 Def;
		D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
		D3DXVec3Subtract(&Def, &game->GetEnemy()->Getpos(), &position);
		D3DXQuaternionRotationAxis(&rotation, &UP, atan2f(Def.x, Def.z));
	}

}

void Player::MovePlayer()
{

	float R_stickX = pad.GetLStickXF() *2.0f;
	float R_stickZ = pad.GetLStickYF() *2.0f;
	D3DXVECTOR3 MoveSpeed = characterController.GetMoveSpeed();
	D3DXVECTOR3 OldCameraPos = camera->GetEyePt();
	MoveSpeed.y =  0.0f;
	MoveSpeed.x =  0.0f ;
	MoveSpeed.z =  0.0f ;
	//MoveSpeed = { skinmodel.GetMatrix().m[2][0],skinmodel.GetMatrix().m[2][1],skinmodel.GetMatrix().m[2][2] };
	MoveSpeed *= 5;

///////////////////////////////////////////////////////////////////////////////////
	//入力の判定　(後で関数化する)


	if (fabs(R_stickX) > 0)
	{
		
		AnimationNo = Run_anim;
		//animation.SetAnimationEndTime(Run_anim,0.8);
	}
	if (fabs(R_stickZ) > 0)
	{
		AnimationNo = Run_anim;
		animation.SetAnimationEndTime(Run_anim, 0.8);
	}
	else
	{
		AnimationNo = Stand_anim;
	}

	if (pad.IsPress(Pad::enButtonY))
	{
		MoveSpeed.y = 5;
		
		characterController.Jump();
	}
	
	if (pad.IsPress(Pad::enButtonA))
	{
		MoveSpeed *= 0;
		MoveSpeed.y = -5;
		characterController.Jump();
	}
	
///////////////////////////////////////////////////////////////////////////////////

	characterController.SetMoveSpeed(MoveSpeed);
	characterController.Execute();
	position = characterController.GetPosition();
	//camera->SetEyePt(OldCameraPos);
}
void Player::AnglePlayer()
{

	///////////////////////////////////////////////////////////////////////////////////
		//キャラの向きと移動速度の計算

	D3DXVECTOR3 AnglemoveSpeed = characterController.GetMoveSpeed();

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
	/*AnglemoveSpeed.x = moveDir.x * inertia;
	AnglemoveSpeed.z = moveDir.z * inertia;*/

	if (pad.GetLStickXF() != 0 && pad.GetLStickYF() != 0 && inertia <= 15 && !oveline)
	{
		inertia += 0.2f;
		X_input = pad.GetLStickXF();
		Y_input = pad.GetLStickYF();
		oveline = false;
	}

	else if (pad.GetLStickXF() == 0 && pad.GetLStickYF() == 0 && inertia > 0)
	{
		oveline = true;


	}

	if (oveline)
	{

			X_input_old = X_input;
			Y_input_old = Y_input;
			inertia -= 0.015f;
			if (pad.IsPress(Pad::enButtonA))
			{
				inertia -= 0.1f;
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
				/*if (pad.GetLStickXF() > 0 && X_input_old > 0 || pad.GetLStickXF() < 0 && X_input_old < 0||
					pad.GetLStickYF() > 0 && Y_input_old > 0 || pad.GetLStickYF() < 0 && Y_input_old < 0)
				{
					oveline = false;
				}*/

				inertia -= 0.5f;

			}

			

	}

	
	AnglemoveSpeed.x = moveDir.x * inertia;
	AnglemoveSpeed.z = moveDir.z * inertia;


///////////////////////////////////////////////////////////////////////////////////
		//ブースト
	if (pad.IsTrigger(Pad::enButtonRB2) && DashTime < TURBO)
	{


		if (pad.GetLStickXF() != 0 && pad.GetLStickYF() != 0)
		{ 
		
			//AnglemoveSpeed *= 2;
			inertia = 15;
			inertia *= 2;

		}


		DashTime++;
		characterController.SetGravity(0);

	}
	if (DashTime >= TURBO)
	{
		characterController.SetGravity(-9.8f); //重力の設定
	}
	if (!pad.IsPress(Pad::enButtonRB2) && DashTime > 0)
	{
		DashTime--;
		characterController.SetGravity(-9.8f); //重力の設定
	}

	///////////////////////////////////////////////////////////////////////////////////
	//弾の生成
	D3DXMATRIX Ahead = skinmodel.GetMatrix(); //プレイヤーの行列を取得
	D3DXVECTOR3 Epos = game->GetEnemy()->Getpos() - position;
	D3DXVec3Normalize(&Epos, &Epos);
	if (pad.IsPress(Pad::enButtonRB1) && bulletFireInterval == 0) {
		Bullet* bullet = new Bullet();
		D3DXVECTOR3 bulletPos = position;
		bulletPos.y += 0.5f;
		game->GetCamera()->Update();
		bullet->Start(bulletPos,{ Ahead.m[2][0],Ahead.m[2][1],Ahead.m[2][2] });//プレイヤーの前方向を渡す
		
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

	///////////////////////////////////////////////////////////////////////////////////

	
	characterController.SetMoveSpeed(AnglemoveSpeed);
	characterController.Execute();

	///////////////////////////////////////////////////////////////////////////////////
		//キャラの向きを変える処理


	D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
	if (D3DXVec3LengthSq(&moveDir) > 0.0001f)
	{
		//D3DXQuaternionRotationAxis(&rotation, &UP, atan2f(moveDir.x, moveDir.z));
	}

	D3DXVECTOR3 topos = game->GetCamera()->GetLookatPt() - game->GetCamera()->GetEyePt();
	D3DXVec3Normalize(&topos, &topos);
	D3DXQUATERNION mul;
	D3DXVECTOR3 rotAxis;
	D3DXVec3Cross(&rotAxis, &UP, &topos);
	D3DXQuaternionRotationAxis(&rotation, &UP, atan2(topos.x, topos.z));
	D3DXQuaternionRotationAxis(&mul, &-rotAxis, topos.y);
	D3DXQuaternionMultiply(&rotation,&rotation,&mul);
	MoveSpeed = characterController.GetMoveSpeed();
	
}

//アニメーションの処理
void Player::Animetion()
{


	if (AnimationNo == Run_anim && !IsMove)
	{
		animation.PlayAnimation(Run_anim, 0.1f);
		AnimationNo = Run_anim;
		IsMove = true;
	}
	else if(AnimationNo == Stand_anim && IsMove)
	{
	
		animation.PlayAnimation(Stand_anim,0.1f);
		IsMove = false;
	}
	animation.Update(1.0f / 60.0f);
}

//描画処理
void Player::Render()
{
	camera = game->GetCamera();
	skinmodel.Draw(&camera->GetViewMatrix(), &camera->GetProjectionMatrix());
}

