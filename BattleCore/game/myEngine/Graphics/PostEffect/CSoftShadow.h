#pragma once
class CSoftShadow
{
public:
	CSoftShadow();
	~CSoftShadow();
	void Render();
	void UpdateWeight(float dispersion);

private:
	LPD3DXEFFECT		m_softEffect;						//!<シェーダーエフェクト。
	CRenderTarget		m_RenderTarget;				//深度を書き込むレンダリングターゲット
	float weights[8];						//!<ガウスブラーで使う重みテーブル。

};

