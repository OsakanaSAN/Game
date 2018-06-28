#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "myEngine/Sound/CSoundSource.h"
#include "myEngine/Physics/SphereCollider.h"
#include "myEngine/Graphics/Particle/ParticleEmitter.h"
#include "Character/Enemy.h"


enum BulletState
{
	e_Update,
	e_homingUpdate,
	e_IsHit,
	e_IsDete,

};
class Bullet
{
public:
	Bullet();
	~Bullet();
	void Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed);
	void homingStart(CEnemy&  enemy, const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed);
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
		m_IsHit = hit;
	}
	bool GetIsHit()
	{
		return m_IsHit;
	}
	bool GetFree()
	{
		return m_free;
	}
	void LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX& lightProjMatrix);
	void LockOn(D3DXVECTOR3 lock);
	

private:
	static SkinModelData*	modelData;
	Pad						BPad;
	SkinModel				model;
	D3DXMATRIX				Mat;
	Animation				animation;
	Light					m_light;
	CSoundSource*			m_BulletSe;
	SphereCollider*			m_collider;
	RigidBody*				m_rigidbody;
	CharacterController*	m_characterController;
	CParticleEmitter* 		m_particleEmit[2];
	BulletState				m_state;
	SParicleEmitParameter	m_SparticleEmit;
	D3DXQUATERNION			m_rotation;
	D3DXVECTOR3				m_position;
	D3DXVECTOR3				moveSpeed;
	D3DXVECTOR3				PlayerFrontPosition;
	CEnemy*                  m_homingTarget = nullptr;

	
	float					m_time;
	float					m_life;		//éıñΩÅB
	bool					IsLifeDown = false;	
	bool					m_IsHit = false;
	bool					m_free = false;	//égÇÌÇÍÇƒÇ¢Ç»Ç¢Ç©ÇÃîªíË

};