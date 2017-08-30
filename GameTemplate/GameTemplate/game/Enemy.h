#pragma once
#include "IGameObject.h"
#include "myEngine\Physics\CharacterController.h"
#include "myEngine\HID\Pad.h"
#include "Bullet.h"
class Enemy : public IGameObject
{
public:
	Enemy();
	~Enemy();
	void Start();
	void Update();
	void Render();
	void EndEnemy();
	void EnemyBulletON();
	D3DXVECTOR3 Getpos()
	{
		return position;
	}
	void EnemyMove();

private:

		  SkinModel             skinmodel;
		  SkinModelData         skinmodelData;
		  Light                 PLight;
		  D3DXVECTOR3           position = { 0.0f,1.0f,4.0f };
		  D3DXQUATERNION        rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		  Animation             animation;    //�A�j���[�V����
		  CharacterController   characterController;
		  Camera*               camera;
		  bool                  IsDete;
		  Pad                   pad;          //�p�b�g
		  int                   HP = 100;
		  int bulletFireInterval;	//�e�ۂ�������x�łĂ�悤�ɂȂ�܂ł̃C���^�[�o��
		  int                   tactics = 2; //��p

};
