#pragma once
#include "IGameObject.h"
#include "myEngine\Graphics\SkinModel.h"
#include "myEngine\HID\Pad.h"
#include "myEngine\Physics\CharacterController.h"
#include "myEngine\Graphics\Camera.h"
#include "Bullet.h"

class Player : public IGameObject
{
public:
	Player();
	~Player();
	void Start();
	void Update();
	void Render();
	void MovePlayer();
	void AnglePlayer();
	void Attention();
	D3DXVECTOR3 GetPos()
	{
		return position;
	}

	void Animetion();
private:
	SkinModel             skinmodel;
	SkinModelData         skinmodelData;
	Light                 PLight;
	D3DXVECTOR3           position = { 0.0f,1.0f,0.0f };
	D3DXVECTOR3           Bespos;
	D3DXQUATERNION        rotation = { 0.0f, 0.0f, 0.0f, 1.0f };
	Animation             animation;    //�A�j���[�V����
	Pad                   pad;          //�p�b�g
	int                   AnimationNo;  //���݂̃A�j���[�V�����ԍ�
	Camera*               camera;       //�J����
	bool                  IsMove = false;
	CharacterController   characterController;
	int bulletFireInterval;	//�e�ۂ�������x�łĂ�悤�ɂȂ�܂ł̃C���^�[�o��
	D3DXVECTOR3           oldDir;
	int                   DashTime = 0;
	float                 SpeedDown = 1.0f;
	bool                  ZAttent = false;
	
};

