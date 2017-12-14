#pragma once
class CFade 
{
public:
	CFade();
	~CFade();
	

	void Update();

	void Drow(LPD3DXSPRITE spt);

	bool IsExecute() const
	{
		return m_IsExecute;
	}

	void StartFadeOut()
	{
		m_Timer = 0.0f;
		m_SetActiveFlag = true;
		m_IsExecute = true;
		m_State = eFadeOut;

	}
	void StartFadeIn()
	{
		if (m_Alpha > 0.0f) {
			m_Timer = 0.0f;
			m_SetActiveFlag = true;
			m_IsExecute = true;
			m_State = eFadeIn;
		}
	}


private:
	//状態。
	enum EnState {
		eFadeOut,		//!<フェードアウト。
		eFadeIn,		//!<フェードイン。
	};

	enum Load
	{
		UP,
		DOWN,
	};

	int			m_Cabetu = UP;
	const float m_FADE_TIME = 1.5f;		//!<フェードアウトの時間。(単位：秒)

	Sprite*		m_Sprite;				//!ブラックアウト用のスプライト。
	
	EnState		m_State = eFadeIn;		//!<状態。
	float       m_Alpha = 0.0f;
	bool		m_IsExecute = false;	//!<フェードを実行中？
	float		m_Timer = 1.0f;			//!<タイマ(単位：秒)
	float       m_OutTimer = 0.0f;
	bool        m_SetActiveFlag = false;

	D3DXVECTOR2 m_Cabepos = { 720,-280 };
};

