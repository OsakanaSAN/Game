#include "stdafx.h"
#include "Player.h"
#include "game.h"
#define  TURBOTIME 5
#define  NomalSpeed 30
#define	 BULLETTIME 0.1f


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
	m_PlayerSE->Release();
	delete m_PlayerSE;

	m_CharacterController.RemoveRigidBoby();
	m_SkinmodelData.Release();

}
void CPlayer::Start()
{
	m_TagetUI = new CGUI;
	m_TagetUI->Start({ 10.0f,10.0f }, "Assets/sprite/HUD2.png");
	//m_Light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	//m_Light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	//m_Light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	//m_Light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	//m_Light.SetDiffuseLightColor(0, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_Light.SetDiffuseLightColor(1, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_Light.SetDiffuseLightColor(2, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_Light.SetDiffuseLightColor(3, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	m_Light.SetAmbientLight({ 1.5f,1.5f,1.5f,1.0f });
	m_SkinmodelData.LoadModelData("Assets/modelData/robo6.x",&m_Animation);
	m_Skinmodel.Init(&m_SkinmodelData);

	m_Skinmodel.SetLight(&m_Light);
	//�A�j���[�V�����̏�����
	//m_Animation.Init();
	m_Animation.PlayAnimation(0);
	m_Animation.SetAnimetionSpeed(1.0f);
	m_Animation.SetAnimetionLoopFlag(0, true);
	/*
	animation.SetAnimetionLoopFlag(Run_anim, false);
	animation.SetAnimationEndTime(Stand_anim, -1.0f);
	animation.SetAnimationEndTime(walk_anim, 0.54f);
	animation.SetAnimetionLoopFlag(Stand_anim,true);
	animation.SetAnimetionLoopFlag(Battle_anim,false);*/
	
	m_CharacterController.Init(1.0f, 1.0f, m_Position);
	m_CharacterController.SetGravity(-20.0f); //�d�͂̐ݒ�

	m_PlayerSE = new CSoundSource;
	m_PlayerSE->Init("Assets/Sound/SE/LoockOnSound.wav");
	m_EnemyNo = 0;

	SParicleEmitParameter SparticleEmit;
	SparticleEmit.texturePath = "Assets/Particle/smoke3.png";
	SparticleEmit.w = 1.5f;
	SparticleEmit.h = 2.5f;
	SparticleEmit.intervalTime = 0.3f;
	SparticleEmit.timer = 1.0f;
	SparticleEmit.initSpeed = D3DXVECTOR3(0.0f, -2.0f, 0.0f);
	m_ParticleEmitter.Init(SparticleEmit);
	//m_ParticleEmitter.SetPosition({ m_Position.x,m_Position.y + 5.0f,m_Position.z });
}


void CPlayer::Update()
{
	
	Animetion();  //�A�j���[�V����
	MovePlayer();//�v���C���[�̈ړ�
	OnLock(); //���b�N�I��
	
	m_Pad.Update(); //�p�b�h�̃A�v�f

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
	m_ParticleEmitter.SetPosition({ m_Position.x,m_Position.y + 5.0f,m_Position.z });
	if (m_CharacterController.GetMoveSpeed().y != 0)
	{
		m_ParticleEmitter.SetLockPosition(true);
	}
	else 
	{
		m_ParticleEmitter.SetLockPosition(false);
	}
	//m_ParticleEmitter.Update();
	m_Skinmodel.UpdateWorldMatrix(m_Position, m_Rotation, D3DXVECTOR3(2.0f, 2.0f, 2.0f));

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
	int			Box = 0;				//�G�̐�������  
	float		angle = 0.0f;
	//��������ԋ߂��G�Ƀ��b�N�I��������
	for (const auto& enemy : game->GetEnemys())
	{
			//�J�����̑O����
			m_FrontPosition = { m_Mat.m[2][0],m_Mat.m[2][1], m_Mat.m[2][2] };
			//Player����Enemy�Ɍ������x�N�g�����v�Z����B
			D3DXVECTOR3 toPlayer = enemy->Getpos() - m_Position;
			//���K��
			D3DXVec3Normalize(&toPlayer, &toPlayer);
			D3DXVec3Normalize(&m_FrontPosition, &m_FrontPosition);

			angle = D3DXVec3Dot(&toPlayer, &m_FrontPosition);
			angle = acos(angle);

			//�G���v���C���[�Ɍ������x�N�g�����擾
			Lenght2 = m_Position - enemy->Getpos();

		
			/*if (LockEnemy)
			{
				if (m_EnemyNo != Box) { return; }
				LockEnemy = true;
				m_EnemyBox[Box].Position = enemy->Getpos();
				m_EnemyBox[Box].Number = Box;
				m_EnemyNo = Box;
				break;
			}*/

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
	
	
	/*D3DXVECTOR3 pPos = m_EnemyBox[m_EnemyNo].Position;
	D3DXVec3Subtract(&pPos, &pPos, &m_Position);

	if (D3DXVec3Length(&pPos) > 5000)
	{
		m_ZAttent = false;
	}*/

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
		//�L�����̌����ƈړ����x�̌v�Z

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

	//�㏸���~�̐��䕔��
	{

		if (m_Pad.IsPress(Pad::enButtonY) && AnglemoveSpeed.y <= 30.0f)
		{

			AnglemoveSpeed.y += 2.0f;
			m_CharacterController.Jump();

		}
		//���~
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

	//�ړ��ʂ̌v�Z
	if (m_Pad.GetLStickXF() != 0 && m_Pad.GetLStickYF() != 0  && !m_Oveline)
	{
		//�����Ă��鎞�̈ړ����@
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

	//�p�b�g�̓��͂�������Ί��������X�Ɏ�߂�
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
		m_CharacterController.SetGravity(0.0f);
	}

	//�����Ă��鎞�̈ړ���
	if (!m_CharacterController.IsOnGround())
	{
		AnglemoveSpeed.x = moveDir.x * m_Inertia;
		AnglemoveSpeed.z = moveDir.z * m_Inertia;
	}

	//�n�ʂɂ��Ă��鎞�̈ړ���
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
	

	TransformAngle();//�L�����̌�����ς��鏈��
	
}

//�u�[�X�g�̏���
const D3DXVECTOR3& CPlayer::InitBootht(D3DXVECTOR3& MoveSpeed)
{
	//�_�b�V���ł��鎞�Ԃ̌v�Z
	if (m_Pad.IsPress(Pad::enButtonRB2) && m_DashTime < TURBOTIME && !m_NoBoothtInput)
	{
		//�_�b�V���ł��鎞�Ԃ��i�[
		m_DashTime += GameTime().GetFrameDeltaTime();

		//�p�b�h�̓��͂��L��΃_�b�V������B
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

	//R2��������Ă��Ȃ��Ίɂ߂�
	else if (m_DashTime > 0)
	{
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

	//�ău�[�X�g����
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
		//characterController.SetGravity(-9.8f); //�d�͂̐ݒ�
	}

	m_MoveSpeed = m_Inertia;

	return MoveSpeed;
}

//�v���C���[�̌����v�Z
void CPlayer::TransformAngle()
{
	if (m_ZAttent) { return; }
	D3DXVECTOR3 UP = { 0.0f,1.0f,0.0f };

	D3DXVECTOR3 topos = game->GetGameCamara()->Getcamera()->GetLookatPt() - game->GetGameCamara()->Getcamera()->GetEyePt();
	D3DXVec3Normalize(&topos, &topos);
	D3DXQUATERNION mul , rot;
	D3DXVECTOR3 rotAxis;
	D3DXVec3Cross(&rotAxis, &UP, &topos);
	D3DXMATRIX Ahead = m_Skinmodel.GetMatrix(); //�v���C���[�̍s����擾
	
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


//�v���C���[�̒e�̏���
void CPlayer::InitBullet()
{
	//�e�̐���
	D3DXMATRIX Ahead = game->GetGameCamara()->GetSpringGamera()->GetCamera()->GetCameraRotation();/*/m_Skinmodel.GetMatrix();//*/ //�v���C���[�̍s����擾
	D3DXVECTOR3 Epos = { 0,0,0 };
	D3DXVECTOR3 Length;
	m_Mat = Ahead;
	//D3DXVec3Normalize(&Epos, &Epos);
	if (m_Pad.IsPress(Pad::enButtonRB1) && m_bulletIntervalTime >= BULLETTIME) {
		Bullet* bullet = new Bullet();
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
		bullet->Start(bulletPos, PlayerFront);//�v���C���[�̑O������n��


		if (m_ZAttent)
		{
			bullet->Start(bulletPos, Epos);
		}
		game->AddPlayerBullets(bullet);
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

//�A�j���[�V�����̏���
void CPlayer::Animetion()
{
	
	if (m_Pad.GetLStickXF() != 0 && m_Pad.GetLStickYF() != 0 && m_CharacterController.IsOnGround())
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

	
	m_Animation.Update(1.0f / 60.0f);
}

//�`�揈��
void CPlayer::Render()
{
	m_Skinmodel.SetShadowMap(false);
	m_Skinmodel.SetShadowRecieve(false);
	//m_ParticleEmitter.Render(game->GetCamera()->GetViewMatrix(), game->GetCamera()->GetProjectionMatrix());
	m_Skinmodel.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
	if (m_ZAttent)
	{
		m_TagetUI->Render(game->GetGameCamara()->Getcamera()->GetViewMatrix(), game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
	}
	//if (m_ZoomBlurOn)
	//{
		//m_ParticleEmitter.Render(game->GetGameCamara()->Getcamera()->GetViewMatrix(), game->GetGameCamara()->Getcamera()->GetProjectionMatrix());


	//}
	
}

//�V���h�E�}�b�v�����p�̏���
void CPlayer::LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix)
{
	if (this == NULL) { return; }
	m_Skinmodel.SetShadowMap(true);
	m_Skinmodel.SetShadowRecieve(false);
	m_Skinmodel.Draw(&lightViewMatrix, &lightProjMatrix);
	
}

