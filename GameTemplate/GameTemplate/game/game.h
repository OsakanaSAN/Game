/*!
 * @brief	ゲームクラス。
 */

#pragma once

#include "myEngine/Physics/Physics.h"
#include "Player.h"
#include "Enemy.h"
#include "myEngine/HID/Pad.h"
#include "Map.h"
#include "Bullet.h"
#include <list>
#include "Primitive.h"
#include "GameCamera.h"
#include "Sprite.h"
#include "Bootht.h"
#include "C2DImage.h"
#include "TITLE.h"
#include "Fade.h"

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
	 * @brief	描画。
	 */
	void Render();

	Camera* GetCamera()
	{
		return Gamecamera.Getcamera();
	}
	Pad* GetPad()
	{
		return &pad;
	}
	Enemy* GetEnemy()
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
	const std::list<Enemy*>& GetEnemys()
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

	void AddEnemy(Enemy* enemy)
	{
		Enemys.push_back(enemy);
	}

	Player* GetPlayer()
	{
		return &player;
	}
	Fade* GetFade()
	{
		return &g_fade;
	}
	void SetGameScene()
	{
		Scene = Game_Scene;
	}
private:
	enum SceneState {
		Title_Scene,	//タイトル画面
		Game_Scene,		//ゲーム画面
	};


	D3DXVECTOR3		toEyePos;		//注視点から視点までのベクトル。
	D3DXVECTOR3    oldPos;
	GameCamera Gamecamera;        //カメラ
	Player player;                //プレイヤー
	Enemy  enemy;                 //敵
	Pad    pad;                   //パッド
	Map    map;                   //マップ
	Bootht Boot;
	TITLE  title;				  //タイトル画面
	Fade   g_fade;
	

	CPrimitive prim;              //プリミティブ
	Sprite*    sprite;            //スプライト

	LPD3DXSPRITE spt;
	D3DXVECTOR3  rag;

	std::list<Bullet*> PlayerBullets;
	std::list<Bullet*> EnemyBullets;
	std::list<Enemy*>  Enemys;


	SceneState         Scene = Title_Scene;    //シーンの管理変数

};

extern Game* game;