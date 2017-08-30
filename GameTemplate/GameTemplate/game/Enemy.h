#pragma once
#include "IGameObject.h"
#include "myEngine\Physics\CharacterController.h"
#include "myEngine\HID\Pad.h"
class Enemy : public IGameObject
{
public:
	Enemy();
	~Enemy();
	void Start();
	void Update();
	void Render();
	void EndEnemy();
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
		  Animation             animation;    //アニメーション
		  CharacterController   characterController;
		  Camera*               camera;
		  bool                  IsDete;
		  Pad                   pad;          //パット

};

