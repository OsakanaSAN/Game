#pragma once
#include "myEngine/Camera/CameraCollisionSolver.h"
#include "myEngine/Camera/SpringCamera.h"
class GameCamera
{
public:
	GameCamera();
	~GameCamera();
	void Strat();  //������
	void Update(); //�X�V
	void Render(); ///�`��
	void CameraAngle(); //�J�����̉�]

	//�v���C���[�Ƃ̋���������
	void SetPLength(float leng)
	{
		Plength = leng;
	}

	//�J������n��
	Camera* Getcamera()
	{
		return Springcamera.GetCamera();
	}
	
	//�v����[�����b�N�I���������ɓ����֐�
	void LookOnCamera(D3DXVECTOR3 LookPos, bool on)
	{
		LookOn = on;
		LookTag = LookPos;
	}
private:
	Camera          camera;  //�J����
	SpringCamera    Springcamera; //�o�l�J����

	D3DXVECTOR3     Eyepos = { 3.0f,0.0f, 3.0f };  //�J�����̈ʒu
	D3DXVECTOR3     Up = { 0.0f,1.0f,0.0f };
	D3DXVECTOR3     toEyePos;//�����_���王�_�܂ł̃x�N�g���B
	D3DXVECTOR3     Lookpos; //�����_
	D3DXVECTOR3     Vpos;    //��]
	CharacterController  CameraControl;
	D3DXVECTOR3     OldPlayerPos;
	CameraCollisionSolver CameraCol;
	SpringCamera          Spcamera;

	float           Plength; //�v���C���[�Ƃ̋���
	float           Time;    //����
	Pad             pad;     //�p�b�g
	bool            QuickTurn = false;  //Quick�^�[��
	float             Quick = 0;
	bool           LookOn = false;
	D3DXVECTOR3    LookTag;
};

