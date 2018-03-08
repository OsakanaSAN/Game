#pragma once
#include "Map/Map.h"

class CGameScene
{
public:
	enum GameState {
		e_Upadte,		//アップデート中
		e_Clear,		//クリア判定
		e_End,		//終了判定
	};
	CGameScene();
	~CGameScene();
	void Start();
	void Update();
	void Render();
	void CountUp(int count)
	{
		m_EnemyCount += count;
	}
	void CountDown(int count)
	{
		m_EnemyCount -= count;
	}
	

private:
	bool	m_Clear = false;
	int		m_EnemyCount = 0;
	GameState		m_gameState;
	CSoundSource*	m_Sound[2];


};

