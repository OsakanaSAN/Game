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
	//��ԁB
	enum EnState {
		eFadeOut,		//!<�t�F�[�h�A�E�g�B
		eFadeIn,		//!<�t�F�[�h�C���B
	};

	enum Load
	{
		UP,
		DOWN,
	};

	int			m_Cabetu = UP;
	const float m_FADE_TIME = 1.5f;		//!<�t�F�[�h�A�E�g�̎��ԁB(�P�ʁF�b)

	Sprite*		m_Sprite;				//!�u���b�N�A�E�g�p�̃X�v���C�g�B
	
	EnState		m_State = eFadeIn;		//!<��ԁB
	float       m_Alpha = 0.0f;
	bool		m_IsExecute = false;	//!<�t�F�[�h�����s���H
	float		m_Timer = 1.0f;			//!<�^�C�}(�P�ʁF�b)
	float       m_OutTimer = 0.0f;
	bool        m_SetActiveFlag = false;

	D3DXVECTOR2 m_Cabepos = { 720,-280 };
};

