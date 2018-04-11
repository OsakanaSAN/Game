#pragma once
#include "myEngine/Graphics/RenderTarget.h"
#include "myEngine/Camera/CameraCollisionSolver.h"
#include "myEngine/Camera/SpringCamera.h"
#include "myEngine/Graphics/Sprite/Primitive.h"


class CRadarMap
{
public:
	CRadarMap();
	~CRadarMap();
	void InitRender();
	void Update(const D3DXVECTOR3 position);
	void Draw(SkinModel* skinmodel);
	//テクスチャを取得。
	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return m_SrenderTarget.GetTexture();
	}


private:
	CPrimitive		m_primitive;
	CRenderTarget m_SrenderTarget;		//レーダーマップを書きこむレンダリングターゲット。
	D3DXMATRIX  m_lightViewMatrix;		//ライトビューマトリクス。
	D3DXMATRIX	m_lightProjMatrix;		//ライトプロジェクションマトリクス。
	D3DXVECTOR3 m_viewPosition;			//ライトビューの視点。
	D3DXVECTOR3 m_viewTarget;			//ライトビューの注視点。
	D3DXVECTOR3 m_lightDirection;		//!<ライトの向き
	Camera      m_Scamera;							//カメラ
	SpringCamera    m_Springcamera;					//バネカメラ
	CameraCollisionSolver CameraCol;				//剛体付きカメラ
	LPDIRECT3DTEXTURE9	texture;			//!<テクスチャ。
	ID3DXEffect*		shaderEffect;		//!<シェーダーエフェクト。


	D3DXVECTOR3     Eyepos =	{0.0f,500.0f, 0.0f }; //カメラの位置
	D3DXVECTOR3     m_targetpos = { 0.0f,0.0f, 0.0f }; //カメラの位置
	D3DXMATRIX		m_rot;

	float					m_Movelight = 0.0f; //ライトの視点を移動させる
	float					m_Far;				//!<遠平面。
	float					m_Near;				//!<近平面。
	float			        m_viewAngle;		//!<画角(ラジアン)。
	float					m_aspect;			//!<アスペクト比
	Pad						pad;     //パット
};

