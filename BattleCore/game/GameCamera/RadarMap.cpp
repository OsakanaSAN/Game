#include "stdafx.h"
#include "RadarMap.h"


struct SShapeVertex_PT {
	float		pos[4];
	float		uv[2];
};
static const D3DVERTEXELEMENT9 scShapeVertex_PT_Element[] = {
	{ 0, 0 ,   D3DDECLTYPE_FLOAT4		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION	, 0 },
	{ 0, 16 ,  D3DDECLTYPE_FLOAT2		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD	, 0 },
	D3DDECL_END()
};

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
	m_Far = 2000.0f;
	m_viewAngle = 90.0f * (3.14159265358979323846 / 180.0f);
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
		600,
		600,
		1,						//レンダリングターゲットにはミップマップは不要なので一枚のみ。
		D3DFMT_A16B16G16R16F,		    //Zの精度
		D3DFMT_D16,		    //学生のＰＣで24bitの深度バッファを作成できなかったので、16ビットで深度バッファを作成する。
		D3DMULTISAMPLE_NONE,	//マルチサンプリングはなし。
		0						//マルチサンプリングしないので０を指定。
	);


	static SShapeVertex_PT vertex[]{
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

	static unsigned short index[] = {
		0,1,2,3
	};
	m_primitive.Create(
		CPrimitive::eTriangleStrip,
		4,
		sizeof(SShapeVertex_PT),
		scShapeVertex_PT_Element,
		vertex,
		4,
		D3DFMT_INDEX16,
		index
	);

	//シェーダーをロード
	shaderEffect = g_effectManager->LoadEffect("Assets/Shader/CircleTexture.fx");

}

void CRadarMap::Update(const D3DXVECTOR3 position)
{

		D3DXVECTOR3 target = game->GetGameCamara()->Getcamera()->GetLookatPt();
		D3DXVECTOR3 viewPos = target;
		viewPos.y += 100;


		m_viewPosition = viewPos;
		m_viewTarget = target;


		//ライトビュープロジェクション行列を更新。
		//普通のカメラと同じ。
		//カメラの上方向を決める計算だけ入れておく。
		D3DXVECTOR3 tmp = m_viewTarget - m_viewPosition;
		D3DXVECTOR3 lightViewUp;
		D3DXVec3Normalize(&tmp, &tmp);

		D3DXMatrixLookAtLH(&m_lightViewMatrix, &m_viewPosition, &m_viewTarget, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	
		D3DXMatrixOrthoLH(
			&m_lightProjMatrix,
			1240,
			1240,
			m_Near,
			m_Far
			
		);
		D3DXMATRIX rot;
		angle++;
		float rStick_x = pad.GetRStickXF();

		D3DXMatrixRotationAxis(&rot, &D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXToRadian(rStick_x));
		//D3DXMatrixRotationZ(&m_rot, 0.1f * rStick_x);
		D3DXMatrixMultiply(&m_lightProjMatrix, &m_lightProjMatrix,&rot);
		pad.Update();

}

void CRadarMap::Draw(SkinModel* skinmodel)
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

	skinmodel->Draw(&m_lightViewMatrix, &m_lightProjMatrix);

	for (const auto& Enemy : game->GetEnemys()) {
		Enemy->MiniMapRender(m_lightViewMatrix, m_lightProjMatrix);
	}
	game->GetMap()->RadarMapRender(m_lightViewMatrix, m_lightProjMatrix);
	
	
	D3DXMATRIX viewProj;
	D3DXMatrixMultiply(&viewProj, &m_lightViewMatrix, &m_lightProjMatrix);

	shaderEffect->SetTechnique("creatTex");
	shaderEffect->SetMatrix("g_WVPmt", &viewProj);

	shaderEffect->SetTexture("g_texture", GetTexture());
	shaderEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	shaderEffect->BeginPass(0);
	shaderEffect->CommitChanges();
	g_pd3dDevice->SetStreamSource(0, m_primitive.GetVertexBuffer()->GetBody(), 0, m_primitive.GetVertexBuffer()->GetStride());
	g_pd3dDevice->SetIndices(m_primitive.GetIndexBuffer()->GetBody());
	g_pd3dDevice->SetVertexDeclaration(m_primitive.GetVertexDecl());
	g_pd3dDevice->DrawIndexedPrimitive(m_primitive.GetD3DPrimitiveType(), 0, 0, m_primitive.GetNumVertex(), 0, m_primitive.GetNumPolygon());

	shaderEffect->EndPass();
	shaderEffect->End();
	
	//game->GetMap()->RadarMapRender(m_Springcamera.GetCamera()->GetViewMatrix(), m_Springcamera.GetCamera()->GetProjectionMatrix());
	g_pd3dDevice->EndScene();
	g_pd3dDevice->SetRenderTarget(0, renderTargetBackup);		//戻す。
	g_pd3dDevice->SetDepthStencilSurface(depthBufferBackup);	//戻す。
	g_pd3dDevice->EndScene();
}