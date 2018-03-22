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
	//シャドウマップを書き込むための描画処理
	void ShadowMapRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix);
private:
	std::list<MapChip*>	mapChipList;		//マップチップのリスト。
	Skydoom*                skydoom;		//空
	Sea*					sea;			//水面
	CSun*					sun;			//太陽

};

