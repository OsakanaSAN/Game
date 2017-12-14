#include "stdafx.h"
#include "GameObjectManager.h"


GameObjectManager::GameObjectManager()
{
	
}


GameObjectManager::~GameObjectManager()
{
}
void GameObjectManager::AddGameObject(IGameObject*go)
{
	m_gameObjects.push_back(go);
}

void GameObjectManager::Update()
{
	for (IGameObject*go : m_gameObjects)
	{
		go->Update();
	}

}

