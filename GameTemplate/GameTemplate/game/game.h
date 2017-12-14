/*!
 * @brief	ゲームクラス。
 */

#pragma once

#include "myEngine/Physics/Physics.h"
#include "Character/Player.h"
#include "Character/Enemy.h"
#include "myEngine/HID/Pad.h"
#include "Map/Map.h"
#include "Item/Bullet.h"
#include <list>
#include "myEngine/Graphics/Sprite/Primitive.h"
#include "GameCamera/GameCamera.h"
#include "myEngine/Graphics/Sprite/Sprite.h"
#include "HUD/Hud.h"
#include "myEngine/Graphics/Sprite/C2DImage.h"
#include "Scene/TITLE.h"
#include "Fade/Fade.h"
#include "myEngine/GameManager/GameObjectManager.h"
#include "myEngine/Graphics/PostEffect/Bloom.h"

 /*!
 * @brief	ゲームクラス。
 */
class Game{
public:
	/*!
	 * @brief	コンストラクタ。
	 */
	Game();
	/*!
	 * @brief	デストラクタ。
	 */
	~Game();
	/*!
	 * @brief	ゲームが起動してから一度だけ呼ばれる関数。
	 */
	void Start();
	/*!
	 * @brief	更新。
	 */
	void Update();
	/*!
	 * @brief	3Dモデルの描画。
	 */
	void Render();

	//			2Dの描画
	void Render2D();

	Camera* GetCamera()
	{
		return Gamecamera.Getcamera();
	}
	GameCamera* GetGameCamara()
	{
		return &Gamecamera;
	}
	Pad* GetPad()
	{
		return &pad;
	}
	CEnemy* GetEnemy()
	{
		return &enemy;
	}
	//プレイヤーの弾のリスト
	const std::list<Bullet*>& GetPlayerBullet()
	{
		return PlayerBullets;
	}
	//敵の弾のリスト
	const std::list<Bullet*>& GetEnemyBullet()
	{
		return EnemyBullets;
	}
	//エネミーリスト
	const std::list<CEnemy*>& GetEnemys()
	{
		return Enemys;
	}
	/*!
	* @brief	プレイヤーが放った弾を追加。
	*/
	void AddPlayerBullets(Bullet* bullet)
	{
		PlayerBullets.push_back(bullet);
	}

	void AddEnemyBullets(Bullet* bullet)
	{
		EnemyBullets.push_back(bullet);
	}

	void AddEnemy(CEnemy* enemy)
	{
		Enemys.push_back(enemy);
	}
	//現在のマップを取得
	Map* GetMap()
	{
		return &map;
	}
	//現在のプレイヤーを取得
	CPlayer* GetPlayer()
	{
		return &player;
	}
	CFade* GetFade()
	{
		return &g_fade;
	}
	void SetGameScene()
	{
		Scene = Game_Scene;
	}
	//敵を生成するための関数
	void InitEnemy(D3DXVECTOR3 Setpos) 
	{
		CEnemy* enemy = new CEnemy;
		Enemys.push_back(enemy);
		enemy->Start();
		enemy->SetPos(Setpos);
	}
private:
	enum SceneState {
		Title_Scene,	//タイトル画面
		Game_Scene,		//ゲーム画面
		Result_Scene	//リザルト画面に移行
	};

	
	D3DXVECTOR3		toEyePos;		//注視点から視点までのベクトル。
	D3DXVECTOR3		oldPos;
	GameCamera		Gamecamera;		//カメラ
	CPlayer			player;					//プレイヤー
	CEnemy			enemy;					//敵
	Pad				pad;						//パッド
	Map				map;						//マップ
	CHud			Hud;						//HUD
	TITLE			title;					//タイトル画面
	CFade			g_fade;
	

	CPrimitive prim;				//プリミティブ
	Sprite*    sprite;				//スプライト

	LPD3DXSPRITE spt;
	D3DXVECTOR3  rag;

	std::list<Bullet*> PlayerBullets;			//プレイヤーの弾のリスト
	std::list<Bullet*> EnemyBullets;			//敵の弾のリスト
	std::list<CEnemy*>  Enemys;					//敵のリスト
	GameObjectManager  GameManager;				//ゲームマネージャー
	SceneState         Scene = Title_Scene;		//シーンの管理変数

};

extern Game* game;