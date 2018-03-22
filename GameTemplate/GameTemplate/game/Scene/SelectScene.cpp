#include "stdafx.h"
#include "SelectScene.h"


CSelectScene::CSelectScene()
{
}


CSelectScene::~CSelectScene()
{
	for (int i = 0;i < 3;i++)
	{

		m_Select_imag[i]->TexRelease();
		delete m_Select_imag[i];

	}

}


void CSelectScene::Start()
{
	for (int i = 0;i < 3;i++)
	{

		m_Select_imag[i] = new Sprite;

	}
	m_Select_imag[0]->Loadtex("Assets/sprite/Select/Select_BackScreen.png");
	m_Select_imag[0]->Initialize();
	m_Select_imag[0]->SetSize({ 950.0f,720.0f });
	m_Select_imag[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Select_imag[0]->Setposition({ 0.0f,0.0f });

	m_Select_imag[1]->Loadtex("Assets/sprite/Select/map1.png");
	m_Select_imag[1]->Initialize();
	m_Select_imag[1]->SetSize({ 700.0f,300.0f });
	m_Select_imag[1]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Select_imag[1]->Setposition({ 170.0f,400.0f });

	m_Select_imag[2]->Loadtex("Assets/sprite/Select/yazirusi.png");
	m_Select_imag[2]->Initialize();
	m_Select_imag[2]->SetSize({ 100.0f,100.0f });
	m_Select_imag[2]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Select_imag[2]->Setposition({ 350.0f,200.0f });
}


void CSelectScene::Update()
{
	switch (m_state)
	{
	case e_Upadte:
		for (int i = 0;i < 3;i++)
		{
			m_Select_imag[i]->Update();
		}

		if (m_pad.IsTrigger(Pad::enButtonDown) && m_mapselect != 2)
		{
			m_mapselect = 2;
			sprintf(TextName, "Assets/sprite/Select/map%d.png", m_mapselect);
			m_Select_imag[1]->TexRelease();
			m_Select_imag[1]->Loadtex(TextName);
			m_Select_imag[1]->Initialize();
			m_Select_imag[2]->Setposition({ 350.0f,300.0f });
		}
		else if (m_pad.IsTrigger(Pad::enButtonUp) && m_mapselect != 1)
		{
			m_mapselect = 1;
			sprintf(TextName, "Assets/sprite/Select/map%d.png", m_mapselect);
			m_Select_imag[1]->TexRelease();
			m_Select_imag[1]->Loadtex(TextName);
			m_Select_imag[1]->Initialize();
			m_Select_imag[2]->Setposition({ 350.0f,200.0f });
		}

		if (m_pad.IsTrigger(Pad::enButtonStart))
		{

			game->GetFade()->StartFadeOut();
			m_state = e_Clear;


		}
		m_pad.Update();
		break;
	case e_Clear:
		if (!game->GetFade()->IsExecute())
		{

			game->SetGameScene();
			game->NewGameScene();
			game->GetGameScene()->Start(m_mapselect);
			game->GetFade()->StartFadeIn();
			m_state = e_End;
			delete this;

		}

		break;
	}

}

void CSelectScene::Render(LPD3DXSPRITE spt)
{
	for (int i = 0;i < 3;i++)
	{
		m_Select_imag[i]->Draw(spt);
	}
	
}