#include "stdafx.h"
#include "Fade.h"


CFade::CFade()
{
	m_Sprite = new Sprite;
	m_Sprite->Loadtex("Assets/sprite/BackScreen.png");
	m_Sprite->Initialize();
	m_Sprite->Setcolor(0.0f, 1.0f, 1.0f, 1.0f);
	m_Sprite->Setposition({ 0.0f, 0.0f });
	m_Sprite->SetSize({ 1280.0f, 720.0f });

}


CFade::~CFade()
{
}


void CFade::Update()
{


	if (m_IsExecute) {
		switch (m_State) {
		case eFadeOut: {
			m_Timer += GameTime().GetFrameDeltaTime();
			if (m_Timer < m_FADE_TIME) {
				float t = m_Timer / m_FADE_TIME;
				m_Alpha = (min(t, 1.0f));
				

				m_OutTimer += GameTime().GetFrameDeltaTime();
			}
			else if (m_OutTimer < 2)
			{
				m_Alpha = 1.0f;

				m_OutTimer += GameTime().GetFrameDeltaTime();
			}

			else {
				m_Alpha = 1.0f;
				m_IsExecute = false;
				m_OutTimer = 0;

			}
		}break;

		case eFadeIn:
			//ブルスクが映り込まないようにするための処理
			m_OutTimer += GameTime().GetFrameDeltaTime();
			if (m_OutTimer < 1.5) { return; }

			//ここからロード画面を消す
			m_Timer += GameTime().GetFrameDeltaTime();
			if (m_Timer < m_FADE_TIME) {
				float t = m_Timer / m_FADE_TIME;
				m_Alpha = max(1.0f - t, 0.0f);

			}


			else {
				//透明になったので非アクティブにする。
				m_Alpha = 0.0f;
				m_SetActiveFlag = false;
				m_Timer = m_FADE_TIME;
				m_IsExecute = false;
				m_OutTimer = 0;
			}
			break;
		}
		
	}

	m_Sprite->Setcolor(m_Alpha,1.0f,1.0f,1.0f);
	m_Sprite->Update();
}

void CFade::Drow(LPD3DXSPRITE spt)
{
	m_Sprite->Draw(spt);
}
