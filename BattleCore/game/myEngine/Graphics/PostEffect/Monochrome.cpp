#include "stdafx.h"
#include "Monochrome.h"


CMonochrome::CMonochrome()
{
	

	m_RenderTarget.Create(
		1280,					//フレームバッファの幅
		720,					//フレームバッファの高さ
		1,					//ミップマップレベル
		D3DFMT_A16B16G16R16F,	//カラーバッファのフォーマット
		D3DFMT_D24S8,			//デプスステンシルバッファのフォーマット
		D3DMULTISAMPLE_NONE,	//マルチサンプリングの種類
		0						//マルチサンプリングの品質
	);

	m_MonochromeEffect = g_effectManager->LoadEffect("Assets/Shader/Monochrome.fx");

}


CMonochrome::~CMonochrome()
{

	
}

void CMonochrome::Render()
{
	if (game->GetPlayer() == NULL || game->GetPlayer()->GetHp() >= 100)
	{
		m_AlphaPower = 1.0f;
		return; }
	if (m_AlphaPower >= 0)
	{
		m_AlphaPower -= GameTime().GetFrameDeltaTime() / 2;
	}
	// Zテストで失敗してもらったら困るので、Zテストは無効にしておく。
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// αブレンドもいらない。
	//g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	CRenderTarget* rt = g_PostEffect->GetMainRenderTarget();

	g_pd3dDevice->SetRenderTarget(0, rt->GetRenderTarget());
	g_pd3dDevice->SetDepthStencilSurface(rt->GetDepthStencilBuffer());

	//Monochromeを設定。
	m_MonochromeEffect->SetTechnique("Default");
	m_MonochromeEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_MonochromeEffect->BeginPass(0);
	//シーンテクスチャを設定する
	m_MonochromeEffect->SetTexture("g_scene", rt->GetTexture());
	m_MonochromeEffect->SetFloat("g_sceneTexSizeX", 1280.0f);
	m_MonochromeEffect->SetFloat("g_sceneTexSizeY", 720.0f);
	m_MonochromeEffect->SetFloat("g_AlphaPower",m_AlphaPower);
	//定数レジスタへの変更をコミットする。
	m_MonochromeEffect->CommitChanges();
	g_PostEffect->RenderPrimitive();
	
	m_MonochromeEffect->EndPass();
	m_MonochromeEffect->End();

	//セピア
	//m_SepiaEffct->EndPass();
	//m_SepiaEffct->End();
	// 変更したレンダリングステートを元に戻す。
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}
