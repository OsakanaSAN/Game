#include "stdafx.h"
#include "Map.h"
#include "MapChip.h"

SMapChipLocInfo mapChipInfo[] =
{

#include "Tutorial.h"

};
SMapChipLocInfo mapChipInfo2[] =
{

#include "locationInfo.h"

};

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

void Map::Init(int mapserect)
{
	int numObject;
	switch (mapserect)
	{
	case 1:
		//配置されているオブジェクトの数を計算。
		numObject= sizeof(mapChipInfo) / sizeof(mapChipInfo[0]);
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
		break;

	case 2:
		//配置されているオブジェクトの数を計算。
		numObject = sizeof(mapChipInfo2) / sizeof(mapChipInfo2[0]);
		for (int i = 0; i < numObject; i++) {
			//オブジェクトを一個ずつロードしていく。
			//まずはMapChipのインスタンスを動的に生成。
			if (mapChipInfo2[i].modelName == "skydoom")
			{
				skydoom = new Skydoom;
				skydoom->Init(mapChipInfo2[i].pos, mapChipInfo2[i].rotation);

			}

			else if (mapChipInfo2[i].modelName == "Wave")
			{
				sea = new Sea;
				sea->Start(mapChipInfo2[i].pos, mapChipInfo2[i].rotation);


			}

			//敵の生成
			else if (mapChipInfo2[i].modelName == "Enemy")
			{
				game->InitEnemy(mapChipInfo2[i].pos);
				game->GetGameScene()->CountUp(1);
				game->GetScorecheckre()->CountUp();
			}

			else if (mapChipInfo2[i].modelName == "Player")
			{
				game->GetPlayer()->SetPos(mapChipInfo2[i].pos);
			}

			/*else if (mapChipInfo2[i].modelName == "Sun")
			{
				sun = new CSun;
				sun->Start(mapChipInfo2[i].pos, mapChipInfo2[i].rotation);
			}*/

			else
			{

				MapChip* mapChip = new MapChip;
				//マップチップの情報を渡して初期化。
				mapChip->Init(mapChipInfo2[i]);
				//動的配列にプッシュ。
				mapChipList.push_back(mapChip);
			}
		}

	}
}
void Map::Update()
{
	skydoom->Update();
	
	sea->Update();
	//マップチップを一個ずつ更新。
	for (const auto& mapchip : mapChipList)
	{
		mapchip->Update();
	}
	
}
void Map::Render()
{
	
	
	
	skydoom->Render();
	sea->Render();
	//マップチップを一個ずつ描画。
	for (const auto& mapchip : mapChipList)
	{
		mapchip->Render();
	}

	
	
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
