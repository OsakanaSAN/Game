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
	SkinModel				m_model;				//�X�L�����f��
	D3DXMATRIX				m_Mat;
	SkinModelData*			m_modelData;			//�X�L�����f���f�[�^�[	
	Light					m_light;				//���C�g
	D3DXVECTOR3				m_position;				//���W
	D3DXVECTOR3				m_TagPos;				//�ڕW���̍��W
	D3DXVECTOR3				moveSpeed;				//����
	D3DXVECTOR3				PlayerFrontPosition;
	D3DXQUATERNION			m_rotation;				//��]
	CSoundSource*			m_BulletSe;				//�T�E���h�G�t�F�N�g
	SphereCollider*			m_collider;				//�R���C�_�[
	RigidBody*				m_rigidbody;			//����
	CharacterController*	m_characterController;  //�L�����N�^�[�R���g���[���[
	CParticleEmitter*		m_particleEmit;			//�p�[�e�B�N��
	MissileState			m_state;
	float					m_time;

	bool IsLifeDown = false;
	int life;		//�����B
	bool IsHit = false;
};

