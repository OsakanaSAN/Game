#pragma once
#include "Map/Map.h"
#include "SelectScene.h"
#include "Character/Player.h"
#include "Map/Map.h"
#include "GameCamera/GameCamera.h"

class CGameScene
{
public:
	CGameScene();
	~CGameScene();
	void Start(int map);
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

	CPlayer* GetPlayer()
	{
		return m_Player.get();
	}

private:
	bool	m_Clear = false;
	int		m_EnemyCount = 0;
	SceneState		m_gameState;
	CSoundSource*	m_Sound[3];
	float			m_FadeTime = 0.0f;
	bool			m_VoiceOn = false;
	std::unique_ptr<CPlayer>	m_Player = NULL;
	std::unique_ptr<Map>		m_Map;
	std::unique_ptr<GameCamera>	m_GameCamera;

		
};

