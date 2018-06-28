#include "stdafx.h"
#include "Sun.h"


CSun::CSun()
{
}


CSun::~CSun()
{
}

void CSun::Start(D3DXVECTOR3 pos, D3DXQUATERNION rot)
{
	m_position = pos;
	m_rot = rot;
	//ƒ‰ƒCƒg‚ð‰Šú‰»B
	//m_light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	//m_light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	//m_light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	//m_light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	//m_light.SetDiffuseLightColor(0, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_light.SetDiffuseLightColor(1, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_light.SetDiffuseLightColor(2, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	//m_light.SetDiffuseLightColor(3, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	m_light.SetAmbientLight(D3DXVECTOR4(5.0f, 5.0f, 5.0f, 1.0f));


	


	
	

	m_skinmodeldata.LoadModelData("Assets/modelData/sun.X", NULL);
	m_skinmodel.Init(&m_skinmodeldata);
	m_skinmodel.SetLight(&m_light);
}

void CSun::Update()
{

	

	
	/*
	D3DXVECTOR3 targetvec = m_position - D3DXVECTOR3{ 0.0f,0.0f,0.0f };
	D3DXMATRIX rot;
	
	D3DXMatrixRotationZ(&rot, 0.1f * 0.8f * GameTime().GetFrameDeltaTime());
	

	D3DXVECTOR3 mEyepos = targetvec;
	mEyepos.x = mEyepos.x * rot.m[0][0] + mEyepos.y * rot.m[1][0] + mEyepos.z * rot.m[2][0] + rot.m[3][0];
	mEyepos.y = mEyepos.x * rot.m[0][1] + mEyepos.y * rot.m[1][1] + mEyepos.z * rot.m[2][1] + rot.m[3][1];
	mEyepos.z = mEyepos.x * rot.m[0][2] + mEyepos.y * rot.m[1][2] + mEyepos.z * rot.m[2][2] + rot.m[3][2];

	D3DXVec3Add(&m_position, &D3DXVECTOR3{ 0.0f,0.0f,0.0f }, &mEyepos);

	*/
	m_skinmodel.UpdateWorldMatrix(m_position, m_rot, { 0.1f,0.1f,0.1f });

}
void CSun::Render()
{

	m_skinmodel.SetShadowMap(false);
	m_skinmodel.SetShadowRecieve(false);
	
	m_skinmodel.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
}
