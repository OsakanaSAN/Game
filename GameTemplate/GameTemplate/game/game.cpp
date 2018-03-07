/*!
 * @brief	�Q�[��
 */
#include "stdafx.h"
#include "game.h"

int count = 0;
/*!
 * @brief	�R���X�g���N�^�B
 */
Game::Game()
{
	D3DXCreateSprite(g_pd3dDevice, &spt);
	
}
/*!
 * @brief	�f�X�g���N�^�B
 */
Game::~Game()
{
	//delete &player;
	
	/*for (CSoundSource* soundit : Sounds)
	{
		soundit->Release();
		delete soundit;
	}
	Sounds.clear();
	m_SoundEngine->Release();
	delete m_SoundEngine;*/
	
}
/*!
 * @brief	�Q�[�����N�����Ă����x�����Ă΂��֐��B
 */
void Game::Start()
{
	m_SoundEngine = new CSoundEngine;
	m_SoundEngine->Init();

	Hud.Start(); //�X�v���C�g
	m_title = new TITLE;
	m_title->Start();
}
/*!
 * @brief	�X�V�B
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
			//���S
			//�G�̎c�萔
			game->GetGameScene()->CountDown(1);
			(*EnemyIt)->Delete();
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
		if (!(*bulletIt)->Update())
		{
			//���S
			bulletIt = PlayerBullets.erase(bulletIt);
		}
		else {
			bulletIt++;
		}


	}

	auto EbulletIt = EnemyBullets.begin();
	while (EbulletIt != EnemyBullets.end()) {

		if (!(*EbulletIt)->Update()) {
			//���S
			EbulletIt = EnemyBullets.erase(EbulletIt);
			
		}
		else {
			EbulletIt++;
		}


	}

}

/*!
* @brief	3D���f���̕`��B
*/
void Game::Render()
{
	
	switch (Scene)
	{
	//�^�C�g����ʂ̕`��
	case Title_Scene:

		m_title->Drow(spt);

		break;
	//�Q�[����ʂ̕`��
	case Game_Scene:

		m_GameScene->Render();
		
		break;
	case Result_Scene:
		break;
	}
	
	
}

//2D�̕`��
void Game::Render2D()
{
	switch (Scene)
	{
	case Title_Scene:
		m_title->Drow(spt);
		break;

	case Game_Scene:
		Hud.Drow(spt);
		for (const auto& Enemy : game->GetEnemys()) {
			Enemy->Render2D();
		}
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
		g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
		/*for (auto Bullet : game->GetPlayerBullet())
		{
			Bullet->Render2D();
		}*/
		break;
	case Result_Scene:
		m_Result->Render(spt);
		m_ScoreCheck->Render(spt);
		break;

	}

	g_fade.Drow(spt);

}
