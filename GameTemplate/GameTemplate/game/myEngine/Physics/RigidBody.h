#pragma once


class ICollider;

//剛体情報。
struct RigidBodyInfo {
	RigidBodyInfo() :
		collider(NULL),
		mass(0.0f)
	{
		pos = {0.0f, 0.0f, 0.0f};
		rot = {0.0f, 0.0f, 0.0f, 1.0f};
		localInertia = { 0.0f, 0.0f, 0.0f };
	}
	D3DXVECTOR3 pos;			//座標。
	D3DXQUATERNION rot;			//回転。
	D3DXVECTOR3	localInertia;	//ローカル慣性。
	ICollider* collider;		//コライダー。
	float mass;					//質量。

};
//剛体クラス。
class RigidBody
{
	btRigidBody*			rigidBody;		//剛体。
	btDefaultMotionState*	myMotionState;	//モーションステート。
public:
	RigidBody();
	~RigidBody();
	void Release();
	void Create(RigidBodyInfo& rbInfo);
	btDefaultMotionState* GetMotionState()
	{
		return myMotionState;
	}
	btRigidBody* GetBody()
	{
		return rigidBody;
	}
};

