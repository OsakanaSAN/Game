#pragma once
class CDof
{
public:
	CDof();
	~CDof();
	void Render();

private:
	LPD3DXEFFECT		m_DofEffect;						//!<シェーダーエフェクト。
	CRenderTarget		m_DepthRenderTarget;				//深度を書き込むレンダリングターゲット
	CRenderTarget		m_combineRenderTarget;				//ボケ合成用のレンダリングターゲット。


};

