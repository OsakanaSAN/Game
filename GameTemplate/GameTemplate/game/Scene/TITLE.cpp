#include "stdafx.h"
#include "TITLE.h"


TITLE::TITLE()
{
	
}


TITLE::~TITLE()
{
}

void TITLE::Start()
{
	m_Title_Sound[0] = new CSoundSource;
	m_Title_Sound[0]->Init("Assets/Sound/TitleSound.wav");
	m_Title_Sound[0]->SetVolume(0.8f);
	m_Title_Sound[0]->Play(1);
	m_Title_Sound[1] = new CSoundSource;
	m_Title_Sound[1]->Init("Assets/Sound/TitleSound_Back2.wav");
	m_Title_Sound[1]->SetVolume(0.5f);
	m_Title_Sound[1]->Play(1);
	m_Title_Sound[2] = new CSoundSource;
	m_Title_Sound[2]->Init("Assets/Sound/Se/Select_SE.wav");
	m_Title_Sound[2]->SetVolume(1.0f);

	for (int i = 0; i < 4;i++)
	{
		Title[i] = new Sprite;
	}
	Title[0]->Loadtex("Assets/sprite/BackScreen.png");
	Title[1]->Loadtex("Assets/sprite/NewGame.png");
	Title[2]->Loadtex("Assets/sprite/Exit.png");
	Title[3]->Loadtex("Assets/sprite/BattleCore.png");

	Title[0]->Initialize();
	Title[1]->Initialize();
	Title[2]->Initialize();
	Title[3]->Initialize();
	


	Title[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	Title[1]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	Title[2]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	Title[3]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	

	Title[0]->SetSize({ 1280.0f,720.0f });
	Title[1]->SetSize({ 200.0f,100.0f });
	Title[2]->SetSize({ 800.0f,700.0f });
	Title[3]->SetSize({ 1000.0f,400.0f });

	Title[0]->Setposition({ 0.0f,0.0f });
	Title[1]->Setposition({ 550.0f,500.0f });
	Title[2]->Setposition({ 600.0f,545.0f });
	Title[3]->Setposition({ 0.0f,0.0f });


	


}

void TITLE::Update()
{
	if (m_state == End) { return; }
	Title[0]->Update();
	Title[1]->Update();
	Title[2]->Update();
	Title[3]->Update();
	pad.Update();

	switch (m_state)
	{
	case eStateWaitFadeIn:
		if (!game->GetFade()->IsExecute()) {
			m_state = eStateRun;
		}
		break;

	case eStateRun:
		TitleFontCastom();
		

		if (pad.IsTrigger(Pad::enButtonStart))
		{
			
			m_Title_Sound[2]->Play(0);
			game->GetFade()->StartFadeOut();
			m_state = eStateWaitFadeOut;
			

		}

		m_Title_Sound[0]->Update();
		m_Title_Sound[1]->Update();
		break;
	case eStateWaitFadeOut:
		m_Title_Sound[0]->Update();
		m_Title_Sound[1]->Update();
		m_Title_Sound[2]->Update();

		if (!game->GetFade()->IsExecute())
		{
				Title[0]->Setcolor(0.0, 1.0f, 1.0f, 1.0f);
				Title[1]->Setcolor(0.0f, 1.0f, 1.0f, 1.0f);
				Title[2]->Setcolor(0.0f, 1.0f, 1.0f, 1.0f);
				Title[3]->Setcolor(0.0f, 1.0f, 1.0f, 1.0f);

				m_Title_Sound[0]->Release();
				m_Title_Sound[1]->Release();
				m_Title_Sound[2]->Release();
				game->SetGameScene();
				game->NewGameScene();
				game->GetGameScene()->Start();
				game->GetFade()->StartFadeIn();
				m_state = End;
				return;
		}

		break;
	case End:

		break;

	}
	
}

void TITLE::TitleFontCastom()
{
	m_timer += GameTime().GetFrameDeltaTime() / 10.0f;



	if (m_AlphaUp == false)
	{
		if (m_timer > 0.5f)
		{
			m_AlphaUp = true;
			m_timer = 0.5f;
		}
	}
	else
	{
		if (m_timer > 1.0f)
		{

			m_AlphaUp = false;
			m_timer = 0.0f;
		}
	}


	if (m_AlphaUp == false)
	{
		Title[3]->Setcolor(alpha - m_timer, 1.0f, 1.0f, 1.0f);

	}
	else
	{
		Title[3]->Setcolor(m_timer, 1.0f, 1.0f, 1.0f);

	}

}

void TITLE::Drow(LPD3DXSPRITE spt)
{
	if (m_state == End) { return; }
	Title[0]->Draw(spt);
	Title[1]->Draw(spt);
	//Title[2]->Draw(spt);
	Title[3]->Draw(spt);
}