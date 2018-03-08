#pragma once
class CZoomBlur
{
public:
	CZoomBlur();
	~CZoomBlur();
	void Render();

private:

	LPD3DXEFFECT	m_ZoomEffect;		//シェーダーエフェクト。
	CRenderTarget	m_ZoomRenderTarget;			//レンダリングターゲット。
	float			m_BlurPower = 0.0f;

};

