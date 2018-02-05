#include "stdafx.h"
#include "Bullet.h"
#include "game.h"

SkinModelData* Bullet::modelData = NULL;



Bullet::Bullet()
{
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	moveSpeed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_BulletSe = new CSoundSource;
	m_BulletSe->Init("Assets/Sound/SE/BulletSound2.wav");
	m_BulletSe->SetVolume(0.2f);
	
	
}


Bullet::~Bullet()
{
	m_BulletSe->Release();
	delete modelData;
	modelData = NULL;
}
void Bullet::Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed)
{
	m_position = pos;
	PlayerFrontPosition = moveSpeed;
	D3DXVec3Normalize(&PlayerFrontPosition,&PlayerFrontPosition);
	
	this->moveSpeed = PlayerFrontPosition ;//1.5f; //弾速調整
	//ライトを初期化。
	light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	light.SetDiffuseLightColor(0, D3DXVECTOR4(1.0f, 1.0f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(1, D3DXVECTOR4(1.0f, 1.0f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(2, D3DXVECTOR4(1.0f, 1.0f, 0.2f, 1.0f));
	light.SetDiffuseLightColor(3, D3DXVECTOR4(1.0f, 1.0f, 0.2f, 1.0f));
	light.SetAmbientLight(D3DXVECTOR4(10.0f, 10.0f, 5.0f, 1.0f));
	
	/*m_collider = new SphereCollider;
	m_rigidbody = new RigidBody;

	m_collider->Create(1.0f);
	RigidBodyInfo rbInfo;
	rbInfo.collider = m_collider;
	rbInfo.localInertia = { 1.0f, 1.0f, 1.0f };
	rbInfo.mass = 0.0f;
	rbInfo.pos = m_position;
	rbInfo.rot = m_rotation;
	m_rigidbody->Create(rbInfo);
	g_physicsWorld->AddRigidBody(m_rigidbody);*/
 
	m_characterController = new CharacterController;
	m_characterController->Init(0.3f, 0.3f, m_position);
	m_characterController->SetGravity(0.0f);
	if (modelData == NULL) {
		//モデルをロード。
		modelData = new SkinModelData;
		modelData->LoadModelData("Assets/modelData/Bullet2.X", NULL);
	}
	m_rotation = game->GetPlayer()->GetRot(); //プレイヤーの回転
	model.Init(modelData);
	model.SetLight(&light);
	life = 120;
	//m_BulletSe->Play(0);
}
bool Bullet::Update()
{
	life--;
	//if (m_characterController->IsHitWall())
	//{
	//	MessageBox(NULL, "当たったよ", "IsHit", MB_OK);
	//}

	 if (life < 0 || m_characterController->IsHitWall() || IsHit /*|| m_characterController->IsOnGround()*/) {
		
		//死亡。
		if (IsHit)
		{
			m_BulletSe->Init("Assets/Sound/SE/HitSound.wav");
			m_BulletSe->SetVolume(0.1f);
			m_BulletSe->Play(0);
			
			
		}

	
		m_characterController->RemoveRigidBoby();
		return false;
	 }
	

	m_position = moveSpeed * 50.0f;
	m_characterController->SetMoveSpeed(m_position);
	m_characterController->Execute();
	m_position = m_characterController->GetPosition();
	

	model.UpdateWorldMatrix(m_position, m_rotation, D3DXVECTOR3(0.3f, 0.3f, 0.3f));
	return true;
}
void Bullet::LockOn(D3DXVECTOR3 lock)
{
	D3DXVECTOR3 old = lock - moveSpeed;
	D3DXVec3Normalize(&old, &old);
	moveSpeed = old *0.8f;

}

void Bullet::Render()
{
	if (this->life < 0) { return; }

	model.SetShadowMap(false);
	model.SetShadowRecieve(true);
	
	model.Draw(&game->GetCamera()->GetViewMatrix(), &game->GetCamera()->GetProjectionMatrix());
}

void Bullet::LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix)
{
	model.SetShadowMap(true);
	model.SetShadowRecieve(false);
	model.UpdateWorldMatrix(m_position, m_rotation, D3DXVECTOR3(0.3f, 0.3f, 0.3f));
	//model.Draw(lightViewMatrix, lightProjMatrix);

}



