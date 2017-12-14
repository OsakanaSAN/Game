#pragma once
class CRadarMap
{
public:
	CRadarMap();
	~CRadarMap();
	void InitRender();
	void Update();
	void Draw();


private:

	CRenderTarget m_SrenderTarget;		//レーダーマップを書きこむレンダリングターゲット。
	D3DXMATRIX  m_lightViewMatrix;		//ライトビューマトリクス。
	D3DXMATRIX	m_lightProjMatrix;		//ライトプロジェクションマトリクス。
	D3DXVECTOR3 m_viewPosition;			//ライトビューの視点。
	D3DXVECTOR3 m_viewTarget;			//ライトビューの注視点。
	D3DXVECTOR3 m_lightDirection;		//!<ライトの向き
	Camera      m_Scamera;				//カメラ
	float					m_Movelight = 0.0f; //ライトの視点を移動させる
	float					m_Far;				//!<遠平面。
	float					m_Near;				//!<近平面。
	float			        m_viewAngle;		//!<画角(ラジアン)。
	float					m_aspect;			//!<アスペクト比

};

