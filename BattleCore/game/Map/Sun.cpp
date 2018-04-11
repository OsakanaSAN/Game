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
	m_light.SetAmbientLight(D3DXVECTOR4(2.0f, 2.0f, 2.0f, 1.0f));

	m_skinmodeldata.LoadModelData("Assets/modelData/sun.X", NULL);
	m_skinmodel.Init(&m_skinmodeldata);
	m_skinmodel.SetLight(&m_light);
}

void CSun::Update()
{

	m_skinmodel.UpdateWorldMatrix(m_position, m_rot, { 0.5f,0.5f,0.5f });

}
void CSun::Render()
{

	m_skinmodel.SetShadowMap(false);
	m_skinmodel.SetShadowRecieve(false);
	
	m_skinmodel.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
}
