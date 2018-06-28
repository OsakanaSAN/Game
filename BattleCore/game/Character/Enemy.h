#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "myEngine\Physics\CharacterController.h"
#include "myEngine\HID\Pad.h"
#include "Item/Bullet.h"
#include "HUD/GUI.h"





struct AdjacentRoute
{
	D3DXVECTOR3  m_RoutePos = {0.0f,0.0f,0.0f};
	int			 m_RouteNumber = 0;
};


class CEnemy 
{
public:
	CEnemy();
	~CEnemy();
	void Start();
	void Update();
	void Render();
	void EndEnemy();
	//Group�ł��邩��Ԃ�
	bool GetIsGroup()
	{
		return m_group;
	}
	//Group�ɂ��邩�̐ݒ�

	void setGroup(bool set)
	{
		m_group = set;
	}

	bool GetIsCountDown()
	{
		return m_CountDown;
	}
	//Group�ɂ��邩�̐ݒ�

	void setCountDown(bool set)
	{
		m_CountDown = set;
	}

	//�G�l�~�[��j������
	void Delete()
	{
		delete this;
	}
	//����ł��邩��Ԃ�
	bool IsEnd()
	{
		
		return m_IsDete;
	}
	void EnemyBulletON();
	void LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix);

	//���݈ʒu�擾
	D3DXVECTOR3 Getpos()
	{
		return m_Position;
	}

	void SetRandomPosition(D3DXVECTOR3 setpos);
	
	//���݈ʒu�ݒ�
	void SetPos(D3DXVECTOR3 SetPosition)
	{
		m_Position = SetPosition;
	}

	//Random�|�W�V������ݒ�
	void SetRandPos(D3DXVECTOR3 SetPosition);
	
	void Render2D();

	//�אڌo�H�T��
	void RouteSearch();
	//����_����œK�ȃ��[�g�����߂�֐�
	void BranchRoute();
	//�ړ��̔��������
	void EnemyMove();
	//�~�j�}�b�v�p�̃����_�[�֐�
	void MiniMapRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix);

private:
		enum Enemystate
		{
			e_EnemyUpdate,
			e_EnemyDelte,
			e_EnemyEnd,
		};

		enum e_priority		//�s���̗D��x
		{
			e_Neutral,		//���펞
			e_Danger,			//�G�ɐڋ߂��ꂽ��
			e_Absolutely,		//�_���[�W���󂯂��ꍇ

		};


		CGUI*					m_GUI;						//HP�Q�[�W�̕\��
		CParticleEmitter*		m_particleEmit;				//�p�[�e�B�N��
		SkinModel				m_Skinmodel;				//�X�L�����f��
		static SkinModelData*	m_SkinmodelData;			//�X�L�����f���f�[�^
		SkinModel				m_MapSkinmodel;				//�~�j�}�b�v�p�X�L�����f��
		static SkinModelData*	m_MapSkinmodelData;			//�~�j�}�b�v�p�X�L�����f���f�[�^


		Light					m_Light;					//�G�l�~�[�̃��C�g
		Light					m_minimaplight;
		D3DXVECTOR3				m_Position = { 0.0f,0.0f,0.0f };				//���W
		D3DXVECTOR3				m_MoveSpeed = { 0.0f,0.0f,0.0f };				//�G�l�~�[�̈ړ����x���i�[
		D3DXQUATERNION			m_Rotation = { 0.0f, 0.0f, 0.0f, 1.0f };		//��]
		Animation				m_Animation;				//�A�j���[�V����
		CharacterController		m_CharacterController;		//�L�����N�^�[�R���g���[���[	
		bool					m_IsDete;					//���S����
		float					m_HP = 400.0f;				//�G�̌��݂�HP
		float					m_MaxHp = 400.0f;			//�G��HP�̍ő�l
		bool					m_rando = false;			//�������Đݒ肷�邩�̔���
		float					m_randoTime = 0.0f;			//�e�ۂ�������x�łĂ�悤�ɂȂ�܂ł̃C���^�[�o���𗐐��Ŋi�[
		float					m_BulletTime = 0.0f;		//���e���_����
		int						m_Tactics = 2;				//��p(���݂͎g�p���Ă��Ȃ�)
		bool					m_MoveX = false;
		AdjacentRoute			m_AdjacentRoute[3][3];		//�G�l�~�[�̈ړ����v�Z���邽�߂̕ϐ�
		D3DXVECTOR3				m_SubPosition = { 0.0f,0.0f,0.0f };	//�G�l�~�[�����Ɉړ�����ꏊ�̍��W���i�[
		int						m_XNumber = 0;
		int						m_ZNumber = 0;
		bool					m_group = false;
		Enemystate				m_State = e_EnemyUpdate;
		bool                    m_CountDown = false;

		const float				m_Movelengt = 1300.0f;		//�b��
		bool					m_rootReturen = true;

		float					m_AvoidanceTime =0.0f;			//������s������
		const float				m_changeTime = 2.0f;

		e_priority				m_priority = e_Neutral;


		 

};