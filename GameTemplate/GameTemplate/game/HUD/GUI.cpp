#include "stdafx.h"
#include "GUI.h"

/*!
*@brief	座標とUV座標
*/
struct SShapeVertex_PT {
	float		pos[4];
	float		uv[2];
};
static const D3DVERTEXELEMENT9 scShapeVertex_PT_Element[] = {
	{ 0, 0 ,   D3DDECLTYPE_FLOAT4		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION	, 0 },
	{ 0, 16 ,  D3DDECLTYPE_FLOAT2		, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD	, 0 },
	D3DDECL_END()
};


CGUI::CGUI()
{
	
}


CGUI::~CGUI()
{
}

void CGUI::Start(const D3DXVECTOR2 setsize, const char* texturePath)
{
	
	m_size = setsize;

	float halfW = m_size.x * 0.5f;
	float halfH = m_size.y * 0.5f;
	oldsize[0] = halfW;
	oldsize[1] = halfH;

	D3DXVECTOR4 uv(0.0f, 0.0f, 1.0f, 1.0f);
	//位置の初期化
	m_position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);


	SShapeVertex_PT vb[] = {
		{
			-halfW, halfH, 0.0f, 1.0f,
			uv.x, uv.y
		},
		{
			halfW, halfH, 0.0f, 1.0f,
			uv.z, uv.y
		},
		{
			-halfW, -halfH, 0.0f, 1.0f,
			uv.x, uv.w
		},
		{
			halfW, -halfH, 0.0f, 1.0f,
			uv.z, uv.w
		},

	};
	short index[]{
		0,1,2,3
	};

	m_primitive.Create(
		CPrimitive::eTriangleStrip,
		4,
		sizeof(SShapeVertex_PT),
		scShapeVertex_PT_Element,
		vb,
		4,
		D3DFMT_INDEX16,
		index
	);
	HRESULT hr = D3DXCreateTextureFromFileA(g_pd3dDevice,texturePath, &m_texture);

	LPD3DXBUFFER  compileErrorBuffer = NULL;
	hr = D3DXCreateEffectFromFile(
		g_pd3dDevice,
		"Assets/Shader/ColorTexPrim.fx",
		NULL,
		NULL,
#ifdef _DEBUG
		D3DXSHADER_DEBUG,
#else
		D3DXSHADER_SKIPVALIDATION,
#endif
		NULL,
		&shaderEffect,
		&compileErrorBuffer
	);

}
void CGUI::setSize(D3DXVECTOR2 setsize)
{
	m_size = setsize;
	float remainHP = oldsize[0] * 2;
	remainHP = remainHP * m_size.x;
	m_oldHp += remainHP;
	float halfW = m_size.x * 0.5f;
	float halfH = m_size.y * 0.5f;

	D3DXVECTOR4 uv(0.0f, 0.0f, 1.0f, 1.0f);

	SShapeVertex_PT vb[] = {
		{
			-oldsize[0] + m_oldHp, halfH, 0.0f, 1.0f,
			uv.x, uv.y
		},
		{
			oldsize[0], oldsize[1], 0.0f, 1.0f,
			uv.z, uv.y
		},
		{
			-oldsize[0] + m_oldHp, -halfH, 0.0f, 1.0f,
			uv.x, uv.w
		},
		{
			oldsize[0], -oldsize[1], 0.0f, 1.0f,
			uv.z, uv.w
		},

	};
	short index[]{
		0,1,2,3
	};

	m_primitive.Create(
		CPrimitive::eTriangleStrip,
		4,
		sizeof(SShapeVertex_PT),
		scShapeVertex_PT_Element,
		vb,
		4,
		D3DFMT_INDEX16,
		index
	);

}


void CGUI::Update()
{

}

void CGUI::Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix)
{


	Time += GameTime().GetFrameDeltaTime();
	D3DXMATRIX m, mTrans, viewRot;
	D3DXMatrixTranslation(&mTrans, m_position.x, m_position.y, m_position.z);
	//1:カメラの回転行列を求める
	D3DXMatrixInverse(&viewRot, NULL, &viewMatrix);
	viewRot.m[3][0] = 0.0f;
	viewRot.m[3][1] = 0.0f;
	viewRot.m[3][2] = 0.0f;
	viewRot.m[3][3] = 1.0f;
	//2:1で求めた回転行列をワールド行列に乗算する。
	m = viewRot * mTrans * viewMatrix * projMatrix;

	//アルファブレンディングを有効にする。
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);

	shaderEffect->SetTechnique("ColorTexPrimTrans");

	shaderEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	shaderEffect->BeginPass(0);


	shaderEffect->SetValue("g_mWVP", &m, sizeof(m));
	shaderEffect->SetTexture("g_texture", m_texture);
	float a = 1.0f;
	shaderEffect->SetFloat("g_alpha", a);
	shaderEffect->CommitChanges();

	g_pd3dDevice->SetStreamSource(0, m_primitive.GetVertexBuffer()->GetBody(), 0, m_primitive.GetVertexBuffer()->GetStride());
	g_pd3dDevice->SetIndices(m_primitive.GetIndexBuffer()->GetBody());
	g_pd3dDevice->SetVertexDeclaration(m_primitive.GetVertexDecl());
	g_pd3dDevice->DrawIndexedPrimitive(m_primitive.GetD3DPrimitiveType(), 0, 0, m_primitive.GetNumVertex(), 0, m_primitive.GetNumPolygon());
	shaderEffect->EndPass();
	shaderEffect->End();

	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

}