#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "myEngine\Graphics\SkinModel.h"
#include "myEngine\HID\Pad.h"
#include "myEngine\Physics\CharacterController.h"
#include "myEngine\Graphics\Camera.h"
#include "Item/Bullet.h"
#include "myEngine/Sound/CSoundSource.h"
#include "myEngine/Graphics/Particle/Particle.h"
#include "myEngine/Graphics/Particle/ParticleEmitter.h"

struct EenemyBoX {
	D3DXVECTOR3 Position;
	int         Number;

};

class CPlayer : public IGameObject
{
public:
	CPlayer();
	~CPlayer();
	void Start();	//プレイヤーの初期化関数
	void Update();	//プレイヤーのアップデート関数
	void Render();	//プレイヤーの描画関数
	void LightEyePosRender(D3DXMATRIX&  lightViewMatrix, D3DXMATRIX&	lightProjMatrix); //影描画用関数
	void PlayerAnimation();	//プレイヤーのアニメーション関数
	void MovePlayer();		//プレイヤの移動の処理をする関数
	void OnLock();			//ロックオン処理関数 
	void InitBullet();		//弾の生成関数
	void TransformAngle();		//プレイヤーの回転の処理関数
	const D3DXVECTOR3& InitBootht(D3DXVECTOR3& MoveSpeed); //ブーストの処理関数
	//現在地を取得
	const D3DXVECTOR3& GetPos()
	{
		return m_Position;
	}
	const void SetPos(D3DXVECTOR3& pos)
	{
		m_Position = pos;
	}
	//ブースと時間取得
	float GetBoothtTime()
	{
		return m_BoothtTime;
	}

	const D3DXMATRIX& GetWorldMatrix()
	{
		return m_Skinmodel.GetWorldMatrix();
	}
	//回転行列の取得
	const D3DXQUATERNION& GetRot()
	{
		return m_Rotation;
	}
	//ブーストボタンを押されているかを返す
	//押されてなければfalseを返す。
	bool GetNotBootht()
	{
		return m_NoBoothtInput;
	}
	//現在のプレイヤーの速度を取得
	int GetMoveSpeed()
	{
		return m_MoveSpeed;
	}
	void Animetion();

private:


	CParticleEmitter		m_ParticleEmitter;

	SkinModel				m_Skinmodel;								//スキンモデル
	SkinModelData			m_SkinmodelData;							//スキンモデルデーター
	Light					m_Light;									//プレイヤーのライト
	D3DXVECTOR3				m_Position = { 0.0f,60.0f,0.0f };			//プレイヤーの現在位置	
	D3DXQUATERNION			m_Rotation = { 0.0f, 0.0f, 0.0f, 1.0f };	//回転
	CharacterController		m_CharacterController;						//キャラクターコントローラー
	CSoundSource*			m_PlayerSE;									//プレイヤーのSE

	float				  m_BoothtTime;								//ブーストの時間を格納
	int					  m_MoveSpeed = 0;							//速さ
	bool				  m_NoBoothtInput;
	Animation             m_Animation;								//アニメーション
	Pad                   m_Pad;									//パット
	int                   m_AnimationNo;							//現在のアニメーション番号
	Camera*               m_Camera;									//カメラ
	bool                  m_IsMove = false;

	int					  m_bulletFireInterval;						//弾丸をもう一度打てるようになるまでのインターバル
	float                 m_DashTime = 0;							//
	float                 m_SpeedDown = 1.0f;
	bool                  m_ZAttent = false;
	float                 m_Inertia = 6;							//慣性
	float                 m_X_input = 0;							//x軸の入力量保管
	float                 m_Y_input = 0;							//y軸の入力量保管
	float                 m_X_input_old = 0;						//過去のx軸の入力量保管
	float                 m_Y_input_old = 0;						//過去のy軸の入力量保管
	bool                  m_Oveline = false;
	bool                  m_BattleMotion = false;					//攻撃中

/////////////////////////////////////////////////////////////////////////////////////
	//エネミーに注目するための変数
	D3DXVECTOR3				m_Enemypos;
	int						m_EnemyNo = 0;
	int						m_MaxEnemy = 1;
	bool					m_NoEenemy = false;						//エネミーが存在しているかの判定
	EenemyBoX				m_EnemyBox[30];
/////////////////////////////////////////////////////////////////////////////////////
	
};

