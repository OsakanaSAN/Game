#include "stdafx.h"
#include "Map.h"
#include "MapChip.h"



Map::Map()
{
	

}


Map::~Map()
{
	delete skydoom;
	delete sea;
	delete sun;
	for (const auto& mapchip : mapChipList)
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
	SMapChipLocInfo mapChipInfo[] =
	{

		#include "Tutorial.h"

	};

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

		else if (mapChipInfo[i].modelName == "Wave")
		{
			sea = new Sea;
			sea->Start(mapChipInfo[i].pos, mapChipInfo[i].rotation);


		}

		//�G�̐���
		else if (mapChipInfo[i].modelName == "Enemy")
		{
			game->InitEnemy(mapChipInfo[i].pos);
			game->GetGameScene()->CountUp(1);
			game->GetScorecheckre()->CountUp();
		}

		else if (mapChipInfo[i].modelName == "Player")
		{
			game->GetPlayer()->SetPos(mapChipInfo[i].pos);
		}

		else if (mapChipInfo[i].modelName == "Sun")
		{
			sun = new CSun;
			sun->Start(mapChipInfo[i].pos, mapChipInfo[i].rotation);
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
	for (const auto& mapchip : mapChipList)
	{
		mapchip->Update();
	}
	sea->Update();
	sun->Update();
}
void Map::Render()
{
	
	skydoom->Render();
	sun->Render();
	

	//�}�b�v�`�b�v������`��B
	for (const auto& mapchip : mapChipList)
	{
		mapchip->Render();
	}

	sea->Render();
	
}

void Map::ShadowMapRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix)
{
	//skydoom->Drow();
	//�}�b�v�`�b�v������`��B
	for (const auto& mapchip : mapChipList)
	{
		mapchip->LightEyePosRender(lightViewMatrix, lightProjMatrix);
	}
}
