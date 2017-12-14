#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "myEngine\Physics\CharacterController.h"
#include "myEngine\HID\Pad.h"
#include "Item/Bullet.h"
class CEnemy 
{
public:
	CEnemy();
	~CEnemy();
	void Start();
	void Update();
	void Render();
	void EndEnemy();
	//����ł��邩��Ԃ�
	bool IsEnd()
	{
		return m_IsDete;
	}
	void EnemyBulletON();
	void LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix);

	//���ݒn�擾
	D3DXVECTOR3 Getpos()
	{
		return m_Position;
	}
	
	//���ݒn�ݒ�
	void SetPos(D3DXVECTOR3 SetPosition)
	{
		m_Position = SetPosition;
	}

	void EnemyMove();

private:

		  SkinModel             m_Skinmodel;
		  SkinModelData         m_SkinmodelData;
		  Light                 m_Light;
		  D3DXVECTOR3           m_Position = { 0.0f,50.0f,10.0f };

		  D3DXQUATERNION        m_Rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		  Animation             m_Animation;    //�A�j���[�V����
		  CharacterController   m_CharacterController;
		  Camera*               m_Camera;
		  bool                  m_IsDete;
		  Pad                   m_Pad;          //�p�b�g
		  int                   m_HP = 500;     //�G��HP
		  int					m_BulletFireInterval;	//�e�ۂ�������x�łĂ�悤�ɂȂ�܂ł̃C���^�[�o��
		  int                   m_Tactics = 2; //��p
		  D3DXVECTOR3           m_TimeVec;
		  int                   m_CoolTime = 0;
		  int                   m_TacticsChange;
		  bool                  m_MoveX = false;
		  D3DXVECTOR3			m_MoveSpeed;
		  float					m_BulletTime = 0.0f; //���e���_����

};

