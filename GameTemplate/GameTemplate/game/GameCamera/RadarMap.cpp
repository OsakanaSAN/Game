#include "stdafx.h"
#include "RadarMap.h"


CRadarMap::CRadarMap()
{

	D3DXMatrixIdentity(&m_lightViewMatrix);
	D3DXMatrixIdentity(&m_lightProjMatrix);
	m_lightDirection = { 0.0f, -1.0f, 0.0f };
	m_viewPosition.x = 0.0f;
	m_viewPosition.y = 0.0f;
	m_viewPosition.z = 0.0f;
	m_viewTarget.x = 0.0f;
	m_viewTarget.y = 0.0f;
	m_viewTarget.z = 0.0f;

	m_Near = 1.0f;
	m_Far = 200.0f;
	m_viewAngle = 60.0f * (3.14159265358979323846 / 180.0f);
	m_aspect = (float)FRAME_BUFFER_WIDTH / (float)FRAME_BUFFER_HEIGHT;
	m_viewPosition = { 0.0f, 3.0f, 0.0f };
	m_viewTarget = { 0.0f,0.0f,0.0f };

}


CRadarMap::~CRadarMap()
{
}

void CRadarMap::InitRender()
{

	//レンダリングターゲットを初期化。
	m_SrenderTarget.Create(
		100,
		100,
		1,						//レンダリングターゲットにはミップマップは不要なので一枚のみ。
		D3DFMT_R16F,		    //Zの精度
		D3DFMT_D24S8,		    //学生のＰＣで24bitの深度バッファを作成できなかったので、16ビットで深度バッファを作成する。
		D3DMULTISAMPLE_NONE,	//マルチサンプリングはなし。
		0						//マルチサンプリングしないので０を指定。
	);

}

void CRadarMap::Update()
{
	D3DXVECTOR3 target = game->GetPlayer()->GetPos();
	D3DXVECTOR3 viewPos = target;
	viewPos.y += 30;
	g_shadowMap.SetLightViewPosition(viewPos);
	g_shadowMap.SetLightViewTarget(target);


	//ライトビュープロジェクション行列を更新。
	//普通のカメラと同じ。
	//カメラの上方向を決める計算だけ入れておく。
	D3DXVECTOR3 tmp = m_viewTarget - m_viewPosition;
	D3DXVec3Normalize(&tmp, &tmp);
	if (fabsf(tmp.y) > 0.9999f) {
		//カメラがほぼ真上or真下を向いている。
		D3DXMatrixLookAtLH(&m_lightViewMatrix, &m_viewPosition, &m_viewTarget, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	}
	else {
		D3DXMatrixLookAtLH(&m_lightViewMatrix, &m_viewPosition, &m_viewTarget, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	//D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(&m_lightProjMatrix, m_viewAngle, m_aspect, m_Near, m_Far);



}

void CRadarMap::Draw()
{
	LPDIRECT3DSURFACE9 renderTargetBackup;
	LPDIRECT3DSURFACE9 depthBufferBackup;
	g_pd3dDevice->GetRenderTarget(0, &renderTargetBackup);		//元々のレンダリングターゲットを保存。後で戻す必要があるので。
	g_pd3dDevice->GetDepthStencilSurface(&depthBufferBackup);	//元々のデプスステンシルバッファを保存。後で戻す必要があるので。
																//レンダリングターゲットを変更する。
	g_pd3dDevice->SetRenderTarget(0, m_SrenderTarget.GetRenderTarget());
	g_pd3dDevice->SetDepthStencilSurface(m_SrenderTarget.GetDepthStencilBuffer());
	//書き込み先を変更したのでクリア。
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

	game->GetPlayer()->LightEyePosRender(m_lightViewMatrix, m_lightProjMatrix);
}