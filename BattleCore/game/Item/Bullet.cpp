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
	delete m_particleEmit[0];
	delete m_particleEmit[1];


}
void Bullet::Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed)
{
	//変数の初期化
	{

		IsLifeDown = false;
		m_IsHit = false;
		m_state = e_Update;
		m_life = 2.0f;		//弾が消滅するまでの寿命（秒）

	}
	m_position = pos;
	


	D3DXVec3Dot(&m_position, &moveSpeed);
	PlayerFrontPosition = moveSpeed;
	D3DXVec3Normalize(&PlayerFrontPosition,&PlayerFrontPosition);
	
	this->moveSpeed = PlayerFrontPosition ;//1.5f; //弾速調整
	
	//ライトを初期化。
	//m_light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	//m_light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	//m_light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	//m_light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	//m_light.SetDiffuseLightColor(0, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_light.SetDiffuseLightColor(1, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_light.SetDiffuseLightColor(2, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_light.SetDiffuseLightColor(3, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));

	m_light.SetAmbientLight(D3DXVECTOR4(5.0f, 5.0f, 5.0f, 1.0f));
	
	m_characterController = new CharacterController;
	m_characterController->Init(0.1f, 0.1f, m_position);
	m_characterController->SetGravity(0.0f);
	
	//モデルをロード。
	if (modelData == NULL) {

		modelData = new SkinModelData;
		modelData->LoadModelData("Assets/modelData/Bullet2.X", NULL);
		
	}

	m_rotation = game->GetPlayer()->GetRot(); //プレイヤーの回転
	model.Init(modelData);
	model.SetLight(&m_light);

	
	

	

	m_BulletSe = new CSoundSource;
	m_BulletSe->Init("Assets/Sound/SE/BulletSound2.wav");
	m_BulletSe->SetVolume(0.2f);
	//パーティクルの設定
	{

		m_SparticleEmit.texturePath = "Assets/Particle/P_Fire.png";
		static float w = 2.0f;
		static float h = 2.0f;
		m_SparticleEmit.w = w;
		m_SparticleEmit.h = h;
		m_SparticleEmit.intervalTime = 0.01f;
		m_SparticleEmit.Endtimer = 1.0f;
		m_SparticleEmit.brightness = 0.2f;
		m_SparticleEmit.life = 1.0f;
		m_SparticleEmit.alpha = 1.0f;
		static D3DXVECTOR3 randomMargin = { 1.5f,1.5f,1.5f };
		m_SparticleEmit.initPositionRandomMargin = randomMargin;

		m_SparticleEmit.initSpeed = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		for (int i = 0;i < 2;i++)
		{
			if (m_free == false)
			{

				m_particleEmit[i] = new CParticleEmitter;

			}

			m_particleEmit[i]->Init(m_SparticleEmit);

		}
			m_free = false;

	}
}
void Bullet::homingStart(CEnemy& enemy, const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed)
{
	if (m_homingTarget == nullptr)
	{

		m_homingTarget = new CEnemy;
		m_homingTarget = &enemy;
		
		
	}

	//変数の初期化
	{

		IsLifeDown = false;
		m_IsHit = false;
		m_state = e_homingUpdate;
		m_life = 5.0f;		//弾が消滅するまでの寿命（秒）

	}
	m_position = pos;

	m_position.x += (((float)g_Random->GetRandDouble() - 0.5f) * 2.0f) * 20.0f;
	m_position.y += (((float)g_Random->GetRandDouble() - 0.5f) * 2.0f) * 20.0f;
	m_position.z += (((float)g_Random->GetRandDouble() - 0.5f) * 2.0f) * 20.0f;

	D3DXVec3Dot(&m_position, &moveSpeed);
	PlayerFrontPosition = moveSpeed;
	D3DXVec3Normalize(&PlayerFrontPosition, &PlayerFrontPosition);

	this->moveSpeed = PlayerFrontPosition;//1.5f; //弾速調整
	m_light.SetAmbientLight(D3DXVECTOR4(5.0f, 5.0f, 5.6f, 1.0f));



	m_characterController = new CharacterController;
	m_characterController->Init(0.1f, 0.1f, m_position);
	m_characterController->SetGravity(0.0f);


	//モデルをロード。
	if (modelData == NULL) {

		modelData = new SkinModelData;
		modelData->LoadModelData("Assets/modelData/Bullet2.X", NULL);
	}

	m_rotation = game->GetPlayer()->GetRot(); //プレイヤーの回転
	model.Init(modelData);
	model.SetLight(&m_light);






	m_BulletSe = new CSoundSource;
	m_BulletSe->Init("Assets/Sound/SE/BulletSound2.wav");
	m_BulletSe->SetVolume(0.2f);
	//パーティクルの設定
	{

		m_SparticleEmit.texturePath = "Assets/Particle/P_Fire.png";
		static float w = 2.0f;
		static float h = 2.0f;
		m_SparticleEmit.w = w;
		m_SparticleEmit.h = h;
		m_SparticleEmit.intervalTime = 0.01f;
		m_SparticleEmit.Endtimer = 1.0f;
		m_SparticleEmit.brightness = 0.2f;
		m_SparticleEmit.life = 1.0f;
		m_SparticleEmit.alpha = 1.0f;
		static D3DXVECTOR3 randomMargin = { 1.5f,1.5f,1.5f };
		m_SparticleEmit.initPositionRandomMargin = randomMargin;

		m_SparticleEmit.initSpeed = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
		for (int i = 0;i < 2;i++)
		{
			if (m_free == false)
			{

				m_particleEmit[i] = new CParticleEmitter;

			}

			m_particleEmit[i]->Init(m_SparticleEmit);

		}
		m_free = false;

	}
}

bool Bullet::Update()
{
	switch (m_state)
	{

	case e_Update:
		m_life -= GameTime().GetFrameDeltaTime();
	/*	if (m_characterController->IsHitWall())
		{
			MessageBox(NULL, "当たったよ", "IsHit", MB_OK);
		}*/

		if (m_life <= 0 || m_characterController->IsHitWall() || m_IsHit) {

			//死亡。
			if (m_IsHit)
			{
				m_BulletSe->Init("Assets/Sound/SE/HitSound.wav");
				m_BulletSe->SetVolume(0.1f);
				m_BulletSe->Play(0);

			}

			
			m_characterController->RemoveRigidBoby();
			m_characterController->GetRigidBody()->Release();
			delete m_characterController;
			

			m_state = e_IsHit;
			
			//m_particleEmit->Update();
		}

		else
		{
			m_position = moveSpeed * 8000.0f * GameTime().GetFrameDeltaTime();
			m_characterController->SetMoveSpeed(m_position);
			m_characterController->Execute();
			m_position = m_characterController->GetPosition();

		}
		return true;
		break;

	case e_homingUpdate:

		m_life -= GameTime().GetFrameDeltaTime();
		/*	if (m_characterController->IsHitWall())
		{
		MessageBox(NULL, "当たったよ", "IsHit", MB_OK);
		}*/

		if (m_life <= 0 || m_characterController->IsHitWall() || m_IsHit) {

			//死亡。
			if (m_IsHit)
			{
				m_BulletSe->Init("Assets/Sound/SE/HitSound.wav");
				m_BulletSe->SetVolume(0.1f);
				m_BulletSe->Play(0);

			}

			
			m_characterController->RemoveRigidBoby();
			m_characterController->GetRigidBody()->Release();
			
			delete m_characterController;
			

			m_state = e_IsHit;

			//m_particleEmit->Update();
		}

		else
		{
			m_position = m_homingTarget->Getpos() - m_position ;
			D3DXVec3Normalize(&m_position, &m_position);
			m_position = m_position * 7000.0f * GameTime().GetFrameDeltaTime();
			m_characterController->SetMoveSpeed(m_position);
			m_characterController->Execute();
			m_position = m_characterController->GetPosition();

		}
		return true;


		break;
	case e_IsHit:

		m_time += GameTime().GetFrameDeltaTime();
		for (int i = 0;i < 2;i++)
		{
			//if (m_IsHit) 
			//{

				m_particleEmit[i]->SetPosition({ m_position.x,m_position.y,m_position.z });
				m_particleEmit[i]->Update();

			//}
		}
		//最後に生成されるパーティクルの寿命までは死なない
		if (m_time >m_SparticleEmit.Endtimer + m_SparticleEmit.life)
		{
			m_state = e_IsDete;
			m_time = 0.0f;
			m_free = true;
			delete m_BulletSe;
		}
		
		
		break;

	case e_IsDete:

		int i = 0;
		return true;
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
		//if (this->m_life <= 0) { return; }

		model.SetShadowMap(false);
		model.SetShadowRecieve(false);
		model.UpdateWorldMatrix(m_position, m_rotation, D3DXVECTOR3(0.3f, 0.3f, 0.3f));
		model.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
		break;
	case e_homingUpdate:

		model.SetShadowMap(false);
		model.SetShadowRecieve(false);
		model.UpdateWorldMatrix(m_position, m_rotation, D3DXVECTOR3(0.3f, 0.3f, 0.3f));
		model.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
		break;
	case e_IsHit:
		//m_particleEmit->Update();
		for (int i = 0;i < 2;i++)
		{
			//if (m_IsHit)
			//{

			m_particleEmit[i]->Render(game->GetGameCamara()->Getcamera()->GetViewMatrix(), game->GetGameCamara()->Getcamera()->GetProjectionMatrix());

			//}
		}
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



