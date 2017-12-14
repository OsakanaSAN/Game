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
	


	Title[0]->Setcolor(alpha, 1.0f, 1.0f, 1.0f);
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

		if (pad.IsTrigger(Pad::enButtonStart))
		{
			
			game->GetFade()->StartFadeOut();
			m_state = eStateWaitFadeOut;
			

		}
		break;
	case eStateWaitFadeOut:
		if (!game->GetFade()->IsExecute())
		{
			Title[0]->Setcolor(0.0, 1.0f, 1.0f, 1.0f);
			Title[1]->Setcolor(0.0f, 1.0f, 1.0f, 1.0f);
			Title[2]->Setcolor(0.0f, 1.0f, 1.0f, 1.0f);
			Title[3]->Setcolor(0.0f, 1.0f, 1.0f, 1.0f);

			game->SetGameScene();
			game->GetFade()->StartFadeIn();
			m_state = End;
			return;
		}
		break;
	case End:

		break;

	}
	
}

void TITLE::Drow(LPD3DXSPRITE spt)
{
	if (m_state == End) { return; }
	Title[0]->Draw(spt);
	Title[1]->Draw(spt);
	Title[2]->Draw(spt);
	Title[3]->Draw(spt);
}