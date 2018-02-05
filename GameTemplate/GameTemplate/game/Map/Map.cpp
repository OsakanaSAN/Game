#include "stdafx.h"
#include "Map.h"
#include "MapChip.h"

static SMapChipLocInfo mapChipInfo[] = 
{
#include /*"locationInfo.h"*/"Tutorial.h"

};

Map::Map()
{
}


Map::~Map()
{
	delete skydoom;
	for (MapChip* mapchip : mapChipList)
	{
		delete mapchip;
	}
	mapChipList.clear();
}
void Map::Start()
{

}

void Map::Init()
{
	//�z�u����Ă���I�u�W�F�N�g�̐����v�Z�B
	int numObject = sizeof(mapChipInfo) / sizeof(mapChipInfo[0]);
	for (int i = 0; i < numObject; i++) {
		//�I�u�W�F�N�g��������[�h���Ă����B
		//�܂���MapChip�̃C���X�^���X�𓮓I�ɐ����B
		if (mapChipInfo[i].modelName == "skydoom")
		{
			skydoom = new Skydoom;
			skydoom->Init(mapChipInfo[i].pos, mapChipInfo[i].rotation);

		}
		//�G�̐���
		if (mapChipInfo[i].modelName == "Enemy")
		{
			game->InitEnemy(mapChipInfo[i].pos);
			game->GetGameScene()->CountUp(1);
		}

		else if (mapChipInfo[i].modelName == "Player")
		{
			game->GetPlayer()->SetPos(mapChipInfo[i].pos);
		}
		else {
			MapChip* mapChip = new MapChip;
			//�}�b�v�`�b�v�̏���n���ď������B
			mapChip->Init(mapChipInfo[i]);
			//���I�z��Ƀv�b�V���B
			mapChipList.push_back(mapChip);
		}
	}
}
void Map::Update()
{
	skydoom->Update();
	//�}�b�v�`�b�v������X�V�B
	for (MapChip* mapchip : mapChipList)
	{
		mapchip->Update();
	}
	
}
void Map::Render()
{
	skydoom->Drow();
	//�}�b�v�`�b�v������`��B
	for (MapChip* mapchip : mapChipList)
	{
		mapchip->Render();
	}
}

void Map::ShadowMapRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix)
{
	//skydoom->Drow();
	//�}�b�v�`�b�v������`��B
	for (MapChip* mapchip : mapChipList)
	{
		mapchip->LightEyePosRender(lightViewMatrix, lightProjMatrix);
	}
}
