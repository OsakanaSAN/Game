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
	m_CharacterController.SetGravity(-9.8f); //�d�͂̐ݒ�
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

//�G�̒e�ۏ���
void CEnemy::EnemyBulletON()
{

	
	D3DXMATRIX Ahead = m_Skinmodel.GetMatrix(); //�v���C���[�̍s����擾
	D3DXVECTOR3 pPos = game->GetPlayer()->GetPos();
	D3DXVec3Subtract(&pPos, &pPos, &m_Position);
	//�v���C���[��ǂ��e
	D3DXVECTOR3 Pos = game->GetPlayer()->GetPos() - m_Position;
	D3DXVec3Normalize(&Pos, &Pos);

	m_BulletTime += GameTime().GetFrameDeltaTime();  //60����1�b
	if (D3DXVec3Length(&pPos) < 200 && m_BulletTime > 2.0f) {
		Bullet* bullet = new Bullet();
		D3DXVECTOR3 bulletPos = m_Position;
		bulletPos.y += 1.0f;
		bullet->Start(bulletPos, Pos);//�v���C���[�̑O������n��
		game->AddEnemyBullets(bullet);
		//bulletFireInterval = 20 ; //�b��3��
		m_BulletTime = 0.0f;
	}

	m_BulletFireInterval--;
	if (m_BulletFireInterval < 0) {
		m_BulletFireInterval = 0;
	}

}

//�G�̈ړ��֐�
void CEnemy::EnemyMove()
{
	if (m_CharacterController.IsJump()) {
		
		m_CharacterController.SetPosition(m_Position);
		m_CharacterController.Execute();
		return;
	
	}
	
//////////////////////////////////////////////////////////////////////////////////////////////////////////
	D3DXVECTOR3 pPos = game->GetPlayer()->GetPos(); //�v���C���[�̌��ݒn
	int pSpeeed = game->GetPlayer()->GetMoveSpeed();//�v���C���̈ړ����x

	D3DXVec3Subtract(&pPos, &pPos, &m_Position);

	//�G�̈ړ�
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
	//�v���C���[�̒e���擾
	auto& Bullets = game->GetPlayerBullet();
	D3DXVECTOR3 Movespeed = { 0.0f,0.0f,0.0f };

	//�v���C���[�Ƃ̒e�̓����蔻��̌v�Z
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