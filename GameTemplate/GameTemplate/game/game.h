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

	Player* GetPlayer()
	{
		return &player;
	}
private:
	D3DXVECTOR3		toEyePos;		//注視点から視点までのベクトル。
	Camera camera;
	Player player;
	Enemy  enemy;
	Pad    pad;
	Map    map;
	std::list<Bullet*> PlayerBullets;
	std::list<Bullet*> EnemyBullets;

};

extern Game* game;