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

	
}
/*!
 * @brief	ゲームが起動してから一度だけ呼ばれる関数。
 */
void Game::Start()
{
	m_SoundEngine = new CSoundEngine;
	m_SoundEngine->Init();

	Hud.Start(); //スプライト
	m_title = new TITLE;
	m_title->Start();


	g_physicsWorld = new PhysicsWorld;
	g_physicsWorld->Init();
}
/*!
 * @brief	更新。
 */
void Game::Update()
{
	m_SoundEngine->Update();
	g_fade.Update();
	switch (Scene)
	{

	case Result_Scene:
		m_Result->Update();
		break;


	case Select_Scene:
		m_SelectScene->Update();

		break;

	case Title_Scene:

		m_title->Update();
		break;

	case Game_Scene:
		g_physicsWorld->Update();
		m_GameScene->Update();
		Hud.Update();
		break;
	}
}

void Game::ListUpdate()
{

	auto EnemyIt = Enemys.begin();
	while (EnemyIt != Enemys.end()) {


		if ((*EnemyIt)->IsEnd()) {
			//死亡
			//敵の残り数
			if (!(*EnemyIt)->GetIsCountDown())
			{
				(*EnemyIt)->setCountDown(true);
				game->GetGameScene()->CountDown(1);
				//(*EnemyIt)->Delete();
				//EnemyIt = Enemys.erase(EnemyIt);
			}
		}
		

			(*EnemyIt)->Update();
			EnemyIt++;
		


	}


	//プレイヤーの弾の更新
	for (auto& Playerbullet : PlayerBullets)
	{

		Playerbullet->Update();
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


		break;

	case Select_Scene:


		break;
	//ゲーム画面の描画
	case Game_Scene:

		m_GameScene->Render();
		
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
		
		m_title->Drow(spt);
		break;

	case Select_Scene:

		m_SelectScene->Render(spt);

		break;

	case Game_Scene:
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		Hud.Drow(spt);
		for (const auto& Enemy : game->GetEnemys()) {
			Enemy->Render2D();
		}
		game->GetMap()->Render2D();
		
		for (auto Bullet : game->GetPlayerBullet())
		{
			Bullet->Render2D();
		}
		break;
	case Result_Scene:
		m_Result->Render(spt);
		m_ScoreCheck->Render(spt);
		break;

	}

	g_fade.Drow(spt);

}
