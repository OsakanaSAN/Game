#pragma once
#include "myEngine/GameManager/IGameObject.h"
#include "Skydoom.h"
#include "Sea.h"
#include "Sun.h"

class MapChip;

class Map 
{
public:
	Map();
	~Map();
	void Start();
	void Init(int mapserect);
	void Update();
	void Render();
	CSun* GetSun()
	{
		return sun;
	}
	const std::list<CEnemy*> GetGroupenemy()
	{
		return m_Enemy[GroupNo];
	}
	//シャドウマップを書き込むための描画処理
	void ShadowMapRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix);
	void RadarMapRender(D3DXMATRIX&  RederCameraMatrix, D3DXMATRIX&	RederCameraProjMatrix);
private:
	std::list<MapChip*>	mapChipList;		//マップチップのリスト。
	Skydoom*                skydoom;		//空
	Sea*					sea;			//水面
	CSun*					sun;			//太陽
	std::list<CEnemy*>		m_Enemy[4];		//グループ
	int						GroupCount = 0;
	int						GroupNo = 0;

};

