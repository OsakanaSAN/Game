#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "myEngine\Graphics\SkinModel.h"
#include "myEngine\HID\Pad.h"
#include "myEngine\Physics\CharacterController.h"
#include "myEngine\Graphics\Camera.h"
#include "Item/Bullet.h"
#include "myEngine/Sound/CSoundSource.h"
#include "myEngine/Graphics/Particle/Particle.h"
#include "myEngine/Graphics/Particle/ParticleEmitter.h"
#include "HUD/GUI.h"


struct EenemyBoX {
	D3DXVECTOR3 Position;
	int         Number;

};

class CPlayer : public IGameObject
{
public:
	CPlayer();
	~CPlayer();
	void Start();	//�v���C���[�̏������֐�
	void Update();	//�v���C���[�̃A�b�v�f�[�g�֐�
	void Render();	//�v���C���[�̕`��֐�
	void LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix); //�e�`��p�֐�
	void MovePlayer();		//�v���C���̈ړ��̏���������֐�
	void OnLock();			//���b�N�I�������֐� 
	void InitBullet();		//�e�̐����֐�
	void TransformAngle();		//�v���C���[�̉�]�̏����֐�
	const D3DXVECTOR3& InitBootht(D3DXVECTOR3& MoveSpeed); //�u�[�X�g�̏����֐�

	//�Q�b�^�[
	const D3DXVECTOR3& GetPos()
	{
		return m_Position;
	}
	//�Z�b�^�[
	const void SetPos(D3DXVECTOR3& pos)
	{
		m_Position = pos;
	}
	//�u�[�X�Ǝ��Ԏ擾
	float GetBoothtTime()
	{
		return m_DashTime;
	}
	//���[���h�s��̎擾
	const D3DXMATRIX& GetWorldMatrix()
	{
		return m_Skinmodel.GetWorldMatrix();
	}
	//��]�s��̎擾
	const D3DXQUATERNION& GetRot()
	{
		return m_Rotation;
	}
	//�u�[�X�g�{�^����������Ă��邩��Ԃ�
	//������ĂȂ����false��Ԃ��B
	bool GetNotBootht()
	{
		return m_NoBoothtInput;
	}
	//���݂̃v���C���[�̑��x���擾
	int GetMoveSpeed()
	{
		return m_MoveSpeed;
	}
	//�A�j���[�V�����𔻒肷��֐�
	void Animetion();

	//�ő�HP���擾����
	int GetMaxHp()
	{
		return m_MaxHp;
	}
	//���݂�HP���擾
	int GetHp()
	{
		return m_Hp;
	}
	//Zoom�u���[�������邩�̔����Ԃ�
	bool GetZoomBlur()
	{
		return m_ZoomBlurOn;
	}
	bool IsPlayer()
	{
		return m_Isplayer;
	}

private:
	D3DXMATRIX				m_Mat;
	D3DXVECTOR3				m_FrontPosition;
	CGUI*					m_TagetUI;									//���b�N�I������UI
	int						m_MaxHp = 200;								//�v���C���[�̍ő�HP
	int						m_Hp	= 200;								//�v���C���[�̌��݂�HP
	CParticleEmitter		m_ParticleEmitter;							//�p�[�e�B�N��
	SkinModel				m_Skinmodel;								//�X�L�����f��
	SkinModelData			m_SkinmodelData;							//�X�L�����f���f�[�^�[
	Light					m_Light;									//�v���C���[�̃��C�g
	D3DXVECTOR3				m_Position = { 0.0f,50.0f,0.0f };			//�v���C���[�̌��݈ʒu	
	D3DXQUATERNION			m_Rotation = { 0.0f, 0.0f, 0.0f, 1.0f };	//��]
	CharacterController		m_CharacterController;						//�L�����N�^�[�R���g���[���[
	CSoundSource*			m_PlayerSE;									//�v���C���[��SE
	float					m_BoothtTime;								//�u�[�X�g�̎��Ԃ��i�[
	int						m_MoveSpeed = 0;							//�L�����N�^�[�̈ړ����x
	bool					m_NoBoothtInput;							/*�u�[�X�Ǝ��Ԃ̏���𒴂��Ă�R2�����������Ă��鎞��
																		  �؂�ւ�肪��������̂�h������*/
	Animation				m_Animation;								//�A�j���[�V����
	Pad						m_Pad;										//�p�b�g
	int						m_AnimationNo;								//���݂̃A�j���[�V�����ԍ�
	bool					m_IsMove = false;							//�ړ������̔���
	float					m_bulletIntervalTime;						//�e�ۂ�������x�łĂ�悤�ɂȂ�܂ł̃C���^�[�o��
	float					m_DashTime = 0;								//���݂̃u�[�X�Ǝ��Ԃ��i�[
	bool					m_ZAttent = false;							//�G�ɒ��ڂ��邩�̔���
	float					m_Inertia = 6;								//����
	float					m_X_input = 0;								//x���̓��͗ʕۊ�
	float					m_Y_input = 0;								//y���̓��͗ʕۊ�
	float					m_X_input_old = 0;							//�ߋ���x���̓��͗ʕۊ�
	float					m_Y_input_old = 0;							//�ߋ���y���̓��͗ʕۊ�
	bool					m_Oveline = false;
	bool					m_BattleMotion = false;					//�U����

/////////////////////////////////////////////////////////////////////////////////////
	//�G�l�~�[�ɒ��ڂ��邽�߂̕ϐ�
	D3DXVECTOR3				m_Enemypos;
	int						m_EnemyNo = 0;
	int						m_MaxEnemy = 1;
	bool					m_NoEenemy = false;						//�G�l�~�[�����݂��Ă��邩�̔���
	EenemyBoX				m_EnemyBox[30];
/////////////////////////////////////////////////////////////////////////////////////
	bool					m_ZoomBlurOn = false;					//Zoom�u���[���I���ɂ���
	float					m_Radian = 0.0f;						//�v���C���[���X���鏈��
	bool					m_siyakakuON = false;
	bool					m_Isplayer = false;
	
};

