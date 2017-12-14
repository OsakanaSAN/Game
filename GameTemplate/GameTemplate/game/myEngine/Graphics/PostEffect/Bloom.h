#pragma once
#include "myEngine/Graphics/RenderTarget.h"

class CBloom
{
public:
	CBloom();
	~CBloom();
	//描画
	void Render();

	//重みの更新
	void UpdateWeight(float dispersion);
private:
	static const int MGF_DOWN_SAMPLE_COUNT = 1;     //ダウンサンプリングの回数
	LPD3DXEFFECT  m_BloomEffect;						//!<シェーダーエフェクト。
	CRenderTarget m_luminanceRenderTarget;			//!<輝度を抽出するためのレンダリングターゲット。
	CRenderTarget m_downSamplingRenderTarget[MGF_DOWN_SAMPLE_COUNT][2];	//!<ブラーをかけるためのダウンサンプリング用のレンダリングターゲット。
	static const int NUM_WEIGHTS = 8;				//!<ガウスブラーの重み。
	float weights[NUM_WEIGHTS];						//!<ガウスブラーで使う重みテーブル。
};

