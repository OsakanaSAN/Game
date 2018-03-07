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
	void Init();
	void Update();
	void Render();
	void ShadowMapRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix);
private:
	std::list<MapChip*>	mapChipList;	//�}�b�v�`�b�v�̃��X�g�B
	Skydoom*                skydoom;
	Sea*					sea;
	CSun*					sun;
};

