#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "myEngine/Sound/CSoundSource.h"
#include "myEngine/Physics/SphereCollider.h"

class Bullet
{
public:
	Bullet();
	~Bullet();
	void Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed);
	bool Update();
	void Render();
	const D3DXVECTOR3& Getops() 
	{
		return m_position;
	}
	const D3DXMATRIX& getMatrix() const
	{
		return Mat;
	}
	const D3DVECTOR& GetSpeed()
	{
		return moveSpeed;
	}
	void SetSpeed(D3DXVECTOR3& speed)
	{
		moveSpeed = speed;
	}
	//弾丸が敵に被弾しているかの判定
	void  SetIsHit(bool hit)
	{
		IsHit = hit;
	}
	void LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX& lightProjMatrix);
	void LockOn(D3DXVECTOR3 lock);
private:
	Pad      BPad;
	SkinModel model;
	D3DXMATRIX  Mat;
	static SkinModelData* modelData;
	Animation animation;
	Light light;
	D3DXVECTOR3				m_position;
	D3DXVECTOR3				moveSpeed;
	D3DXVECTOR3				PlayerFrontPosition;
	D3DXQUATERNION			m_rotation;
	CSoundSource*			m_BulletSe;
	SphereCollider*			m_collider;
	RigidBody*				m_rigidbody;
	CharacterController*		m_characterController;
	

	bool IsLifeDown = false;
	int life;		//寿命。
	bool IsHit = false;
};

