#pragma once
#include "myEngine/Time/GameTime.h"
class TITLE
{
public:
	TITLE();
	~TITLE();
	void Start();
	void Update();
	void Drow(LPD3DXSPRITE spt);
private:
	enum EState {
		eStateWaitFadeIn,	//!<�t�F�[�h�C���҂��B
		eStateRun,			//!<���s���B
		eStateWaitFadeOut,	//!<�t�F�[�h�A�E�g�҂��B
		End,
	};
	EState			m_state = eStateRun;
	Sprite*			Title[4];
	D3DXVECTOR2		Meterpos = { 0.0f,0.0f };
	Pad				pad;
	int				Speed;   //meter�̐����i�[
	float			alpha = 1.0f;
	float			m_timer = 1.0f;			//!<�^�C�}(�P�ʁF�b)
	float			OutTimer = 0.0f;        
	const float		FADE_TIME = 3.0f;		//!<�t�F�[�h�A�E�g�̎��ԁB(�P�ʁF�b)
	bool			m_isExecute = true;	//!<�t�F�[�h�����s���H
};

