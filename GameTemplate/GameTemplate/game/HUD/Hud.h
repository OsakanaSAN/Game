#pragma once
#include "myEngine/Graphics/Sprite/Sprite.h"
class CHud
{
public:
	CHud();
	~CHud();
	void Start();
	void Update();
	void DashTime();
	void Drow(LPD3DXSPRITE spt);
private:
	Sprite* m_Hp[2];   //ブーストゲージの表示
	Sprite* m_Bootht[2];
	Sprite* m_Circle; //サークル表示
	Sprite* m_Number[3];
	float m_num = 0;
	int   m_setnum = 0;
	int   oldnum = 0;
	int	  m_Minute = 0;
	char        TextName[255];

	D3DXVECTOR2  m_Meterpos = { 300.0f,40.0f };
	Pad     m_pad;
	int     m_Speed;   //meterの数字格納
	D3DXVECTOR2  m_BootSize = { 60.0f,-400.0f };
	D3DXVECTOR2  m_HpSize = { 50.0f,-390.0f };
	float test = 0;
};

