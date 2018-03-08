#pragma once
#include "Map/Map.h"

class CGameScene
{
public:
	enum GameState {
		e_Upadte,		//�A�b�v�f�[�g��
		e_Clear,		//�N���A����
		e_End,		//�I������
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

