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
	//Groupであるかを返す
	bool GetIsGroup()
	{
		return m_group;
	}
	//Groupにするかの設定

	void setGroup(bool set)
	{
		m_group = set;
	}

	bool GetIsCountDown()
	{
		return m_CountDown;
	}
	//Groupにするかの設定

	void setCountDown(bool set)
	{
		m_CountDown = set;
	}

	//エネミーを破棄する
	void Delete()
	{
		delete this;
	}
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

	void SetRandomPosition(D3DXVECTOR3 setpos);
	
	//現在位置設定
	void SetPos(D3DXVECTOR3 SetPosition)
	{
		m_Position = SetPosition;
	}

	//Randomポジションを設定
	void SetRandPos(D3DXVECTOR3 SetPosition);
	
	void Render2D();

	//隣接経路探索
	void RouteSearch();
	//分岐点から最適なルートを求める関数
	void BranchRoute();
	//移動の判定をする
	void EnemyMove();
	//ミニマップ用のレンダー関数
	void MiniMapRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix);

private:
		enum Enemystate
		{
			e_EnemyUpdate,
			e_EnemyDelte,
			e_EnemyEnd,
		};

		enum e_priority		//行動の優先度
		{
			e_Neutral,		//平常時
			e_Danger,			//敵に接近された時
			e_Absolutely,		//ダメージを受けた場合

		};


		CGUI*					m_GUI;						//HPゲージの表示
		CParticleEmitter*		m_particleEmit;				//パーティクル
		SkinModel				m_Skinmodel;				//スキンモデル
		static SkinModelData*	m_SkinmodelData;			//スキンモデルデータ
		SkinModel				m_MapSkinmodel;				//ミニマップ用スキンモデル
		static SkinModelData*	m_MapSkinmodelData;			//ミニマップ用スキンモデルデータ


		Light					m_Light;					//エネミーのライト
		Light					m_minimaplight;
		D3DXVECTOR3				m_Position = { 0.0f,0.0f,0.0f };				//座標
		D3DXVECTOR3				m_MoveSpeed = { 0.0f,0.0f,0.0f };				//エネミーの移動速度を格納
		D3DXQUATERNION			m_Rotation = { 0.0f, 0.0f, 0.0f, 1.0f };		//回転
		Animation				m_Animation;				//アニメーション
		CharacterController		m_CharacterController;		//キャラクターコントローラー	
		bool					m_IsDete;					//死亡判定
		float					m_HP = 400.0f;				//敵の現在のHP
		float					m_MaxHp = 400.0f;			//敵のHPの最大値
		bool					m_rando = false;			//乱数を再設定するかの判定
		float					m_randoTime = 0.0f;			//弾丸をもう一度打てるようになるまでのインターバルを乱数で格納
		float					m_BulletTime = 0.0f;		//次弾争点時間
		int						m_Tactics = 2;				//戦術(現在は使用していない)
		bool					m_MoveX = false;
		AdjacentRoute			m_AdjacentRoute[3][3];		//エネミーの移動を計算するための変数
		D3DXVECTOR3				m_SubPosition = { 0.0f,0.0f,0.0f };	//エネミーが次に移動する場所の座標を格納
		int						m_XNumber = 0;
		int						m_ZNumber = 0;
		bool					m_group = false;
		Enemystate				m_State = e_EnemyUpdate;
		bool                    m_CountDown = false;

		const float				m_Movelengt = 1300.0f;		//秒速
		bool					m_rootReturen = true;

		float					m_AvoidanceTime =0.0f;			//回避を行う時間
		const float				m_changeTime = 2.0f;

		e_priority				m_priority = e_Neutral;


		 

};