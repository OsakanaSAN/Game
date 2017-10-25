#pragma once

#include "myEngine/Physics/MeshCollider.h"
#include "myEngine/Physics/RigidBody.h"

//マップチップの配置情報。
struct SMapChipLocInfo {
	const char* modelName;		//モデル。
	D3DXVECTOR3	pos;			//座標。
	D3DXQUATERNION	rotation;		//回転。
};

class MapChip
{
public:
	MapChip();
	~MapChip();
	void Init(SMapChipLocInfo& locInfo);
	void Update();
	void Draw();
private:
	SkinModel model;
	SkinModelData modelData;
	D3DXVECTOR3	position;
	D3DXQUATERNION rotation;
	Light light;
	MeshCollider meshCollider;	//メッシュコライダー。
	RigidBody rigidBody;		//剛体。
	bool      Sea;
	D3DXVECTOR4 Up = { 0.75f,0.0f,0.0f,1.0f };
	int         lightNo = 0;
};