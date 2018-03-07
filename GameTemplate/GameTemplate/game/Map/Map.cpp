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

	//配置されているオブジェクトの数を計算。
	int numObject = sizeof(mapChipInfo) / sizeof(mapChipInfo[0]);
	for (int i = 0; i < numObject; i++) {
		//オブジェクトを一個ずつロードしていく。
		//まずはMapChipのインスタンスを動的に生成。
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

		//敵の生成
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
			//マップチップの情報を渡して初期化。
			mapChip->Init(mapChipInfo[i]);
			//動的配列にプッシュ。
			mapChipList.push_back(mapChip);
		}
	}
}
void Map::Update()
{
	skydoom->Update();
	

	//マップチップを一個ずつ更新。
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
	

	//マップチップを一個ずつ描画。
	for (const auto& mapchip : mapChipList)
	{
		mapchip->Render();
	}

	sea->Render();
	
}

void Map::ShadowMapRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix)
{
	//skydoom->Drow();
	//マップチップを一個ずつ描画。
	for (const auto& mapchip : mapChipList)
	{
		mapchip->LightEyePosRender(lightViewMatrix, lightProjMatrix);
	}
}
