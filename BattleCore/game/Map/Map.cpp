#include "stdafx.h"
#include "Map.h"
#include "MapChip.h"

SMapChipLocInfo mapChipInfo[] =
{

#include "Tutorial2.h"

};
SMapChipLocInfo mapChipInfo2[] =
{

#include "Test.h"

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
	for (int No = 0;No < 4;No++)
	{
		for (const auto& groupEnemy : m_Enemy[No])
		{
			groupEnemy->Delete();

		}
		m_Enemy[No].clear();
	}
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
				m_nomalEnemyCount++;
			}

			else if (mapChipInfo[i].modelName == "PTEnemy")
			{
				CEnemy* enemy = new CEnemy;
				m_Enemy[GroupNo].push_back(enemy);
				enemy->Start();
				enemy->SetPos(mapChipInfo[i].pos);
				enemy->setGroup(true);
				GroupCount++;
				if (GroupCount == 5)
				{
					GroupNo++;
					GroupCount = 0;
				}
				game->GetGameScene()->CountUp(1);
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
			else if (mapChipInfo[i].modelName == "Wall" || mapChipInfo[i].modelName == "Wall2")
			{
				MapChip* mapChip = new MapChip;
				//マップチップの情報を渡して初期化。
				mapChip->Init(mapChipInfo[i]);
				//動的配列にプッシュ。
				NoRenderobj.push_back(mapChip);
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
				m_nomalEnemyCount++;
			}

			else if (mapChipInfo2[i].modelName == "Player")
			{
				game->GetPlayer()->SetPos(mapChipInfo2[i].pos);
			}

			else if (mapChipInfo2[i].modelName == "Sun")
			{
				sun = new CSun;
				sun->Start(mapChipInfo2[i].pos, mapChipInfo2[i].rotation);
			}

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

	GroupNo = 0;
	GroupCount = 5;
}
void Map::Update()
{
	skydoom->Update();
	sun->Update();

	sea->Update();
	//マップチップを一個ずつ更新。
	for (const auto& mapchip : mapChipList)
	{
		mapchip->Update();
	}
	for (const auto& mapchip : NoRenderobj)
	{
		mapchip->Update();
	}
	if (m_nomalEnemyCount == 0 && GroupNo != 3)
	{
		
		if (GroupCount == 0 )
		{
		
			GroupNo++;
			GroupCount = 5;
			return;
		
		}
		else
		{

			auto& EnemyIt = m_Enemy[GroupNo].begin();
			while (EnemyIt != m_Enemy[GroupNo].end()) {
				if ((*EnemyIt)->IsEnd()) {
					//死亡
					//敵の残り数
					if ((*EnemyIt)->GetIsGroup())
					{
						(*EnemyIt)->setGroup(false);
						game->GetGameScene()->CountDown(1);
						GroupCount--;
					}
					EnemyIt++;
				}
				else {
					(*EnemyIt)->Update();
					EnemyIt++;
				}


			}
		}
		
	}
	
}
void Map::Render2D()
{
	if (m_nomalEnemyCount == 0)
	{
		for (const auto& GroupEnemy : game->GetMap()->GetGroupenemy())
		{
			GroupEnemy->Render2D();
		}
	}

}

void Map::Render()
{
	
	
	
	skydoom->Render();
	sun->Render();
	//sea->Render();
	//マップチップを一個ずつ描画。
	for (const auto& mapchip : mapChipList)
	{
		mapchip->Render();
	}

	sea->Render();

	for (const auto& mapchip : NoRenderobj)
	{
		mapchip->Render();
	}

	if (m_nomalEnemyCount == 0 && GroupNo != 3)
	{
		for (const auto& groupEnemy : m_Enemy[GroupNo])
		{
				groupEnemy->Render();

		}
		
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

void Map::RadarMapRender(D3DXMATRIX&  RederCameraMatrix, D3DXMATRIX&	RederCameraProjMatrix)
{
	if (m_nomalEnemyCount == 0 && GroupNo != 3)
	{
		for (const auto& groupEnemy : m_Enemy[GroupNo])
		{
			groupEnemy->MiniMapRender(RederCameraMatrix, RederCameraProjMatrix);

		}

	}

}
