#include "stdafx.h"
#include "Map.h"
#include "MapChip.h"

static SMapChipLocInfo mapChipInfo[] = 
{
#include "locationInfo.h"//"Tutorial.h"

};

Map::Map()
{
}


Map::~Map()
{
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
	for (int i = 0; i < mapChipList.size(); i++) {
		mapChipList[i]->Update();
	}
	
}
void Map::Render()
{
	skydoom->Drow();
	//�}�b�v�`�b�v������`��B
	for (int i = 0; i < mapChipList.size(); i++) {
		mapChipList[i]->Draw();
	}
}

void Map::ShadowMapRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix)
{
	//skydoom->Drow();
	//�}�b�v�`�b�v������`��B
	for (int i = 0; i < mapChipList.size(); i++) {
		mapChipList[i]->LightEyePosRender(lightViewMatrix, lightProjMatrix);
	}
}
