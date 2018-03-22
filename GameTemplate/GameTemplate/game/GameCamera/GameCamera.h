#pragma once
#include "myEngine/Camera/CameraCollisionSolver.h"
#include "myEngine/Camera/SpringCamera.h"
class GameCamera
{
public:
	GameCamera();
	~GameCamera();
	void Strat();  //初期化
	void Update(); //更新
	void Render(); ///描画
	void CameraAngle(); //カメラの回転

	//プレイヤーとの距離を入れる
	void SetPLength(float leng)
	{
		Plength = leng;
	}

	//カメラを渡す
	Camera* Getcamera()
	{
		return Springcamera.GetCamera();
	}

	SpringCamera* GetSpringGamera()
	{
		return	&Springcamera;
	}
	
	//プレやーがロックオンした時に動く関数
	void LookOnCamera(D3DXVECTOR3 LookPos, bool on)
	{
		LookOn = on;
		m_LookTag = LookPos;
	}
	D3DXMATRIX GetCameraMatrix()
	{

		return m_rot;
	}
private:
	Camera          camera;							//カメラ
	SpringCamera    Springcamera;					//バネカメラ

	D3DXVECTOR3     Eyepos = { 12.0f,0.0f, 12.0f }; //カメラの位置
	D3DXVECTOR3     Up =	{ 0.0f,1.0f,0.0f };		//カメラの上方向のベクトル
	D3DXVECTOR3     toEyePos;						//注視点から視点までのベクトル。
	D3DXVECTOR3     Lookpos;						//注視点
	D3DXVECTOR3     Vpos;							//回転
	D3DXMATRIX		m_rot;

	CharacterController  CameraControl;
	D3DXVECTOR3     OldPlayerPos;
	CameraCollisionSolver CameraCol;
	SpringCamera          Spcamera;

	float           Plength; //プレイヤーとの距離
	float           Time;    //時間
	Pad             pad;     //パット
	bool            QuickTurn = false;  //Quickターン
	float             Quick = 0;
	bool           LookOn = false;
	D3DXVECTOR3    m_LookTag;
};

