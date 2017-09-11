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
		  D3DXVECTOR3           position = { 70.0f,1.0f,50.0f };
		  D3DXQUATERNION        rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		  Animation             animation;    //アニメーション
		  CharacterController   characterController;
		  Camera*               camera;
		  bool                  IsDete;
		  Pad                   pad;          //パット
		  int                   HP = 100;
		  int bulletFireInterval;	//弾丸をもう一度打てるようになるまでのインターバル
		  int                   tactics = 2; //戦術
		  D3DXVECTOR3           TimeVec;
		  int                   coolTime = 0;
		  int                   tacticsChange;

};

