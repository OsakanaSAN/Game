#pragma once
#include "myEngine/Graphics/Sprite/Sprite.h"
class CHud
{
public:
	CHud();
	~CHud();
	void Start();
	void Update();
	void Drow(LPD3DXSPRITE spt);
private:
	Sprite* m_boot;   //ブーストゲージの表示
	Sprite* m_Circle; //サークル表示
	D3DXVECTOR2  m_Meterpos = { 300.0f,40.0f };
	Pad     m_pad;
	int     m_Speed;   //meterの数字格納
	D3DXVECTOR2  m_BootSize = { 300.0f,10.0f };
};

