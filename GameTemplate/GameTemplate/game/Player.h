#pragma once
#include "IGameObject.h"
#include "myEngine\Graphics\SkinModel.h"
#include "myEngine\HID\Pad.h"
#include "myEngine\Physics\CharacterController.h"
#include "myEngine\Graphics\Camera.h"
#include "Bullet.h"

class Player : public IGameObject
{
public:
	Player();
	~Player();
	void Start();
	void Update();
	void Render();
	void LightEyePosRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix);
	void PlayerAnimation();
	void AnglePlayer();
	void LockOn();     //���b�N�I������ 
	void BulletInit(); //�e�̐���
	void ChangeAngle(); //�v���C���[�̌����̏���
	D3DXVECTOR3 BoothtInit(D3DXVECTOR3 MoveSpeed); //�u�[�X�g�̏���


	D3DXVECTOR3 GetPos()
	{
		return position;
	}
	/*�u�[�X�Ǝ��Ԏ擾*/
	float GetBoothtTime()
	{
		return BoothtTime;
	}

	D3DXMATRIX GetWorldMatrix()
	{
		return skinmodel.GetWorldMatrix();
	}
	D3DXQUATERNION GetRot()
	{
		return rotation;
	}
	bool GetNotBootht()
	{
		return NoBoothtInput;
	}
	int GetMoveSpeed()
	{
		return MOVESpeed;
	}
	void SetEnemypos(D3DXVECTOR3 epos)
	{/*
		if (EnemyIt[EnemyNo])
		{

			Enemypos[EnemyNo] = epos;

		}

		EnemyNo++;
		if (EnemyNo == 3)
		{
			EnemyNo = 0;
		}*/
	}
	void EnemyDet()
	{
		EnemyIt[LockNo] = false;
	}

	
	

	void Animetion();
private:
	SkinModel             skinmodel;
	SkinModelData         skinmodelData;
	Light                 PLight;
	D3DXVECTOR3           position = { -650.0f,460.0f,130.0f };
	D3DXVECTOR3           MAEposition = { 1.0f,0.0f,0.0f };
	D3DXVECTOR3           Bespos;
	D3DXQUATERNION        rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	float				  BoothtTime;
	bool				  NoBoothtInput = false; //�u�[�X�ƒ����̔���
	int                   MOVESpeed = 0;         //����
	

	Animation             animation;    //�A�j���[�V����
	Pad                   pad;          //�p�b�g
	int                   AnimationNo;  //���݂̃A�j���[�V�����ԍ�
	Camera*               camera;       //�J����
	bool                  IsMove = false;
	CharacterController   characterController;
	int bulletFireInterval;	//�e�ۂ�������x�łĂ�悤�ɂȂ�܂ł̃C���^�[�o��
	D3DXVECTOR3           oldDir;
	int                   DashTime = 0;
	float                 SpeedDown = 1.0f;
	bool                  ZAttent = false;
	bool                  keyOn = false;
	float                 inertia = 6; //����
	float                 X_input = 0; //x���̓��͗ʕۊ�
	float                 Y_input = 0; //y���̓��͗ʕۊ�
	float                 X_input_old = 0; //�ߋ���x���̓��͗ʕۊ�
	float                 Y_input_old = 0; //�ߋ���y���̓��͗ʕۊ�
	bool                  oveline = false;
	bool                  Battle_Motion = false; //�U����

/////////////////////////////////////////////////////////////////////////////////////
	//�G�l�~�[�ɒ��ڂ��邽�߂̕ϐ�
	D3DXVECTOR3           Enemypos[3];
	bool                  EnemyIt[3];
	int                   EnemyNo = 0;
	int                   LockNo = 0;
	bool                  NoEenemy = false;
/////////////////////////////////////////////////////////////////////////////////////
	
};

