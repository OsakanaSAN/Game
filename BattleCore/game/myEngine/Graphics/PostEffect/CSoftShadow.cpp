#include "stdafx.h"
#include "CSoftShadow.h"

void CSoftShadow::UpdateWeight(float dispersion)
{
	float total = 0;
	for (int i = 0; i<8; i++) {
		weights[i] = expf(-0.5f*(float)(i*i) / dispersion);
		total += 2.0f*weights[i];

	}
	// 規格化
	for (int i = 0; i < 8; i++) {
		weights[i] /= total;
	}
}

CSoftShadow::CSoftShadow()
{
	
	
	m_softEffect = g_effectManager->LoadEffect("Assets/Shader/SoftShadow.fx");

}


CSoftShadow::~CSoftShadow()
{
}


void CSoftShadow::Render()
{
	if (game->GetPlayer() == NULL) { return; }
	// Zテストで失敗してもらったら困るので、Zテストは無効にしておく。
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// αブレンドもいらない。
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//CRenderTarget* rt = g_PostEffect->GetMainRenderTarget();

	LPDIRECT3DSURFACE9 renderTargetBackup;
	g_pd3dDevice->GetRenderTarget(0, &renderTargetBackup);		//元々のレンダリングターゲットを保存。後で戻す必要があるので。

	UpdateWeight(25.0f);
																//レンダリングターゲットを変更する。
	g_pd3dDevice->SetRenderTarget(0, g_shadowMap.GetRenderTarget().GetRenderTarget());
	m_softEffect->SetTechnique("XBlur");
	m_softEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_softEffect->BeginPass(0);

	//輝度のテクスチャサイズ転送
	float size[2] = {
		(float)(1280.0f),
		(float)(720.0f)
	};
	m_softEffect->SetValue("g_luminanceTexSize", size, sizeof(size));

	//オフセット転送
	float offset[] = {
		16.0f / size[0],
		0.0f
	};
	m_softEffect->SetValue("g_offset", offset, sizeof(size));

	//レンダリングターゲットのサイズを転送
	float renderTargetSize[2];
	renderTargetSize[0] = (float)g_shadowMap.GetRenderTarget().GetWidth();
	renderTargetSize[1] = (float)g_shadowMap.GetRenderTarget().GetHeight();
	m_softEffect->SetValue("g_renderTargetSize", renderTargetSize, sizeof(renderTargetSize));

	m_softEffect->SetValue("g_weight", weights, sizeof(weights));
	//シーンテクスチャを設定する
	m_softEffect->SetTexture("g_scene", g_shadowMap.GetTexture(0));
	//定数レジスタへの変更をコミットする。
	m_softEffect->CommitChanges();
	g_PostEffect->RenderPrimitive();

	m_softEffect->EndPass();
	m_softEffect->End();

	// 変更したレンダリングステートを元に戻す。
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


	g_pd3dDevice->SetRenderTarget(0, renderTargetBackup);
	


}
