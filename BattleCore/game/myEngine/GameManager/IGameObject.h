#pragma once
class IGameObject
{
public:
	IGameObject();
	~IGameObject();
	virtual void Update()=0;
	//virtual void Start()=0;
	//virtual void Render()=0;
	
private:


};

