#pragma once
#include "IGameObject.h"
#include "Skydoom.h"

class MapChip;

class Map 
{
public:
	Map();
	~Map();
	void Start();
	void Init();
	void Update();
	void Render();
private:
	std::vector<MapChip*>	mapChipList;	//マップチップのリスト。
	Skydoom*                skydoom;
};

