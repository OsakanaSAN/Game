#pragma once
#include "myEngine/Graphics/Sprite/Primitive.h"
#include "Monochrome.h"
#include "ZoomBlur.h"
class CPostEffect
{
public:
	CPostEffect();
	~CPostEffect();
	void InitMainRenderTarget();	//メインレンダリングターゲットを初期化。
	void InitPrimitive();			//板ポリの初期化
	void MainRTToCurrentRT();		//メインレンダリングターゲットの内容を現在のレンダリングターゲットにコピー。
	void LoadShader();				//シェーダーをロード。
	void Render();
	void RenderPrimitive();
	CRenderTarget* GetMainRenderTarget();
private:
	CBloom          bloom;
	CMonochrome		monochro;
	CZoomBlur		zoomBlur;
	LPD3DXEFFECT m_CopyEffct;
	LPD3DXEFFECT m_SepiaEffct;			//セピア調のフィルターをするシェーダー。
	bool		Zoom = false;
};

extern CPostEffect* g_PostEffect;