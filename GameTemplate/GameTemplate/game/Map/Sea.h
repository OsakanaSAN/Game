#pragma once
class Sea
{
public:
	Sea();
	~Sea();
	void Start(D3DXVECTOR3 pos, D3DXQUATERNION rot);
	void Update();
	void Render();
private:
	SkinModel model;
	SkinModelData modelData;
	D3DXVECTOR3	position;
	D3DXQUATERNION rotation;
	Light light;
};

