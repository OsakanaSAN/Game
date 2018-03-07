#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "myEngine/Sound/CSoundSource.h"
#include "myEngine/Physics/SphereCollider.h"
#include "myEngine/Graphics/Particle/ParticleEmitter.h"


enum BulletState
{
	e_Update,
	e_IsHit,
	e_IsDete,

};
class Bullet
{
public:
	Bullet();
	~Bullet();
	void Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed);
	bool Update();
	void Render2D();
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
	//íeä€Ç™ìGÇ…îÌíeÇµÇƒÇ¢ÇÈÇ©ÇÃîªíË
	void  SetIsHit(bool hit)
	{
		IsHit = hit;
	}
	bool GetIsHit()
	{
		return IsHit;
	}
	void LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX& lightProjMatrix);
	void LockOn(D3DXVECTOR3 lock);
	

private:
	Pad						BPad;
	SkinModel				model;
	D3DXMATRIX				Mat;
	static SkinModelData*		modelData;
	Animation				animation;
	Light					m_light;
	D3DXVECTOR3				m_position;
	D3DXVECTOR3				moveSpeed;
	D3DXVECTOR3				PlayerFrontPosition;
	D3DXQUATERNION			m_rotation;
	CSoundSource*			m_BulletSe;
	SphereCollider*			m_collider;
	RigidBody*				m_rigidbody;
	CharacterController*	m_characterController;
	CParticleEmitter*		m_particleEmit;
	BulletState				m_state;
	float					m_time;

	bool IsLifeDown = false;
	int life;		//éıñΩÅB
	bool IsHit = false;
};

