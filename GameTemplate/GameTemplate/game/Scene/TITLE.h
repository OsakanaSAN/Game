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
		eStateWaitFadeIn,	//!<フェードイン待ち。
		eStateRun,			//!<実行中。
		eStateWaitFadeOut,	//!<フェードアウト待ち。
		End,
	};
	EState			m_state = eStateRun;
	Sprite*			Title[4];
	D3DXVECTOR2		Meterpos = { 0.0f,0.0f };
	Pad				pad;
	int				Speed;   //meterの数字格納
	float			alpha = 1.0f;
	float			m_timer = 1.0f;			//!<タイマ(単位：秒)
	float			OutTimer = 0.0f;        
	const float		FADE_TIME = 3.0f;		//!<フェードアウトの時間。(単位：秒)
	bool			m_isExecute = true;	//!<フェードを実行中？
};

