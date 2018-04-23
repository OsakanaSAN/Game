#pragma once


class ICollider;

//���̏��B
struct RigidBodyInfo {
	RigidBodyInfo() :
		collider(NULL),
		mass(0.0f)
	{
		pos = {0.0f, 0.0f, 0.0f};
		rot = {0.0f, 0.0f, 0.0f, 1.0f};
		localInertia = { 0.0f, 0.0f, 0.0f };
	}
	D3DXVECTOR3 pos;			//���W�B
	D3DXQUATERNION rot;			//��]�B
	D3DXVECTOR3	localInertia;	//���[�J�������B
	ICollider* collider;		//�R���C�_�[�B
	float mass;					//���ʁB

};
//���̃N���X�B
class RigidBody
{
	btRigidBody*			rigidBody;		//���́B
	btDefaultMotionState*	myMotionState;	//���[�V�����X�e�[�g�B
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
