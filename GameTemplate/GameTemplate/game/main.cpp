/*!
 *@brief	ゲームテンプレート。
 */
#include "stdafx.h"
#include "myEngine/Graphics/Camera.h"
#include "myEngine/Graphics/Light.h"
#include "game.h"
#include "myEngine/Graphics/PostEffect/PostEffect.h"

Game* game;
CPostEffect* g_PostEffect;

//-----------------------------------------------------------------------------
// Name: ゲームを初期化。
//-----------------------------------------------------------------------------
void Init()
{
	g_PostEffect = new CPostEffect;
	//メインレンダリングターゲットを作成
	g_PostEffect->InitMainRenderTarget();
	//板ポリの初期化
	g_PostEffect->InitPrimitive();
	//ポストエフェクト用のシェーダーロード
	g_PostEffect->LoadShader();
	game = new Game;
	game->Start();
}
//-----------------------------------------------------------------------------
// Name: 描画処理。
//-----------------------------------------------------------------------------
VOID Render()
{

	//シャドウマップにレンダリング。
	g_shadowMap.Draw();

	// 画面をクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	
	//シーンの描画開始。
	g_pd3dDevice->BeginScene();

	g_PostEffect->Render();

	//game->Render();
	//game->Render2D();

	//// シーンの描画終了。
	//g_pd3dDevice->EndScene();
	//// バックバッファとフロントバッファを入れ替える。
	//g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

}
/*!-----------------------------------------------------------------------------
 *@brief	更新処理。
 -----------------------------------------------------------------------------*/
void Update()
{
	game->Update();
}
//-----------------------------------------------------------------------------
//ゲームが終了するときに呼ばれる処理。
//-----------------------------------------------------------------------------
void Terminate()
{
	delete game;
	delete g_effectManager;
}
