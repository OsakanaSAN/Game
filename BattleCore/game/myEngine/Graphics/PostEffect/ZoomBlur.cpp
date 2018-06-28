#include "stdafx.h"
#include "ZoomBlur.h"


CZoomBlur::CZoomBlur()
{
	m_ZoomRenderTarget.Create(
		1280,					//フレームバッファの幅
		720,					//フレームバッファの高さ
		1,						//ミップマップレベル
		D3DFMT_A8B8G8R8,	//カラーバッファのフォーマット
		D3DFMT_D24S8,			//デプスステンシルバッファのフォーマット
		D3DMULTISAMPLE_NONE,	//マルチサンプリングの種類
		0						//マルチサンプリングの品質
	);

	m_ZoomEffect = g_effectManager->LoadEffect("Assets/Shader/ZoomFilter.fx");

}


CZoomBlur::~CZoomBlur()
{

}

void CZoomBlur::Render()
{

	// Zテストで失敗してもらったら困るので、Zテストは無効にしておく。
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// αブレンドもいらない。
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	CRenderTarget* rt = g_PostEffect->GetMainRenderTarget();

	g_pd3dDevice->SetRenderTarget(0, rt->GetRenderTarget());
	g_pd3dDevice->SetDepthStencilSurface(rt->GetDepthStencilBuffer());

	//Monochromeを設定。
	m_ZoomEffect->SetTechnique("Default");
	m_ZoomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_ZoomEffect->BeginPass(0);
	//シーンテクスチャを設定する
	m_ZoomEffect->SetTexture("g_scene",rt->GetTexture());
	m_ZoomEffect->SetFloat("g_BlurPower", 5.0f);
	//定数レジスタへの変更をコミットする。
	m_ZoomEffect->CommitChanges();
	g_PostEffect->RenderPrimitive();

	m_ZoomEffect->EndPass();
	m_ZoomEffect->End();

	//セピア
	//m_SepiaEffct->EndPass();
	//m_SepiaEffct->End();
	// 変更したレンダリングステートを元に戻す。
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

}
