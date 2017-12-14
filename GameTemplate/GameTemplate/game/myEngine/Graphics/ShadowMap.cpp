#include "stdafx.h"
#include "ShadowMap.h"
#include "myEngine\Graphics\SkinModel.h"


int shadowCount = 1; //

CShadowMap::CShadowMap()
{
	D3DXMatrixIdentity(&lightViewMatrix);
	D3DXMatrixIdentity(&lightProjMatrix);
	m_lightDirection = { 0.0f, -1.0f, 0.0f };
	viewPosition.x = 0.0f;
	viewPosition.y = 0.0f;
	viewPosition.z = 0.0f;
	viewTarget.x = 0.0f;
	viewTarget.y = 0.0f;
	viewTarget.z = 0.0f;

	Near = 1.0f;
	Far = 200.0f;
	m_viewAngle = 60.0f * (3.14159265358979323846 / 180.0f);
	aspect = (float)FRAME_BUFFER_WIDTH / (float)FRAME_BUFFER_HEIGHT;
	viewPosition = { 0.0f, 3.0f, 0.0f };
	viewTarget = { 0.0f,0.0f,0.0f };


}
CShadowMap::~CShadowMap()
{
}

//初期化。0
void CShadowMap::Init()
{
	//レンダリングターゲットを初期化。
	SrenderTarget.Create(
		1280,
		720,
		1,						//レンダリングターゲットにはミップマップは不要なので一枚のみ。
		D3DFMT_R16F,		    //Zの精度
		D3DFMT_D24S8,		    //学生のＰＣで24bitの深度バッファを作成できなかったので、16ビットで深度バッファを作成する。
		D3DMULTISAMPLE_NONE,	//マルチサンプリングはなし。
		0						//マルチサンプリングしないので０を指定。
	);
}


//更新。
void CShadowMap::Update()
{
	////ライトビューの注視点。視点。
	////これは各自のゲームで調整するように。
	//D3DXVec3Subtract(&m_lightDirection, &viewTarget, &viewPosition);
	//D3DXVec3Normalize(&m_lightDirection, &m_lightDirection);

	//D3DXVECTOR3 lightUp;
	//D3DXVECTOR3 Up = { 0,1,0 };
	//float t = fabsf(D3DXVec3Dot(&m_lightDirection, &Up));
	//if (fabsf((t - 1.0f)) < 0.00001f) {
	//	//ライトの方向がほぼY軸と並行。
	//	lightUp = { 1,0,0 };
	//}
	//else {
	//	lightUp = { 0,1,0 };
	//}
	////ライトからみたビュー行列を作成。
	//D3DXVECTOR3 target;
	//D3DXVec3Add(&target, &viewPosition, &m_lightDirection);
	//D3DXMatrixLookAtLH(&lightViewMatrix, &viewPosition, &target, &lightUp);

	D3DXVECTOR3 target = game->GetPlayer()->GetPos();
	D3DXVECTOR3 viewPos = target;
	viewPos.y += 30;
	viewPos.x += 10;
	shadowCount++;
	if (shadowCount == 3)
	{
		shadowCount = 1;
	}

	/*D3DXVec3Subtract(&m_lightDirection, &target, &viewPos);
	D3DXVec3Normalize(&m_lightDirection, &m_lightDirection);
	D3DXVec3Add(&target, &viewPos,&m_lightDirection);
	*/
	g_shadowMap.SetLightViewPosition(viewPos);
	g_shadowMap.SetLightViewTarget(target);


	//ライトビュープロジェクション行列を更新。
	//普通のカメラと同じ。
	//カメラの上方向を決める計算だけ入れておく。
	D3DXVECTOR3 tmp = viewTarget - viewPosition;
	D3DXVec3Normalize(&tmp, &tmp);
	if (fabsf(tmp.y) > 0.9999f) {
		//カメラがほぼ真上or真下を向いている。
		D3DXMatrixLookAtLH(&lightViewMatrix, &viewPosition, &viewTarget, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	}
	else {
		D3DXMatrixLookAtLH(&lightViewMatrix, &viewPosition, &viewTarget, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	//D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&lightProjMatrix, m_viewAngle, aspect, Near, Far);


}

//シャドウマップに書き込み。
void CShadowMap::Draw()
{
	
		Update();
		g_pd3dDevice->BeginScene();
		LPDIRECT3DSURFACE9 renderTargetBackup;
		LPDIRECT3DSURFACE9 depthBufferBackup;
		g_pd3dDevice->GetRenderTarget(0, &renderTargetBackup);		//元々のレンダリングターゲットを保存。後で戻す必要があるので。
		g_pd3dDevice->GetDepthStencilSurface(&depthBufferBackup);	//元々のデプスステンシルバッファを保存。後で戻す必要があるので。
																	//レンダリングターゲットを変更する。
		g_pd3dDevice->SetRenderTarget(0, SrenderTarget.GetRenderTarget());
		g_pd3dDevice->SetDepthStencilSurface(SrenderTarget.GetDepthStencilBuffer());
		//書き込み先を変更したのでクリア。
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		game->GetPlayer()->LightEyePosRender(lightViewMatrix, lightProjMatrix);
		/*for (auto Enemy : game->GetEnemys()) {
			Enemy->LightEyePosRender(lightViewMatrix, lightProjMatrix);
		}*/
		/*for (auto bullet : game->GetPlayerBullet()) {
			bullet->LightEyePosRender(lightViewMatrix, lightProjMatrix);
		}*/
		//game->GetMap()->ShadowMapRender(lightViewMatrix, lightProjMatrix);

		g_pd3dDevice->SetRenderTarget(0, renderTargetBackup);		//戻す。
		g_pd3dDevice->SetDepthStencilSurface(depthBufferBackup);	//戻す。
		g_pd3dDevice->EndScene();
	
}
