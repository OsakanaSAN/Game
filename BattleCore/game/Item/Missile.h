#pragma once

enum MissileState
{
	e_m_Update,
	e_m_IsHit,
	e_m_IsDete,

};
class Missile
{
public:
	Missile();
	~Missile();
	void Start(const D3DXVECTOR3 &pos, const D3DXQUATERNION &rot,const D3DXVECTOR3 tagpos);
	bool Update();
	void Render();

private:
	SkinModel				m_model;				//スキンモデル
	D3DXMATRIX				m_Mat;
	SkinModelData*			m_modelData;			//スキンモデルデーター	
	Light					m_light;				//ライト
	D3DXVECTOR3				m_position;				//座標
	D3DXVECTOR3				m_TagPos;				//目標物の座標
	D3DXVECTOR3				moveSpeed;				//速さ
	D3DXVECTOR3				PlayerFrontPosition;
	D3DXQUATERNION			m_rotation;				//回転
	CSoundSource*			m_BulletSe;				//サウンドエフェクト
	SphereCollider*			m_collider;				//コライダー
	RigidBody*				m_rigidbody;			//剛体
	CharacterController*	m_characterController;  //キャラクターコントローラー
	CParticleEmitter*		m_particleEmit;			//パーティクル
	MissileState			m_state;
	float					m_time;

	bool IsLifeDown = false;
	int life;		//寿命。
	bool IsHit = false;
};

