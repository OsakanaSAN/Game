#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "myEngine\Physics\CharacterController.h"
#include "myEngine\HID\Pad.h"
#include "Item/Bullet.h"
#include "HUD/GUI.h"




struct AdjacentRoute
{
	D3DXVECTOR3  m_RoutePos = {0.0f,0.0f,0.0f};
	int			 m_RouteNumber = 0;
};

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

	//現在位置取得
	D3DXVECTOR3 Getpos()
	{
		return m_Position;
	}
	
	//現在位置設定
	void SetPos(D3DXVECTOR3 SetPosition)
	{
		m_Position = SetPosition;
	}
	void Render2D();

	//隣接経路探索
	void RouteSearch();
	//分岐点から最適なルートを求める関数
	void BranchRoute();

	void EnemyMove();

private:

		CGUI*					m_GUI;

		CParticleEmitter		m_ParticleEmitter;
		
		SkinModel				m_Skinmodel;
		SkinModelData			m_SkinmodelData;
		Light					m_Light;
		D3DXVECTOR3				m_Position = { 0.0f,50.0f,10.0f };

		D3DXQUATERNION			 m_Rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
		Animation             m_Animation;    //アニメーション
		CharacterController   m_CharacterController;
		Camera*               m_Camera;
		bool                  m_IsDete;
		Pad                   m_Pad;          //パット
		  float                   m_HP = 400.0f;     //敵のHP
		  int					m_BulletFireInterval;	//弾丸をもう一度打てるようになるまでのインターバル
		  int                   m_Tactics = 2; //戦術
		  D3DXVECTOR3           m_TimeVec;
		  int                   m_CoolTime = 0;
		  int                   m_TacticsChange;
		  bool                  m_MoveX = false;
		  D3DXVECTOR3			m_MoveSpeed;
		  float					m_BulletTime = 0.0f; //次弾争点時間
		  AdjacentRoute			m_AdjacentRoute[3][3];
		  D3DXVECTOR3			m_SubPosition = { 0.0f,0.0f,0.0f };//サブターゲット
		  int					m_XNumber;
		  int					m_ZNumber;

};

