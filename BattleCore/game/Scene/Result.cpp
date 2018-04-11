#include "stdafx.h"
#include "Result.h"


CResult::CResult()
{
}


CResult::~CResult()
{
	for (int i = 0;i < 2;i++)
	{

		m_result_imag[i]->TexRelease();
		delete m_result_imag[i];
	}
}

void CResult::Start()
{
	m_Pad = new Pad;
	m_resultState = e_Upadte;
	m_result_imag[0] = new Sprite;
	m_result_imag[0]->Loadtex("Assets/sprite/Result/Result_BackScreen.png");
	m_result_imag[0]->Initialize();
	m_result_imag[0]->Setcolor(1.0f, 1.0f, 1.0f, 1.0f);
	m_result_imag[0]->Setposition({ 0.0f,0.0f });
	m_result_imag[0]->SetSize({ 1080.0f,720.0f });

	m_result_imag[1] = new Sprite;
	m_result_imag[1]->Loadtex("Assets/sprite/Result/Result_Back2.png");
	m_result_imag[1]->Initialize();
	m_result_imag[1]->Setcolor(1.0f,1.0f,1.0f,1.0f);
	m_result_imag[1]->Setposition({ 400.0f,100.0f });
	m_result_imag[1]->SetSize({ 600.0f,600.0f });



}

void CResult::Update()
{
	switch (m_resultState)
	{
	case e_Upadte:
		for (int i = 0;i < 2;i++)
		{

			m_result_imag[i]->Update();

		}

		if (m_Pad->IsTrigger(Pad::enButtonStart))
		{
			game->GetFade()->StartFadeOut();
			m_resultState = e_Clear;
		}
		
		m_Pad->Update();
		break;
	case e_Clear:
		//フェイドするまで何もしない
		if (!game->GetFade()->IsExecute())
		{
			
			m_resultState = e_End;
			game->NewTitle();
			game->GetTitle()->Start();
			game->SetTitleScene();
			game->GetFade()->StartFadeIn();
			delete game->GetResult();
			return;
		}
		break;

	case e_End:

		break;

	}
}


void CResult::Render(LPD3DXSPRITE spt)
{
	if (m_resultState == e_End) { return; }
	for (int i = 0; i < 2;i++)
	{

		m_result_imag[i]->Draw(spt);

	}
}