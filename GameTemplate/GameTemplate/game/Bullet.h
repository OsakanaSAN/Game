#pragma once
#include "IGameObject.h"
class Bullet
{
public:
	Bullet();
	~Bullet();
	void Start(const D3DXVECTOR3& pos, const D3DXVECTOR3& moveSpeed);
	bool Update();
	void Render();
	D3DXVECTOR3 Getops() const
	{
		return position;
	}
	D3DXMATRIX getMatrix() const
	{
		return Mat;
	}
	D3DVECTOR GetSpeed()
	{
		return moveSpeed;
	}
private:
	SkinModel model;
	D3DXMATRIX  Mat;
	static SkinModelData* modelData;
	Animation animation;
	Light light;
	D3DXVECTOR3 position;
	D3DXVECTOR3 moveSpeed;
	int life;		//éıñΩÅB
};

