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
	//死んでいるかを返す
	bool IsEnd()
	{
		return m_IsDete;
	}
	void EnemyBulletON();
	void LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix);

	//現在地取得
	D3DXVECTOR3 Getpos()
	{
		return m_Position;
	}
	
	//現在地設定
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
		  Animation             m_Animation;    //アニメーション
		  CharacterController   m_CharacterController;
		  Camera*               m_Camera;
		  bool                  m_IsDete;
		  Pad                   m_Pad;          //パット
		  int                   m_HP = 500;     //敵のHP
		  int					m_BulletFireInterval;	//弾丸をもう一度打てるようになるまでのインターバル
		  int                   m_Tactics = 2; //戦術
		  D3DXVECTOR3           m_TimeVec;
		  int                   m_CoolTime = 0;
		  int                   m_TacticsChange;
		  bool                  m_MoveX = false;
		  D3DXVECTOR3			m_MoveSpeed;
		  float					m_BulletTime = 0.0f; //次弾争点時間

};

