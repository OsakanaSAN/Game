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
#include "Sprite.h"
#include "C2DImage.h"
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
	void CameraAngle();

	Camera* GetCamera()
	{
		return &camera;
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
private:
	D3DXVECTOR3		toEyePos;		//注視点から視点までのベクトル。
	D3DXVECTOR3    oldPos;

	Camera camera;
	Player player;
	Enemy  enemy;
	Enemy  enemy2;
	Pad    pad;
	Map    map;
	CPrimitive prim;
	Sprite*    sprite;
	LPD3DXSPRITE spt;
	D3DXVECTOR3  rag;
	float       Time = 0;

	std::list<Bullet*> PlayerBullets;
	std::list<Bullet*> EnemyBullets;
	std::list<Enemy*>  Enemys;
	float             length;


};

extern Game* game;