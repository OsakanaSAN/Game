#pragma once
#include "IGameObject.h"


class GameObjectManager
{
public:
	GameObjectManager();
	~GameObjectManager();
	void AddGameObject(IGameObject* go);
	
	void Update();
private:
	std::vector<IGameObject*>    m_gameObjects;
};

