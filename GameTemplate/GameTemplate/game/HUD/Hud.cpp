#include "stdafx.h"
#include "Hud.h"




CHud::CHud()
{
}


CHud::~CHud()
{
}
//初期化
void CHud::Start()
{
	//Hpゲージ表示
	m_Hp[0] = new Sprite;
	m_Hp[0]->Loadtex("Assets/sprite/back_Hp.png");
	m_Hp[0]->Initialize();
	m_Hp[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Hp[0]->SetSize(m_BootSize);
	m_Hp[0]->Setposition({ 20.0f,650.0f });
	//Hpゲージ表示
	m_Hp[1] = new Sprite;
	m_Hp[1]->Loadtex("Assets/sprite/Hp.png");
	m_Hp[1]->Initialize();
	m_Hp[1]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Hp[1]->SetSize(m_HpSize);
	m_Hp[1]->Setposition({ 22.0f,648.0f });

	//ダッシュゲージ表示
	m_Bootht[0] = new Sprite;
	m_Bootht[0]->Loadtex("Assets/sprite/back_Hp.png");
	m_Bootht[0]->Initialize();
	m_Bootht[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Bootht[0]->SetSize(m_BootSize);
	m_Bootht[0]->Setposition({ 60.0f,650.0f });
	//ダッシュゲージ表示
	m_Bootht[1] = new Sprite;
	m_Bootht[1]->Loadtex("Assets/sprite/Hp.png");
	m_Bootht[1]->Initialize();
	m_Bootht[1]->Setcolor(1.0f, 1.0f, 0.0f, 1.0f);
	m_Bootht[1]->SetSize(m_HpSize);
	m_Bootht[1]->Setposition({ 62.0f,648.0f });

	m_Circle = new Sprite;
	m_Circle->Loadtex("Assets/sprite/HUD2.png");
	m_Circle->Initialize();
	m_Circle->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Circle->SetSize({ 100.0f,100.0f });
	m_Circle->Setposition({ 650.0f, 320.0f });

}

//更新
void CHud::Update()
{
	if (game->GetPlayer() == NULL) { return; }
	int Hp = game->GetPlayer()->GetHp();
	float Percent = (float)Hp / game->GetPlayer()->GetMaxHp();
	Percent = m_HpSize.y * Percent;
	m_Hp[1]->SetSize({ m_HpSize.x,Percent });
	float BoothtTime = 0;
	

	for (int i = 0;i < 2;i++)
	{
		m_Hp[i]->Update();
		m_Bootht[i]->Update();
	}

	float nat = m_BootSize.y / 5.0f * game->GetPlayer()->GetBoothtTime();
	
	test = nat;
	m_Bootht[1]->SetSize({ m_BootSize.x ,m_BootSize.y - test });

	m_Circle->Update();
	
	m_pad.Update();
}

//描画
void CHud::Drow(LPD3DXSPRITE spt)
{
	for (int i = 0;i < 2;i++)
	{
		m_Hp[i]->Draw(spt);
		m_Bootht[i]->Draw(spt);
	}
	
	if (m_NoRenderCircle) { return; }
	m_Circle->Draw(spt);
	
	

}