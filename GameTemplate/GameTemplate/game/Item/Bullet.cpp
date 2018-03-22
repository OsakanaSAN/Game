#include "stdafx.h"
#include "Bullet.h"
#include "game.h"

SkinModelData* Bullet::modelData = NULL;


Bullet::Bullet()
{
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	moveSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	
	
}


Bullet::~Bullet()
{

	m_BulletSe->Release();

	delete m_BulletSe;


}
void Bullet::Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed)
{

	m_position = pos;
	D3DXVec3Dot(&m_position, &moveSpeed);
	PlayerFrontPosition = moveSpeed;
	D3DXVec3Normalize(&PlayerFrontPosition,&PlayerFrontPosition);
	
	this->moveSpeed = PlayerFrontPosition ;//1.5f; //�e������
	
	//���C�g���������B
	//m_light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	//m_light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	//m_light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	//m_light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	//m_light.SetDiffuseLightColor(0, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_light.SetDiffuseLightColor(1, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_light.SetDiffuseLightColor(2, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_light.SetDiffuseLightColor(3, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));

	m_light.SetAmbientLight(D3DXVECTOR4(5.6f, 5.6f, 5.6f, 1.0f));
	
 
	m_characterController = new CharacterController;
	m_characterController->Init(0.1f, 0.1f, m_position);
	m_characterController->SetGravity(0.0f);
	//���f�������[�h�B
	if (modelData == NULL) {

		modelData = new SkinModelData;
		modelData->LoadModelData("Assets/modelData/Bullet2.X", NULL);
	}

	m_rotation = game->GetPlayer()->GetRot(); //�v���C���[�̉�]
	model.Init(modelData);
	model.SetLight(&m_light);

	
	

	life = 120;

	m_BulletSe = new CSoundSource;
	m_BulletSe->Init("Assets/Sound/SE/BulletSound2.wav");
	m_BulletSe->SetVolume(0.2f);
	SParicleEmitParameter SparticleEmit;
	SparticleEmit.texturePath = "Assets/Particle/P_Fire.png";
	SparticleEmit.w = 10.0f;
	SparticleEmit.h = 10.0f;
	SparticleEmit.intervalTime = 0.02f;
	SparticleEmit.timer = 1.0f;

	SparticleEmit.initSpeed = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_particleEmit = std::make_unique<CParticleEmitter>();
	m_particleEmit->Init(SparticleEmit);

	//m_BulletSe->Play(0);
}
bool Bullet::Update()
{
	switch (m_state)
	{

	case e_Update:
		life--;
	/*	if (m_characterController->IsHitWall())
		{
			MessageBox(NULL, "����������", "IsHit", MB_OK);
		}*/

		if (life < 0 || m_characterController->IsHitWall() || IsHit) {

			//���S�B
			if (IsHit)
			{
				m_BulletSe->Init("Assets/Sound/SE/HitSound.wav");
				m_BulletSe->SetVolume(0.1f);
				m_BulletSe->Play(0);

			}

			m_characterController->RemoveRigidBoby();

			m_state = e_IsHit;
			
			//m_particleEmit->Update();
		}

		else
		{
			m_position = moveSpeed * 120.0f;
			m_characterController->SetMoveSpeed(m_position);
			m_characterController->Execute();
			m_position = m_characterController->GetPosition();

		}
		//model.UpdateWorldMatrix(m_position, m_rotation, D3DXVECTOR3(0.3f, 0.3f, 0.3f));
		return true;
		break;

	case e_IsHit:
		m_time += GameTime().GetFrameDeltaTime();
		m_particleEmit->SetPosition({ m_position.x,m_position.y,m_position.z });
		m_particleEmit->Update();

		if (m_time > 3.00f)
		{
			m_state = e_IsDete;
		}
		
		
		break;

	case e_IsDete:

		delete this;
		return false;
		break;

	}
}
void Bullet::LockOn(D3DXVECTOR3 lock)
{
	D3DXVECTOR3 old = lock - moveSpeed;
	D3DXVec3Normalize(&old, &old);
	moveSpeed = old *0.8f;

}

void Bullet::Render()
{
	switch (m_state)
	{

	case e_Update:
		if (this->life < 0) { return; }

		model.SetShadowMap(false);
		model.SetShadowRecieve(false);
		model.UpdateWorldMatrix(m_position, m_rotation, D3DXVECTOR3(0.1f, 0.1f, 0.1f));
		model.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
		break;

	case e_IsHit:
		//m_particleEmit->Update();
		m_particleEmit->Render(game->GetGameCamara()->Getcamera()->GetViewMatrix(), game->GetGameCamara()->Getcamera()->GetProjectionMatrix());

		break;

	case e_IsDete:

		break;
	}


}

void Bullet::Render2D()
{
	switch (m_state)
	{

	case e_Update:

		break;

	case e_IsHit:

		//m_particleEmit->Render(game->GetGameCamara()->Getcamera()->GetViewMatrix(), game->GetGameCamara()->Getcamera()->GetProjectionMatrix());

		break;

	case e_IsDete:

		break;
	}
}

void Bullet::LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix)
{

	switch (m_state)
	{

	case e_Update:
	model.SetShadowMap(false);
	model.SetShadowRecieve(false);
	model.UpdateWorldMatrix(m_position, m_rotation, D3DXVECTOR3(0.3f, 0.3f, 0.3f));
	//model.Draw(lightViewMatrix, lightProjMatrix);
	break;

	case e_IsHit:

		break;

	case e_IsDete:

		break;

	}


}



