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
	void MovePlayer();
	void AnglePlayer();
	void Attention();
	D3DXVECTOR3 GetPos()
	{
		return position;
	}
	D3DXVECTOR3 GetSpeed()
	{
		return MoveSpeed;
	}

	D3DXMATRIX GetWorldMatrix()
	{
		return skinmodel.GetWorldMatrix();
	}

	void Animetion();
private:
	SkinModel             skinmodel;
	SkinModelData         skinmodelData;
	Light                 PLight;
	D3DXVECTOR3           position = { 0.0f,1.0f,0.0f };
	D3DXVECTOR3           MAEposition = { 1.0f,0.0f,0.0f };
	D3DXVECTOR3           Bespos;
	D3DXQUATERNION        rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	D3DXVECTOR3           MoveSpeed;

	Animation             animation;    //アニメーション
	Pad                   pad;          //パット
	int                   AnimationNo;  //現在のアニメーション番号
	Camera*               camera;       //カメラ
	bool                  IsMove = false;
	CharacterController   characterController;
	int bulletFireInterval;	//弾丸をもう一度打てるようになるまでのインターバル
	D3DXVECTOR3           oldDir;
	int                   DashTime = 0;
	float                 SpeedDown = 1.0f;
	bool                  ZAttent = false;
	bool                  keyOn = false;
	float                 inertia = 6; //慣性
	float                 X_input = 0; //x軸の入力量保管
	float                 Y_input = 0; //y軸の入力量保管
	float                 X_input_old = 0; //過去のx軸の入力量保管
	float                 Y_input_old = 0; //過去のy軸の入力量保管
	bool                  oveline = false;
	
};

