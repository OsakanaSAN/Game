#pragma once
#include "IGameObject.h"
#include "myEngine\Physics\CharacterController.h"
#include "myEngine\HID\Pad.h"
#include "Bullet.h"
class Enemy 
{
public:
	Enemy();
	~Enemy();
	void Start(D3DXVECTOR3 pos);
	void Update();
	void Render();
	void EndEnemy();
	void EnemyBulletON();
	void LightEyePosRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix);
	D3DXVECTOR3 Getpos()
	{
		return position;
	}
	void EnemyMove();

private:

		  SkinModel             skinmodel;
		  SkinModelData         skinmodelData;
		  Light                 PLight;
		  D3DXVECTOR3           position = { 0.0f,1000.0f,10.0f };
		  D3DXQUATERNION        rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		  Animation             animation;    //�A�j���[�V����
		  CharacterController   characterController;
		  Camera*               camera;
		  bool                  IsDete;
		  Pad                   pad;          //�p�b�g
		  int                   HP = 500;
		  int bulletFireInterval;	//�e�ۂ�������x�łĂ�悤�ɂȂ�܂ł̃C���^�[�o��
		  int                   tactics = 2; //��p
		  D3DXVECTOR3           TimeVec;
		  int                   coolTime = 0;
		  int                   tacticsChange;
		  bool                  MoveX = false;
		  D3DXVECTOR3			MoveSpeed;

};

