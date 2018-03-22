#pragma once

#include "myEngine/Physics/MeshCollider.h"
#include "myEngine/Physics/RigidBody.h"

//�}�b�v�`�b�v�̔z�u���B
struct SMapChipLocInfo {
	const char* modelName;		//���f���B
	D3DXVECTOR3	pos;			//���W�B
	D3DXQUATERNION	rotation;		//��]�B
};

class MapChip
{
public:
	MapChip();
	~MapChip();
	void Init(SMapChipLocInfo& locInfo);
	void Update();
	void Render();
	
	void LightEyePosRender(D3DXMATRIX  lightViewMatrix, D3DXMATRIX	lightProjMatrix);
private:
	SkinModel		model;							//�X�L�����f��
	SkinModelData	modelData;						//�X�L�����f���f�[�^
	D3DXVECTOR3		position;						//���W				
	D3DXQUATERNION rotation;						//���[�e�[�V����
	Light light;
	MeshCollider meshCollider;	//���b�V���R���C�_�[�B
	RigidBody* rigidBody;		//���́B
	bool      Sea;
	D3DXVECTOR4 Up = { 0.75f,0.0f,0.0f,1.0f };
	int         lightNo = 0;
	LPDIRECT3DCUBETEXTURE9      CubeMap = NULL;
};