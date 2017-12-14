#include "stdafx.h"
#include "Bloom.h"


void CBloom::UpdateWeight(float dispersion)
{
	float total = 0;
	for (int i = 0; i<NUM_WEIGHTS; i++) {
		weights[i] = expf(-0.5f*(float)(i*i) / dispersion);
		total += 2.0f*weights[i];

	}
	// 規格化
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		weights[i] /= total;
	}
}

CBloom::CBloom()
{
	//ブルームのシェーダーをロード
	m_BloomEffect = g_effectManager->LoadEffect("Assets/Shader/Bloom.fx");

	//輝度抽出用のレンダリングターゲットを作成する。
	m_luminanceRenderTarget.Create(
		1280,		//幅と高さはフレームバッファと同じにしておく。
		 720,
		1,
		D3DFMT_A16B16G16R16F,	//ここも浮動小数点バッファにする。
		D3DFMT_D16,				//使わないので16bit。本来は作成する必要もない。
		D3DMULTISAMPLE_NONE,	//マルチサンプリングの種類。今回はマルチサンプリングは行わないのでD3DMULTISAMPLE_NONEでいい。
		0						//マルチサンプリングの品質レベル。今回はマルチサンプリングは行わないので0でいい。
	);

	int W = 1280;
	int H = 720;
	for (int i = 0;i < MGF_DOWN_SAMPLE_COUNT;i++)
	{
		//ブラーをかけるためのダウンサンプリング用のレンダリングターゲットを作成。
		//横ブラー用。
		W /= 2;
		m_downSamplingRenderTarget[i][0].Create(
			W,	//横の解像度をフレームバッファの半分にする。
			H,
			1,
			D3DFMT_A16B16G16R16F,	//ここも浮動小数点バッファにする。
			D3DFMT_D16,				//使わないので16bit。本来は作成する必要もない。
			D3DMULTISAMPLE_NONE,	//マルチサンプリングの種類。今回はマルチサンプリングは行わないのでD3DMULTISAMPLE_NONEでいい。
			0						//マルチサンプリングの品質レベル。今回はマルチサンプリングは行わないので0でいい。
		);
		H /= 2;
		//縦ブラー用。
		m_downSamplingRenderTarget[i][1].Create(
			W,	//縦と横の解像度をフレームバッファの半分にする。
			H,
			1,
			D3DFMT_A16B16G16R16F,	//ここも浮動小数点バッファにする。
			D3DFMT_D16,				//使わないので16bit。本来は作成する必要もない。
			D3DMULTISAMPLE_NONE,	//マルチサンプリングの種類。今回はマルチサンプリングは行わないのでD3DMULTISAMPLE_NONEでいい。
			0						//マルチサンプリングの品質レベル。今回はマルチサンプリングは行わないので0でいい。
		);
	}
}
CBloom::~CBloom()
{
}

void CBloom::Render()
{

	// Zテストで失敗してもらったら困るので、Zテストは無効にしておく。
	//g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// αブレンドもいらない。
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	CRenderTarget* rt = g_PostEffect->GetMainRenderTarget();
	//まずは輝度を抽出する。
	{
		
		//輝度抽出用のレンダリングターゲットに変更する。
		g_pd3dDevice->SetRenderTarget(0, m_luminanceRenderTarget.GetRenderTarget());
		g_pd3dDevice->SetDepthStencilSurface(m_luminanceRenderTarget.GetDepthStencilBuffer());
		//黒でクリア。
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		// 輝度抽出テクニックをセット。
		m_BloomEffect->SetTechnique("SamplingLuminance");
		m_BloomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		m_BloomEffect->BeginPass(0);
		//シーンテクスチャを設定する。
		m_BloomEffect->SetTexture("g_scene",rt->GetTexture());
		//定数レジスタへの変更をコミット。
		m_BloomEffect->CommitChanges();
		g_PostEffect->RenderPrimitive();

		m_BloomEffect->EndPass();
		m_BloomEffect->End();
		// 変更したレンダリングステートを元に戻す。
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	}

	//ガウスブラーで使う重みテーブルを更新。
	UpdateWeight(1.0f);
	//輝度を抽出したテクスチャをXブラー
	for (int i = 0;i <MGF_DOWN_SAMPLE_COUNT;i++)
	{
		{
			g_pd3dDevice->SetRenderTarget(0, m_downSamplingRenderTarget[i][0].GetRenderTarget());

			m_BloomEffect->SetTechnique("XBlur");
			m_BloomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			m_BloomEffect->BeginPass(0);
			float size[2] = {
				(float)(m_luminanceRenderTarget.GetWidth()),
				(float)(m_luminanceRenderTarget.GetHeight())
			};
			float offset[] = {
				16.0f / (float)(m_luminanceRenderTarget.GetWidth()),
				0.0f
			};
			m_BloomEffect->SetValue("g_luminanceTexSize", size, sizeof(size));
			m_BloomEffect->SetValue("g_offset", offset, sizeof(size));
			m_BloomEffect->SetValue("g_weight", weights, sizeof(weights));

			m_BloomEffect->SetTexture("g_blur", m_luminanceRenderTarget.GetTexture());
			m_BloomEffect->CommitChanges();
			g_PostEffect->RenderPrimitive();
			//DrawQuadPrimitive();

			m_BloomEffect->EndPass();
			m_BloomEffect->End();
		}
		//輝度を抽出したテクスチャをYブラー
			{
				g_pd3dDevice->SetRenderTarget(0, m_downSamplingRenderTarget[i][1].GetRenderTarget());

				m_BloomEffect->SetTechnique("YBlur");
				m_BloomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
				m_BloomEffect->BeginPass(0);
				float size[2] = {
					(float)(m_downSamplingRenderTarget[i][0].GetWidth()),
					(float)(m_downSamplingRenderTarget[i][0].GetHeight())
				};
				float offset[] = {
					0.0f ,
					16.0f / (float)(m_downSamplingRenderTarget[i][0].GetHeight())
				};
				m_BloomEffect->SetValue("g_luminanceTexSize", size, sizeof(size));
				m_BloomEffect->SetValue("g_offset", offset, sizeof(size));
				m_BloomEffect->SetValue("g_weight", weights, sizeof(weights));

				m_BloomEffect->SetTexture("g_blur", m_downSamplingRenderTarget[i][0].GetTexture());
				m_BloomEffect->CommitChanges();
				g_PostEffect->RenderPrimitive();
				//DrawQuadPrimitive();

				m_BloomEffect->EndPass();
				m_BloomEffect->End();

			}
			{
				//最終合成。
				float offset[] = {
					0.5f / m_downSamplingRenderTarget[i][1].GetWidth() ,
					0.5f / m_downSamplingRenderTarget[i][1].GetHeight()
				};
				//戻す。
				g_pd3dDevice->SetRenderTarget(0, rt->GetRenderTarget());
				g_pd3dDevice->SetDepthStencilSurface(rt->GetDepthStencilBuffer());
				//加算合成。
				g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
				g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				m_BloomEffect->SetTechnique("Final");
				m_BloomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
				m_BloomEffect->BeginPass(0);
				m_BloomEffect->SetTexture("g_blur", m_downSamplingRenderTarget[i][1].GetTexture());
				m_BloomEffect->SetValue("g_offset", offset, sizeof(offset));
				m_BloomEffect->CommitChanges();

				g_PostEffect->RenderPrimitive();
				//DrawQuadPrimitive();

				m_BloomEffect->EndPass();
				m_BloomEffect->End();

				g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
	}
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}
