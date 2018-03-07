#include "stdafx.h"
#include "PostEffect.h"

CRenderTarget* mainRenderTarget;	//!<18-2 メインレンダリングターゲット。
CPrimitive*	quadPrimitive;			//!<18-3 四角形の板ポリプリミティブ。

CRenderTarget* CPostEffect::GetMainRenderTarget()
{
	return mainRenderTarget;
}

CPostEffect::CPostEffect()
{
}


CPostEffect::~CPostEffect()
{
}

void CPostEffect::InitMainRenderTarget()
{
	mainRenderTarget = new CRenderTarget;

	mainRenderTarget->Create(
		1280,					//フレームバッファの幅
		 720,					//フレームバッファの高さ
		   1,					//ミップマップレベル
		D3DFMT_A16B16G16R16F,	//カラーバッファのフォーマット
		D3DFMT_D24S8,			//デプスステンシルバッファのフォーマット
		D3DMULTISAMPLE_NONE,	//マルチサンプリングの種類
		0						//マルチサンプリングの品質
	);
}

void CPostEffect::InitPrimitive()
{
	quadPrimitive = new CPrimitive;
	//頂点の構造体。
	struct SVertex {
		float pos[4];	//頂点座標。
		float uv[2];		//UV座標。
	};
	//頂点バッファ。
	SVertex vertexBuffer[] = {
		{
			-1.0f, 1.0f, 0.0f, 1.0f,
			0.0f, 0.0f
		},
		{
			1.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 0.0f
		},
		{
			-1.0f, -1.0f, 0.0f, 1.0f,
			0.0f, 1.0f
		},
		{
			1.0f, -1.0f, 0.0f, 1.0f,
			1.0f, 1.0f
		},
	};
	//頂点レイアウト。頂点バッファの構造がどのようになっているかをGPUに教えるためのもの。
	static const D3DVERTEXELEMENT9 scShapeVertex_PT_Element[] = {
		//頂点ストリームの0番目の0バイト目からfloat型の4つ分のデータは座標(D3DDECLUSAGE_POSITION)であることを示している。
		{ 0, 0 ,   D3DDECLTYPE_FLOAT4		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION	, 0 },
		//頂点ストリームの0番目の16バイト目からfloat型の2つ分のデータはUV座標(D3DDECLUSAGE_TEXCOORD)であることを示している。
		{ 0, 16 ,  D3DDECLTYPE_FLOAT2		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD	, 0 },
		D3DDECL_END()
	};
	//インデックスバッファ。
	unsigned short indexBuffer[] = { 0, 1, 2, 3 };
	quadPrimitive->Create(
		CPrimitive::eTriangleStrip,	//今回はプリミティブの種類はトライアングルストリップ。
		4,							//頂点の数。四角形の板ポリでトライアングルストリップなので４。
		sizeof(SVertex),			//頂点ストライド。一つの頂点の大きさ。単位はバイト。
		scShapeVertex_PT_Element,	//頂点レイアウト。
		vertexBuffer,				//頂点バッファ。
		4,							//インデックスの数。
		D3DFMT_INDEX16,				//インデックスバッファのフォーマット。今回は16bitフォーマットを指定する。
									//インデックスの数が65536以下の場合は16bitでいいが、それを超える場合はD3DFMT_INDEX32を指定する。
		indexBuffer					//インデックスバッファ。
	);
}

//メインレンダリングターゲットの内容を現在のレンダリングターゲットにコピー
void CPostEffect::MainRTToCurrentRT()
{
	
	// Zテストで失敗してもらったら困るので、Zテストは無効にしておく。
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// αブレンドもいらない。
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	// 頂点ストリーム0番に板ポリの頂点バッファを設定する。
	g_pd3dDevice->SetStreamSource(
		0,												//頂点ストリームの番号。
		quadPrimitive->GetVertexBuffer()->GetBody(),	//頂点バッファ。
		0,												//頂点バッファの読み込みを開始するオフセットのバイト数。											//今回は先頭から読み込むので0でいい。
		quadPrimitive->GetVertexBuffer()->GetStride()	//頂点一つ分のサイズ。単位はバイト。
	);


	//LPD3DXEFFECT shader = SepiaEffct;			//セピアフィルターをかける

	//コピーシェーダーを設定。
	m_CopyEffct->SetTechnique("Default");
	m_CopyEffct->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_CopyEffct->BeginPass(0);
	//シーンテクスチャを設定する
	m_CopyEffct->SetTexture("g_tex",mainRenderTarget->GetTexture());
	//定数レジスタへの変更をコミットする。
	m_CopyEffct->CommitChanges();


	//セピアシェーダーの設定
	//m_SepiaEffct->SetTechnique("Default");
	//m_SepiaEffct->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	//m_SepiaEffct->BeginPass(0);
	//シーンテクスチャを設定する
	m_SepiaEffct->SetTexture("g_tex", mainRenderTarget->GetTexture());
	//定数レジスタへの変更をコミットする。
	m_SepiaEffct->CommitChanges();

	// インデックスバッファを設定。
	g_pd3dDevice->SetIndices(quadPrimitive->GetIndexBuffer()->GetBody());
	// 頂点定義を設定する。
	g_pd3dDevice->SetVertexDeclaration(quadPrimitive->GetVertexDecl());
	//　準備が整ったので描画。
	g_pd3dDevice->DrawIndexedPrimitive(
		quadPrimitive->GetD3DPrimitiveType(),	//プリミティブの種類を指定する。
		0,										//ベース頂点インデックス。0でいい。
		0,										//最小の頂点インデックス。0でいい。
		quadPrimitive->GetNumVertex(),			//頂点の数。
		0,										//開始インデックス。0でいい。
		quadPrimitive->GetNumPolygon()			//プリミティブの数。
	);
	//コピー
	m_CopyEffct->EndPass();
	m_CopyEffct->End();
	//セピア
	//m_SepiaEffct->EndPass();
	//m_SepiaEffct->End();
	// 変更したレンダリングステートを元に戻す。
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

}

//シェーダーをロード
void CPostEffect::LoadShader()
{

	LPD3DXBUFFER  compileErrorBuffer = NULL;
	//セピア調のシェーダーロード
	m_SepiaEffct = g_effectManager->LoadEffect("Assets/Shader/SepiaFilter.fx");
	m_CopyEffct = g_effectManager->LoadEffect("Assets/Shader/Copy.fx");


}
void CPostEffect::Render()
{
	
	//現在のレンダリングターゲットのバックアップの作成
	LPDIRECT3DSURFACE9 frameBufferRT;
	LPDIRECT3DSURFACE9 frameBufferDS;
	g_pd3dDevice->GetRenderTarget(0, &frameBufferRT);
	g_pd3dDevice->GetDepthStencilSurface(&frameBufferDS);

	//レンダリングターゲットの切り替え
	g_pd3dDevice->SetRenderTarget(
		0,									//何番目のレンダリングターゲット
		mainRenderTarget->GetRenderTarget()  //変更するレンダリングターゲット
	);
	//デプスステンシルバッファの切り替え
	g_pd3dDevice->SetDepthStencilSurface(
		mainRenderTarget->GetDepthStencilBuffer()//変更するデプスステンシルバッファ
	);

	// レンダリングターゲットをクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

	g_pd3dDevice->BeginScene();

	game->Render();
	bloom.Render();
	monochro.Render();
	if (game->GetPlayer()->GetZoomBlur())
	{

		zoomBlur.Render();

	}

	// ハンズオン 1-2 レンダリングターゲットを戻す。
	g_pd3dDevice->SetRenderTarget(0, frameBufferRT);
	g_pd3dDevice->SetDepthStencilSurface(frameBufferDS);

	frameBufferRT->Release();
	frameBufferDS->Release();
	
	// 18-3 オフスクリーンレンダリングした絵をフレームバッファに貼り付ける。
	MainRTToCurrentRT();
	

	game->Render2D();
	// シーンの描画終了。
	//g_pd3dDevice->EndScene();

	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// バックバッファとフロントバッファを入れ替える。
	//g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

}

void CPostEffect::RenderPrimitive()
{
	// 頂点ストリーム0番に板ポリの頂点バッファを設定する。
	g_pd3dDevice->SetStreamSource(
		0,												//頂点ストリームの番号。
		quadPrimitive->GetVertexBuffer()->GetBody(),	//頂点バッファ。
		0,												//頂点バッファの読み込みを開始するオフセットのバイト数。今回は先頭から読み込むので0でいい。
		quadPrimitive->GetVertexBuffer()->GetStride()	//頂点一つ分のサイズ。単位はバイト。
	);
	// インデックスバッファを設定。
	g_pd3dDevice->SetIndices(quadPrimitive->GetIndexBuffer()->GetBody());
	// 頂点定義を設定する。
	g_pd3dDevice->SetVertexDeclaration(quadPrimitive->GetVertexDecl());
	//　準備が整ったので描画。
	g_pd3dDevice->DrawIndexedPrimitive(
		quadPrimitive->GetD3DPrimitiveType(),	//プリミティブの種類を指定する。
		0,										//ベース頂点インデックス。0でいい。
		0,										//最小の頂点インデックス。0でいい。
		quadPrimitive->GetNumVertex(),			//頂点の数。
		0,										//開始インデックス。0でいい。
		quadPrimitive->GetNumPolygon()			//プリミティブの数。
	);
}
