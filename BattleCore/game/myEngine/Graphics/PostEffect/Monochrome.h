#pragma once
#include "myEngine/Graphics/RenderTarget.h"

class CMonochrome
{
public:
	CMonochrome();
	~CMonochrome();
	void Update();
	void Render();

private:

	LPD3DXEFFECT	m_MonochromeEffect;		//シェーダーエフェクト。
	CRenderTarget	m_RenderTarget;			//レンダリングターゲット。
	float			m_AlphaPower = 1.0f;
};

