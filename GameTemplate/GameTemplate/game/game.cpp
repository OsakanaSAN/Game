/*!
 * @brief	ゲーム
 */
#include "stdafx.h"
#include "game.h"


Enemy enemy3;

/*!
 * @brief	コンストラクタ。
 */
Game::Game()
{
	D3DXCreateSprite(g_pd3dDevice, &spt);
	
}
/*!
 * @brief	デストラクタ。
 */
Game::~Game()
{
}
/*!
 * @brief	ゲームが起動してから一度だけ呼ばれる関数。
 */
void Game::Start()
{
	
	title.Start();


	Gamecamera.Strat();
	g_physicsWorld = new PhysicsWorld;
	g_physicsWorld->Init();
	Boot.Start(); //スプライト

	map.Init();

	//プレイヤーのインスタンスを生成
	player.Start();


	/*for (int i = 0;i < 3;i++)
	{
		Enemy* enemy = new Enemy;
		Enemys.push_back(enemy);
		enemy->Start(D3DXVECTOR3(500.0f * i, 1000.0f, -300.0f));
	}*/
	

	game->GetCamera()->SetLookatPt(player.GetPos());
	D3DXVECTOR3 l = player.GetPos() - game->GetCamera()->GetEyePt();
	Gamecamera.SetPLength(D3DXVec3Length(&l));

}
/*!
 * @brief	更新。
 */
void Game::Update()
{
	g_fade.Update();
	switch (Scene)
	{
	case Title_Scene:



		title.Update();
		break;

	case Game_Scene:


		player.Update(); //プレイヤーの更新
		Gamecamera.Update(); //カメラのアップデート
		
		/*for (auto Enemy : Enemys) {
			Enemy->Update();
		}*/

		auto bulletIt = PlayerBullets.begin();
		while (bulletIt != PlayerBullets.end()) {
			if (!(*bulletIt)->Update()) {
				//死亡
				bulletIt = PlayerBullets.erase(bulletIt);
			}
			else {
				bulletIt++;
			}


		}
		auto EbulletIt = EnemyBullets.begin();
		while (EbulletIt != EnemyBullets.end()) {

			if (!(*EbulletIt)->Update()) {
				//死亡
				EbulletIt = EnemyBullets.erase(EbulletIt);
			}
			else {
				EbulletIt++;
			}


		}


		
		map.Update();
		
		Boot.Update();
		break;
	}
}

/*!
* @brief	描画。
*/
void Game::Render()
{
	
	switch (Scene)
	{
	case Title_Scene:

		title.Drow(spt);
		break;
	case Game_Scene:

		player.Render();//プレイヤー描画

		//for (auto Enemy : Enemys) {
		//	Enemy->Render();
		//}

		map.Render();

		for (auto bullet : PlayerBullets) {
			bullet->Render();
		}
		for (auto bullet : EnemyBullets) {
			bullet->Render();
		}

		Boot.Drow(spt);

		break;
	}
	g_fade.Drow(spt);
}
