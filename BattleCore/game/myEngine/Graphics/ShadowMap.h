#pragma once
#include "RenderTarget.h"

class CShadowMap
{
public:
	CShadowMap();
	~CShadowMap();
	//テクスチャを取得。
	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return SrenderTarget[0].GetTexture();
	}
	//初期化。
	void Init();
	//ライトビューの視点を設定。
	void SetLightViewPosition(const D3DXVECTOR3& lightViewPosition)
	{
		viewPosition = lightViewPosition;
	}
	//ライトビューの注視点を設定。
	void SetLightViewTarget(const D3DXVECTOR3& lightViewTarget)
	{
		viewTarget = lightViewTarget;
	}
	//ライトビュー行列を取得。
	const D3DXMATRIX& GetLightViewMatrix()
	{
		return lightViewMatrix;
	}
	//ライトプロジェクション行列を取得。
	const D3DXMATRIX& GetLightProjectionMatrix()
	{
		return lightProjMatrix;
	}
	

	//更新。
	void Update();
	//シャドウマップに書き込み。
	void Draw();
private:
	static const int			NUM_SHADOW_MAP = 3;
	CRenderTarget SrenderTarget[NUM_SHADOW_MAP];		//シャドウマップを書きこむレンダリングターゲット。
	D3DXMATRIX  lightViewMatrix;	//ライトビューマトリクス。
	D3DXMATRIX	lightProjMatrix;	//ライトプロジェクションマトリクス。
	D3DXVECTOR3 viewPosition;		//ライトビューの視点。
	D3DXVECTOR3 viewTarget;			//ライトビューの注視点。
	D3DXVECTOR3 m_lightDirection;	//!<ライトの向き
	Camera      Scamera;            //カメラ

	float        Movelight = 0.0f;       //ライトの視点を移動させる
	float					Far;				//!<遠平面。
	float					Near;				//!<近平面。
	float			        m_viewAngle;		//!<画角(ラジアン)。
	float					aspect;				//!<アスペクト比
};

