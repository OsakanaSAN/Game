#include "stdafx.h"
#include "Player.h"
#include "game.h"
#define  TURBOTIME 5
#define  NomalSpeed 20



enum
{
	Run_anim,		//�u�[�X�g
	Stand_anim,		//�ҋ@��
	Battle_anim,	//�U����
	walk_anim,		//���s

};

CPlayer::CPlayer()
{
	
}


CPlayer::~CPlayer()
{
	m_PlayerSE->SoundDete();
}
void CPlayer::Start()
{
	m_Light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	m_Light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	m_Light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	m_Light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	m_Light.SetDiffuseLightColor(0, D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f));
	m_Light.SetDiffuseLightColor(1, D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f));
	m_Light.SetDiffuseLightColor(2, D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f));
	m_Light.SetDiffuseLightColor(3, D3DXVECTOR4(0.8f, 0.8f, 0.8f, 1.0f));
	m_Light.SetAmbientLight({ 0.5f,0.5f,0.5f,1.0f });
	m_SkinmodelData.LoadModelData("Assets/modelData/robo5.x",NULL);
	m_Skinmodel.Init(&m_SkinmodelData);

	m_Skinmodel.SetLight(&m_Light);
	//�A�j���[�V�����̏�����
	/*animation.PlayAnimation(Stand_anim);
	animation.SetAnimetionLoopFlag(Run_anim, false);
	animation.SetAnimationEndTime(Stand_anim, -1.0f);
	animation.SetAnimationEndTime(walk_anim, 0.54f);
	animation.SetAnimetionLoopFlag(Stand_anim,true);
	animation.SetAnimetionLoopFlag(Battle_anim,false);*/
	
	m_CharacterController.Init(1.0f, 1.0f, m_Position);
	m_CharacterController.SetGravity(-9.0f); //�d�͂̐ݒ�

	m_PlayerSE = new CSoundSource;
	m_PlayerSE->Init("Assets/Sound/SE/LoockOnSound.wav");
	
}


void CPlayer::Update()
{
	
	m_Camera = game->GetCamera();
	m_CharacterController.Execute();
	//PlayerAnimation(); //�A�j���[�V�����̐ݒ�
	//Animetion();  //�A�j���[�V����
	MovePlayer();//�v���C���[�̈ړ�
	OnLock(); //���b�N�I��
	m_Position = m_CharacterController.GetPosition();
	m_Pad.Update(); //�p�b�h�̃A�v�f
	m_Skinmodel.UpdateWorldMatrix(m_Position, m_Rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
}

//���b�N�I��
void CPlayer::OnLock()
{
	if (m_NoEenemy) { return; }
	
	if (m_Pad.IsTrigger(Pad::enButtonLB1))
	{
		if (!m_ZAttent)
		{
			m_PlayerSE->Init("Assets/Sound/SE/LoockOnSound.wav");
			m_PlayerSE->Play(true);
			m_PlayerSE->Update();
			m_ZAttent = true;
		}
		else {
			
			m_ZAttent = false;
			game->GetGameCamara()->LookOnCamera(m_Enemypos, false);
		}
		
	}
	D3DXVECTOR3 Length;
	bool        LockEnemy = false;
	//��������ԋ߂��G�Ƀ��b�N�I��������
	for (auto enemy : game->GetEnemys())
		{
			if (D3DXVec3Length(&Length) > D3DXVec3Length(&(enemy->Getpos() - m_Position)))
			{
				LockEnemy = true;
				m_Enemypos = enemy->Getpos();
				Length = m_Enemypos - m_Position;
			}
		}
	if (!LockEnemy)
	{
		m_ZAttent = false;
		game->GetGameCamara()->LookOnCamera(m_Enemypos, false);
		return;
	}
	
	
	D3DXVECTOR3 pPos = m_Enemypos;
	D3DXVec3Subtract(&pPos, &pPos, &m_Position);

	if (D3DXVec3Length(&pPos) > 5000)
	{
		m_ZAttent = false;
	}

	if (m_ZAttent) {

		D3DXVECTOR3 Def;
		D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };
		D3DXVECTOR3 topos = m_Enemypos - m_Position;
		D3DXVec3Normalize(&topos, &topos);
		D3DXQUATERNION mul;
		D3DXVECTOR3 rotAxis;
		D3DXVec3Cross(&rotAxis, &UP, &topos);

		D3DXVec3Subtract(&Def, &m_Enemypos, &m_Position);
		D3DXQuaternionRotationAxis(&m_Rotation, &UP, atan2f(Def.x, Def.z));
		D3DXQuaternionRotationAxis(&mul, &-rotAxis, topos.y);
		D3DXQuaternionMultiply(&m_Rotation, &m_Rotation, &mul);
		game->GetGameCamara()->LookOnCamera(m_Enemypos,true);
		
		
	}

}

void CPlayer::PlayerAnimation()
{

	float R_stickX = m_Pad.GetLStickXF() *2.0f;
	float R_stickZ = m_Pad.GetLStickYF() *2.0f;
	



	if (fabs(R_stickX) > 0 || fabs(R_stickZ) > 0)
	{
		
		m_AnimationNo = Run_anim;
		m_Animation.SetAnimationEndTime(Run_anim, -1.0f);
	}

	else
	{
		m_AnimationNo = Stand_anim;
	}

	if (m_Pad.IsPress(Pad::enButtonRB1)&&!m_BattleMotion)
	{
		m_AnimationNo = Battle_anim;
		m_BattleMotion = true;
		
	}

	
	
}

void CPlayer::MovePlayer()
{

	///////////////////////////////////////////////////////////////////////////////////
		//�L�����̌����ƈړ����x�̌v�Z

	D3DXVECTOR3 AnglemoveSpeed = m_CharacterController.GetMoveSpeed();
	AnglemoveSpeed.y = 0;
	D3DXVECTOR3 moveDirLocal;
	moveDirLocal.y = 1.0f;
	moveDirLocal.x = m_Pad.GetLStickXF();
	moveDirLocal.z = m_Pad.GetLStickYF();
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
	//����
	//�㏸
	if (m_Pad.IsPress(Pad::enButtonY)&& m_Position.y <= 2190)
	{
		AnglemoveSpeed.y = 20;

		m_CharacterController.Jump();
	}
	//���~
	if (m_Pad.IsPress(Pad::enButtonA) )
	{
	
	   AnglemoveSpeed.y = -20;
	   m_CharacterController.Jump();
	}

	//�ړ��ʂ̌v�Z
	if (m_Pad.GetLStickXF() != 0 && m_Pad.GetLStickYF() != 0 && m_Inertia < NomalSpeed && !m_Oveline)
	{
		m_Inertia += 1.5f;
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

	//�p�b�g�̓��͂�������Ί��������X�Ɏ�߂�
	if (m_Oveline)
	{
		m_X_input_old = m_X_input;
		m_Y_input_old = m_Y_input;
		m_Inertia -= 1.5f;
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
			
			/*if (m_Pad.GetLStickXF() != 0 && m_Pad.GetLStickYF() != 0)
			{
				
				m_Inertia -= 15.5f;

			}*/

	}
	


///////////////////////////////////////////////////////////////////////////////////

	AnglemoveSpeed = InitBootht(AnglemoveSpeed); //�u�[�X�g�̏���

	
////////////////////////////////////////////////////////////////////////////////////////////

	InitBullet(); //�v���C���[�̒e�̏���

	///////////////////////////////////////////////////////////////////////////////////

	if (AnglemoveSpeed == 0)
	{
		m_AnimationNo = Stand_anim;
	}
	
	if (m_CharacterController.IsOnGround())
	{
		m_CharacterController.SetGravity(-9.8f);
	}

	AnglemoveSpeed.x = moveDir.x * m_Inertia;
	AnglemoveSpeed.z = moveDir.z * m_Inertia;

	m_CharacterController.SetMoveSpeed(AnglemoveSpeed);
	m_CharacterController.Execute();
	

	///////////////////////////////////////////////////////////////////////////////////
	

	TransformAngle();//�L�����̌�����ς��鏈��
	
}

//�u�[�X�g�̏���
const D3DXVECTOR3& CPlayer::InitBootht(D3DXVECTOR3& MoveSpeed)
{
	//�u�[�X�g
	m_BoothtTime = m_DashTime;
	
		

	
	
	if (m_Pad.IsPress(Pad::enButtonRB2) && m_DashTime < TURBOTIME && !m_NoBoothtInput)
	{
		

		if (m_Pad.GetLStickXF() != 0 && m_Pad.GetLStickYF() != 0)
		{
			m_Inertia = NomalSpeed;
			m_Inertia *= 4;
			

		}

		m_DashTime += GameTime().GetFrameDeltaTime();
		if (m_DashTime == TURBOTIME)
		{
			m_NoBoothtInput = true;
		}


	}

	//
	else if (m_DashTime > 0)
	{
		m_DashTime -= GameTime().GetFrameDeltaTime();
		if (m_DashTime < 0)
		{
			m_DashTime = 0;
		}
		//characterController.SetGravity(-9.8f); //�d�͂̐ݒ�
		if (m_Inertia > NomalSpeed)
		{
			
			m_Inertia = 30.0f;
		}
	}

	//�ău�[�X�g����
	if (!m_Pad.IsPress(Pad::enButtonRB2) && m_NoBoothtInput)
	{
		m_NoBoothtInput = false;
		m_Inertia = 30;
		/*MoveSpeed.x = 0;
		MoveSpeed.z = 0;*/

	}


	if (m_DashTime >= TURBOTIME)
	{
		m_Inertia = 30;
		//characterController.SetGravity(-9.8f); //�d�͂̐ݒ�
	}

	m_MoveSpeed = m_Inertia;

	return MoveSpeed;
}

//�v���C���[�̌����v�Z
void CPlayer::TransformAngle()
{
	D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };

	D3DXVECTOR3 topos = game->GetCamera()->GetLookatPt() - game->GetCamera()->GetEyePt();
	D3DXVec3Normalize(&topos, &topos);
	D3DXQUATERNION mul;
	D3DXVECTOR3 rotAxis;
	D3DXVec3Cross(&rotAxis, &UP, &topos);

	D3DXQuaternionRotationAxis(&m_Rotation, &UP, atan2(topos.x, topos.z));
	D3DXQuaternionRotationAxis(&mul, &-rotAxis, topos.y);
	D3DXQuaternionMultiply(&m_Rotation, &m_Rotation, &mul);

	

}


//�v���C���[�̒e�̏���
void CPlayer::InitBullet()
{
	//�e�̐���
	D3DXMATRIX Ahead = m_Skinmodel.GetMatrix(); //�v���C���[�̍s����擾
	D3DXVECTOR3 Epos = { 0,0,0 };
	D3DXVECTOR3 Length;
	
	//D3DXVec3Normalize(&Epos, &Epos);
	if (m_Pad.IsPress(Pad::enButtonRB1) && m_bulletFireInterval == 0) {
		Bullet* bullet = new Bullet();
		D3DXVECTOR3 bulletPos = m_Position;
		bulletPos.y += 2.0f;
		for (auto enemy : game->GetEnemys())
		{
			if (D3DXVec3Length(&Length) > D3DXVec3Length(&(enemy->Getpos() - bulletPos)))
			{

				Epos = enemy->Getpos();
				Epos.y += 2.0f;
				Length = Epos - bulletPos;
			}
		}
		Epos = Epos - bulletPos;
		game->GetCamera()->Update();
		bullet->Start(bulletPos,{ Ahead.m[2][0],Ahead.m[2][1],Ahead.m[2][2] });//�v���C���[�̑O������n��

		if (m_ZAttent)
		{
			bullet->Start(bulletPos, Epos);
		}
		game->AddPlayerBullets(bullet);
		m_bulletFireInterval = 10;
		m_PlayerSE->Init("Assets/Sound/SE/BulletSound2.wav");
		m_PlayerSE->Play(true);
		m_PlayerSE->Update();
	}
	m_bulletFireInterval--;
	if (m_bulletFireInterval < 0) {
		m_bulletFireInterval = 0;
	}

}

//�A�j���[�V�����̏���
void CPlayer::Animetion()
{
	if (m_AnimationNo == Battle_anim && !m_BattleMotion)
	{
		m_Animation.PlayAnimation(Battle_anim, 0.4f);
		m_BattleMotion = true;
	}

	else if (m_Inertia != 0 && !m_IsMove)
	{
		m_Animation.PlayAnimation(Run_anim,0.6f);
		m_AnimationNo = Run_anim;
		m_IsMove = true;
	}

	else if (m_Inertia == 0 && m_IsMove)
	{

		m_Animation.PlayAnimation(Stand_anim);
		m_IsMove = false;
	}
	m_Animation.Update(1.0f / 60.0f);
}

//�`�揈��
void CPlayer::Render()
{
	m_Skinmodel.SetShadowMap(false);
	m_Skinmodel.SetShadowRecieve(false);
	m_Skinmodel.Draw(&game->GetCamera()->GetViewMatrix(), &game->GetCamera()->GetProjectionMatrix());
}

//�V���h�E�}�b�v�����p�̏���
void CPlayer::LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix)
{
	
	m_Skinmodel.SetShadowMap(true);
	m_Skinmodel.SetShadowRecieve(false);
	m_Skinmodel.Draw(&lightViewMatrix, &lightProjMatrix);
	
}

