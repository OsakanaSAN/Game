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
	void LockOn();     //ロックオン処理 
	void BulletInit(); //弾の生成
	void ChangeAngle(); //プレイヤーの向きの処理
	D3DXVECTOR3 BoothtInit(D3DXVECTOR3 MoveSpeed); //ブーストの処理


	D3DXVECTOR3 GetPos()
	{
		return position;
	}
	/*ブースと時間取得*/
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
	bool				  NoBoothtInput = false; //ブースと中かの判定
	int                   MOVESpeed = 0;         //速さ
	

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
	bool                  Battle_Motion = false; //攻撃中

/////////////////////////////////////////////////////////////////////////////////////
	//エネミーに注目するための変数
	D3DXVECTOR3           Enemypos[3];
	bool                  EnemyIt[3];
	int                   EnemyNo = 0;
	int                   LockNo = 0;
	bool                  NoEenemy = false;
/////////////////////////////////////////////////////////////////////////////////////
	
};

