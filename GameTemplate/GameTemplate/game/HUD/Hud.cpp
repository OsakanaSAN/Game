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
	//ブーストゲージ表示
	m_boot = new Sprite;
	m_boot->Loadtex("Assets/sprite/Bootht.jpg");
	m_boot->Initialize();
	m_boot->Setcolor(0.5f, 1.0f, 1.0f, 1.0f);
	m_boot->SetSize(m_BootSize);
	m_boot->Setposition({ 500.0f,50.0f });
	m_Circle = new Sprite;
	m_Circle->Loadtex("Assets/sprite/HUD1.png");
	m_Circle->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Circle->SetSize({ 100.0f,100.0f });
	
	m_Circle->Initialize();
	
}

//更新
void CHud::Update()
{
	float BoothtTime = 0;
	if (m_boot->GetSize().x > 0 && game->GetPlayer()->GetBoothtTime() > 0 && m_pad.IsPress(Pad::enButtonRB2)&& !game->GetPlayer()->GetNotBootht())
	{
		BoothtTime = -1.0f;
		m_boot->SetSize({ m_BootSize.x - BoothtTime, m_BootSize.y });
		
	}
	else if (game->GetPlayer()->GetBoothtTime() > 0 )
	{
		BoothtTime = 1.0f;
		m_boot->SetSize({ m_BootSize.x + BoothtTime ,m_BootSize.y });
	}

	m_Circle->Setposition({ 620.0f, 320.0f });
	m_boot->Update();
	m_Circle->Update();
	m_pad.Update();
}

//描画
void CHud::Drow(LPD3DXSPRITE spt)
{

	m_boot->Draw(spt);
	m_Circle->Draw(spt);

}