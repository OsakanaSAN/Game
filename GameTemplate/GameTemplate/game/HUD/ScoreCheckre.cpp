#include "stdafx.h"
#include "ScoreCheckre.h"


CScoreCheckre::CScoreCheckre()
{
}


CScoreCheckre::~CScoreCheckre()
{

	for (int i = 0;i < 4;i++)
	{

		m_Number[i]->TexRelease();
		delete m_Number[i];
		m_Dameg_img[i]->TexRelease();
		delete m_Dameg_img[i];

	}

	for (int i = 0;i < 3;i++)
	{

		m_Score_img[i]->TexRelease();
		delete m_Score_img[i];
	}
	m_Rank_img->TexRelease();
	delete m_Rank_img;
}

void CScoreCheckre::Start()
{
	m_DamageChecker[0] = game->GetPlayer()->GetMaxHp();
	for (int i = 0;i < 4;i++)
	{
		m_Number[i] = new Sprite;
		m_Number[i]->Loadtex("Assets/sprite/Number/number0.png");
		m_Number[i]->Initialize();
		m_Number[i]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Number[i]->SetSize({ 100.0f,100.0f });
		m_Number[i]->Setposition({ m_num,20.0f });
		m_num += 20.0f;

		m_Dameg_img[i] = new Sprite;
		m_Dameg_img[i]->Loadtex("Assets/sprite/Number/number0.png");
		m_Dameg_img[i]->Initialize();
		m_Dameg_img[i]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Dameg_img[i]->SetSize({ 100.0f,100.0f });
		m_Dameg_img[i]->Setposition({ 0.0f,0.0f });

	}
	for (int i = 0;i < 3;i++)
	{
		m_Score_img[i] = new Sprite;
		m_Score_img[i]->Loadtex("Assets/sprite/Number/number0.png");
		m_Score_img[i]->Initialize();
		m_Score_img[i]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
		m_Score_img[i]->SetSize({ 100.0f,100.0f });
		m_Score_img[i]->Setposition({ 0.0f,0.0f });
	}

	m_Rank_img = new Sprite;
	m_Rank_img->Loadtex("Assets/sprite/Number/number0.png");
	m_Rank_img->Initialize();


	m_num = 0;
}

void CScoreCheckre::Update()
{
	m_DamageChecker[1] = game->GetPlayer()->GetHp();//プレイヤーのHP取得
	m_Time += GameTime().GetFrameDeltaTime();
	if (m_Time >= 1)
	{
		++m_TimeChecker;
		m_Time = 0;
	}


}

void CScoreCheckre::SCheck()
{
	sprintf(TextName, "Assets/sprite/Number/number%d.png", m_Score / 100);
	m_Score_img[0]->TexRelease();
	m_Score_img[0]->Loadtex(TextName);
	m_Score_img[0]->Initialize();
	m_Score_img[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Score_img[0]->SetSize({ 130.0f,130.0f });
	m_Score_img[0]->Setposition({ 600.0f,270.0f });

	m_Score %= 100;
	sprintf(TextName, "Assets/sprite/Number/number%d.png", m_Score / 10);
	m_Score_img[1]->TexRelease();
	m_Score_img[1]->Loadtex(TextName);
	m_Score_img[1]->Initialize();
	m_Score_img[1]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Score_img[1]->SetSize({ 130.0f,130.0f });
	m_Score_img[1]->Setposition({ 620.0f,270.0f });

	sprintf(TextName, "Assets/sprite/Number/number%d.png", m_Score % 10);
	m_Score_img[2]->TexRelease();
	m_Score_img[2]->Loadtex(TextName);
	m_Score_img[2]->Initialize();
	m_Score_img[2]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Score_img[2]->SetSize({ 130.0f,130.0f });
	m_Score_img[2]->Setposition({ 640.0f,270.0f });


}

void CScoreCheckre::TimeCheck()
{
	int Minute = m_TimeChecker / 60;
	sprintf(TextName, "Assets/sprite/Number/number%d.png", Minute);
	m_Number[0]->TexRelease();
	m_Number[0]->Loadtex(TextName);
	m_Number[0]->Initialize();
	m_Number[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Number[0]->SetSize({ 130.0f,130.0f });
	m_Number[0]->Setposition({ 600.0f,460.0f });

	Minute = m_TimeChecker % 60;
	sprintf(TextName, "Assets/sprite/Number/number%d.png", Minute / 10);
	m_Number[1]->TexRelease();
	m_Number[1]->Loadtex(TextName);
	m_Number[1]->Initialize();
	m_Number[1]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Number[1]->SetSize({ 130.0f,130.0f });
	m_Number[1]->Setposition({ 640.0f,460.0f });

	Minute = m_TimeChecker % 60;
	sprintf(TextName, "Assets/sprite/Number/number%d.png", Minute % 10);
	m_Number[2]->TexRelease();
	m_Number[2]->Loadtex(TextName);
	m_Number[2]->Initialize();
	m_Number[2]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Number[2]->SetSize({ 130.0f,130.0f });
	m_Number[2]->Setposition({ 660.0f,460.0f });

	m_Number[3]->TexRelease();
	m_Number[3]->Loadtex("Assets/sprite/Number/number_as.png");
	m_Number[3]->Initialize();
	m_Number[3]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Number[3]->SetSize({ 130.0f,130.0f });
	m_Number[3]->Setposition({ 620.0f,460.0f });

	DamageCheck();
	RankCheck();
	SCheck();

}

void CScoreCheckre::DamageCheck()
{
	// 現在のHP％値　  =	現在のHP　　　　÷　　　マックスHP
	float pa = 0.0f;
	float pa2 = 0.0f;
	float pa3 = 0.0f;
	if (m_DamageChecker[1] <= 0)
	{
		m_DamageChecker[1] = 0;
	}
	pa2 = (float)m_DamageChecker[1];
	pa3 = (float)m_DamageChecker[0];
	pa = pa2 / pa3;
	pa *= 100;
	m_DamageChecker[2] = (int)pa;

	sprintf(TextName, "Assets/sprite/Number/number%d.png",m_DamageChecker[2]/100);
	m_Dameg_img[0]->TexRelease();
	m_Dameg_img[0]->Loadtex(TextName);
	m_Dameg_img[0]->Initialize();
	m_Dameg_img[0]->SetSize({ 130.0f,130.0f });
	m_Dameg_img[0]->Setposition({ 650.0f, 370.0f });
	m_DamageChecker[0] = m_DamageChecker[2];
	m_DamageChecker[2] %= 100;
	sprintf(TextName, "Assets/sprite/Number/number%d.png", m_DamageChecker[2] / 10);
	m_Dameg_img[1]->TexRelease();
	m_Dameg_img[1]->Loadtex(TextName);
	m_Dameg_img[1]->Initialize();
	m_Dameg_img[1]->SetSize({ 130.0f,130.0f });
	m_Dameg_img[1]->Setposition({ 670.0f, 370.0f });

	sprintf(TextName, "Assets/sprite/Number/number%d.png", m_DamageChecker[2] % 10);
	m_Dameg_img[2]->TexRelease();
	m_Dameg_img[2]->Loadtex(TextName);
	m_Dameg_img[2]->Initialize();
	m_Dameg_img[2]->SetSize({ 130.0f,130.0f });
	m_Dameg_img[2]->Setposition({ 690.0f, 370.0f });

	m_Dameg_img[3]->TexRelease();
	m_Dameg_img[3]->Loadtex("Assets/sprite/Number/AlphaBet_as.png");
	m_Dameg_img[3]->Initialize();
	m_Dameg_img[3]->SetSize({ 130.0f,130.0f, });
	m_Dameg_img[3]->Setposition({ 710.0f, 350.0f });


}


void CScoreCheckre::RankCheck()
{
	int Hundred = 0;

	if (m_DamageChecker[0] == 100)
	{
		Hundred = 100;
	}

	if ( m_Score + m_DamageChecker[2] + Hundred  <= 100)
	{
		m_Rank = 3; //3 = ランクC
	}

	else if (m_Count * 50 + m_DamageChecker[2] + Hundred >= m_Count * 50 && m_Count * 50 + m_DamageChecker[2] + Hundred <= m_Count * 50 + 89)
	{

		m_Rank = 2; //2 = ランクB

	}

	else if (m_Count * 50 + m_DamageChecker[2] + Hundred >= m_Count * 50 + 90)
	{

		m_Rank = 1;
	}

	m_Rank_img->TexRelease();
	sprintf(TextName, "Assets/sprite/Number/AlphaBet%d.png", m_Rank);
	m_Rank_img->Loadtex(TextName);
	m_Rank_img->Initialize();
	m_Rank_img->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Rank_img->SetSize({ 130.0f,130.0f });
	m_Rank_img->Setposition({ 810.0f,530.0f });
}

void CScoreCheckre::Render(LPD3DXSPRITE spt)
{
	for (int i = 0;i < 4;i++)
	{
		m_Number[i]->Update();
		m_Number[i]->Draw(spt);
		m_Dameg_img[i]->Update();
		m_Dameg_img[i]->Draw(spt);
		
	}

	for (int i = 0;i < 3;i++)
	{
		m_Score_img[i]->Update();
		m_Score_img[i]->Draw(spt);

	}
	m_Rank_img->Update();
	m_Rank_img->Draw(spt);
	
}