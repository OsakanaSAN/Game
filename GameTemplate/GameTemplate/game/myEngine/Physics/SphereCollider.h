#pragma once
#include "myEngine\Physics\ICollider.h"
class SphereCollider : public ICollider
{
public:
	/*!
	* @brief	�R���X�g���N�^�B
	*/
	SphereCollider();
	/*!
	* @brief	�f�X�g���N�^�B
	*/
	~SphereCollider();
	/*!
	* @brief	���̃R���C�_�[���쐬�B
	*/
	void Create(const float radius);
	btCollisionShape* GetBody() override
	{
		return shape;
	}
private:
	btSphereShape*		shape;
};

