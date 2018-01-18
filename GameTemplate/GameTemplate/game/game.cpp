/*!
 * @brief	ゲーム
 */
#include "stdafx.h"
#include "game.h"

int count = 0;
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
	//delete &player;
	//auto SoundIt = Sounds.begin();
	//while (SoundIt != Sounds.end()) {
	//	(*SoundIt)->Release();
	//		//死亡
	//		SoundIt = Sounds.erase(SoundIt);
	//		count++;
	//}
	//m_SoundEngine.Release();
	
}
/*!
 * @brief	ゲームが起動してから一度だけ呼ばれる関数。
 */
void Game::Start()
{
	m_SoundEngine.Init();
	

	Gamecamera.Strat();
	g_physicsWorld = new PhysicsWorld;
	g_physicsWorld->Init();
	Hud.Start(); //スプライト

	map.Init();

	//プレイヤーのインスタンスを生成
	player.Start();

	game->GetCamera()->SetLookatPt(player.GetPos());
	D3DXVECTOR3 l = player.GetPos() - game->GetCamera()->GetEyePt();
	Gamecamera.SetPLength(D3DXVec3Length(&l));
	title.Start();
}
/*!
 * @brief	更新。
 */
void Game::Update()
{
	m_SoundEngine.Update();
	g_fade.Update();
	switch (Scene)
	{

	case Result_Scene:

		break;

	case Title_Scene:



		title.Update();
		break;

	case Game_Scene:

		player.Update(); //プレイヤーの更新
		Gamecamera.Update(); //カメラのアップデート
	

		auto EnemyIt = Enemys.begin();
		while (EnemyIt != Enemys.end()) {
			if ((*EnemyIt)->IsEnd()) {
				//死亡
				EnemyIt = Enemys.erase(EnemyIt);
				count++;
			}
			else {
				(*EnemyIt)->Update();
				EnemyIt++;
			}


		}

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
		
		Hud.Update();
		break;
	}
}

/*!
* @brief	3Dモデルの描画。
*/
void Game::Render()
{
	
	switch (Scene)
	{
	//タイトル画面の描画
	case Title_Scene:

		//title.Drow(spt);
		break;
	//ゲーム画面の描画
	case Game_Scene:

		

		player.Render();//プレイヤー描画

		for (auto Enemy : Enemys) {
			Enemy->Render();
		}

		map.Render();

		for (auto bullet : PlayerBullets) {
			
			bullet->Render();
		}

		for (auto bullet : EnemyBullets) {
			bullet->Render();
		}

		
		break;
	case Result_Scene:
		break;
	}
	
	
}

//2Dの描画
void Game::Render2D()
{
	switch (Scene)
	{
	case Title_Scene:
		title.Drow(spt);
		break;

	case Game_Scene:
		Hud.Drow(spt);
		break;

	}

	g_fade.Drow(spt);

}
