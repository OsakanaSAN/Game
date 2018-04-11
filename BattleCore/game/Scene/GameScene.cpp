#include "stdafx.h"
#include "GameScene.h"


CGameScene::CGameScene()
{



}


CGameScene::~CGameScene()
{
	delete m_Sound[0];
	delete m_Sound[1];
	delete m_Sound[2];
}


void CGameScene::Start(int map)
{
	m_Sound[0] = new CSoundSource;
	m_Sound[0]->Init("Assets/Sound/GameSound_Sea.wav");
	m_Sound[0]->SetVolume(0.3f);
	if (map == 1)
	{

		m_Sound[1] = new CSoundSource;
		m_Sound[1]->Init("Assets/Sound/GameSound_PlayGame.wav");
		m_Sound[1]->SetVolume(0.3f);

	}
	else
	{
		m_Sound[1] = new CSoundSource;
		m_Sound[1]->Init("Assets/Sound/GameSound_PlayGame2.wav");
		m_Sound[1]->SetVolume(0.3f);
	}

	//カメラのインスタンスを生成
	game->NewGameCamera();
	game->GetGameCamara()->Strat();

	//プレイヤーのインスタンスを生成
	game->NewPlayer();
	game->GetPlayer()->Start();

	//スコアチェッカーのインスタンスを生成
	game->NewScoreCheckre();
	game->GetScorecheckre()->Start();

	//マップのインスタンスを生成
	game->NewMap();
	game->GetMap()->Init(map);
	
	//カメラの注視点を設定
	game->GetGameCamara()->Getcamera()->SetLookatPt(game->GetPlayer()->GetPos());
	D3DXVECTOR3 l = game->GetPlayer()->GetPos() - game->GetGameCamara()->Getcamera()->GetEyePt();
	game->GetGameCamara()->SetPLength(D3DXVec3Length(&l));
	m_gameState = e_Upadte;
	m_Sound[0]->Play(1);
	m_Sound[1]->Play(1);
}

void CGameScene::Update()
{
	switch (m_gameState)
	{

		//アップデート
	case e_Upadte:

			game->GetPlayer()->Update();
			game->GetScorecheckre()->Update();
			game->GetMap()->Update();
			game->GetGameCamara()->Update();
			game->ListUpdate();
			m_Sound[0]->Update();
			m_Sound[1]->Update();
			if (m_EnemyCount == 0 )
			{
				m_FadeTime += GameTime().GetFrameDeltaTime();
				
				//m_gameState = e_Clear;
				//game->GetScorecheckre()->TimeCheck();
				//game->GetFade()->StartFadeOut();
			}

			if (m_FadeTime > 4.0f || game->GetPlayer()->GetHp() <= 0)
			{
				m_gameState = e_Clear;
				game->GetScorecheckre()->TimeCheck();
				game->GetFade()->StartFadeOut();
			}

		break;

	case e_Clear:

		//フェイドするまで何もしない
		if (!game->GetFade()->IsExecute())
		{
			m_Sound[1]->Release();
			m_Sound[0]->Release();
			
			delete game->GetMap();
			delete game->GetGameCamara();
			game->DeleteEnemy();
			game->DeletePlayer();
			//delete g_physicsWorld;
			game->NewResult();
			game->GetResult()->Start();
			game->GetFade()->StartFadeIn();
			game->SetResult();
			delete game->GetGameScene();
			
			
			return;
		}
		game->ListUpdate();
		break;

	case e_End:


		break;

	}


}
void CGameScene::Render()
{
	game->GetMap()->Render();
	
	for (const auto& Enemy : game->GetEnemys()) {
		Enemy->Render();
	}
	
	for (const auto& bullet : game->GetPlayerBullet()) {
		
		bullet->Render();
	}

	for (const auto& bullet : game->GetEnemyBullet()) {
		bullet->Render();
	}
	game->GetPlayer()->Render();
	
}
