#include "stdafx.h"
#include "Player.h"
#include "game.h"



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
	characterController.SetGravity(-9.8f); //�d�͂̐ݒ�
	
	

}


void Player::Update()
{
	camera = game->GetCamera();
	characterController.Execute();
	MovePlayer(); //���͌n�̂��
	Animetion();  //�A�j���[�V����
	AnglePlayer();//�v���C���[�̌���
	pad.Update(); //�p�b�h�̃A�v�f
	//camera->Update();//�J�����̃A�v�f
	skinmodel.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
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

///////////////////////////////////////////////////////////////////////////////////
	//���͂̔���@(��Ŋ֐�������)


	if (fabs(R_stickX) > 0)
	{
		
		AnimationNo = Run_anim;
		animation.SetAnimationEndTime(Run_anim,0.8);
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
	//�L�����̌����ƈړ����x�̌v�Z

	D3DXVECTOR3 AnglemoveSpeed = characterController.GetMoveSpeed();

	D3DXVECTOR3 moveDirLocal;
	moveDirLocal.y = pad.GetLStickYF();
	moveDirLocal.x = pad.GetLStickXF();
	moveDirLocal.z = pad.GetLStickYF();
	D3DXMATRIX mViveInv = game->GetCamera()->GetViewMatrixInv();

	D3DXVECTOR3 cameraZ;
	cameraZ.x = mViveInv.m[2][0];
	cameraZ.y = mViveInv.m[2][1];
	cameraZ.z = mViveInv.m[2][2];
	D3DXVec3Normalize(&cameraZ, &cameraZ);

	D3DXVECTOR3 cameraX;
	cameraX.x = mViveInv.m[0][0];
	cameraX.y = mViveInv.m[0][1];
	cameraX.z = mViveInv.m[0][2];
	D3DXVec3Normalize(&cameraX, &cameraX);

	D3DXVECTOR3 moveDir;

	moveDir.x = cameraX.x * moveDirLocal.x + cameraZ.x * moveDirLocal.z;
	moveDir.y = cameraX.y * moveDirLocal.y + cameraZ.y * moveDirLocal.y;
	moveDir.z = cameraX.z * moveDirLocal.x + cameraZ.z * moveDirLocal.z;
	if (pad.GetLStickXF() != 0 || pad.GetLStickYF() != 0)
	{
		oldDir = moveDir;
	}

	AnglemoveSpeed.x = moveDir.x * 6;
	AnglemoveSpeed.z = moveDir.z * 6;
	AnglemoveSpeed.y = moveDir.y * 6;


///////////////////////////////////////////////////////////////////////////////////
	//�e�̐���
	D3DXMATRIX Ahead = skinmodel.GetMatrix(); //�v���C���[�̍s����擾
	D3DXVECTOR3 enemypos = game->GetEnemy()->Getpos() - position; //�G�Ɍ������x�N�g��
	D3DXVec3Normalize(&enemypos, &enemypos);
	if (pad.IsPress(Pad::enButtonRB1) && bulletFireInterval == 0) {
		Bullet* bullet = new Bullet();
		D3DXVECTOR3 bulletPos = position;
		bulletPos.y += 0.5f;
		bullet->Start(bulletPos, { Ahead.m[2][0],Ahead.m[2][1],Ahead.m[2][2] });//�v���C���[�̑O������n��
		if (pad.IsTrigger(Pad::enButtonLB2))
		{
			bullet->Start(bulletPos,enemypos);//�v���C���[�̑O������n��
		}
		game->AddPlayerBullets(bullet);
		bulletFireInterval = 10;
	}
	bulletFireInterval--;
	if (bulletFireInterval < 0) {
		bulletFireInterval = 0;
	}

///////////////////////////////////////////////////////////////////////////////////
	//�u�[�X�g
	if (pad.IsTrigger(Pad::enButtonRB2)&&DashTime < 600)
	{
		AnglemoveSpeed *= 2.5;
		DashTime++;
		characterController.SetGravity(0);
		

		//�㏸
		if (pad.GetLStickXF() == 0 && pad.GetLStickYF() == 0)
		{
			AnglemoveSpeed.y = 3;
			AnglemoveSpeed.y *= 2.5;
			characterController.Jump();
		}
		
	}
	if (DashTime >= 180)
	{
		characterController.SetGravity(-9.8f); //�d�͂̐ݒ�
	}
	if (!pad.IsPress(Pad::enButtonRB2) && DashTime > 0)
	{
		DashTime--;
		characterController.SetGravity(-9.8f); //�d�͂̐ݒ�
	}

///////////////////////////////////////////////////////////////////////////////////


	characterController.SetMoveSpeed(AnglemoveSpeed);
	characterController.Execute();

///////////////////////////////////////////////////////////////////////////////////
	//�L�����̌�����ς��鏈��
	D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
	if (D3DXVec3LengthSq(&moveDir) > 0.0001f)
	{
		D3DXQuaternionRotationAxis(&rotation, &UP, atan2f(moveDir.x, moveDir.z));
	}

}

//�A�j���[�V�����̏���
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

//�`�揈��
void Player::Render()
{
	camera = game->GetCamera();
	skinmodel.Draw(&camera->GetViewMatrix(), &camera->GetProjectionMatrix());
}

