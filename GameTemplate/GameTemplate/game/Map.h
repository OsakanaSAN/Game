#pragma once
#include "IGameObject.h"

class MapChip;

class Map : public IGameObject
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
};

