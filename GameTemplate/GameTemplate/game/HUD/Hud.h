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
	Sprite* m_boot;   //�u�[�X�g�Q�[�W�̕\��
	Sprite* m_Circle; //�T�[�N���\��
	D3DXVECTOR2  m_Meterpos = { 300.0f,40.0f };
	Pad     m_pad;
	int     m_Speed;   //meter�̐����i�[
	D3DXVECTOR2  m_BootSize = { 300.0f,10.0f };
};

