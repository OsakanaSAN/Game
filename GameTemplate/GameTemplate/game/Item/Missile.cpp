#include "stdafx.h"
#include "Missile.h"


Missile::Missile()
{
}


Missile::~Missile()
{
}

void Missile::Start(const D3DXVECTOR3 &pos, const D3DXQUATERNION &rot, const D3DXVECTOR3 tagpos)
{
	m_position = pos;
	m_rotation = rot;
	//真上に飛ばす

	D3DXVECTOR3 Upposition = pos;
	Upposition.y += 50;
	Upposition = Upposition - m_position;
	D3DXVec3Normalize(&Upposition, &Upposition);
	this->moveSpeed = Upposition;

	//ライトを初期化。
	m_light.SetDiffuseLightDirection(0, D3DXVECTOR4(0.707f, 0.0f, -0.707f, 1.0f));
	m_light.SetDiffuseLightDirection(1, D3DXVECTOR4(-0.707f, 0.0f, -0.707f, 1.0f));
	m_light.SetDiffuseLightDirection(2, D3DXVECTOR4(0.0f, 0.707f, -0.707f, 1.0f));
	m_light.SetDiffuseLightDirection(3, D3DXVECTOR4(0.0f, -0.707f, -0.707f, 1.0f));

	m_light.SetDiffuseLightColor(0, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	m_light.SetDiffuseLightColor(1, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	m_light.SetDiffuseLightColor(2, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	m_light.SetDiffuseLightColor(3, D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
	m_light.SetAmbientLight(D3DXVECTOR4(0.6f, 0.6f, 0.6f, 1.0f));


	m_characterController = new CharacterController;
	m_characterController->Init(0.3f, 0.3f, m_position);
	m_characterController->SetGravity(0.0f);
	//if (modelData == NULL) {
	//モデルをロード。
	m_modelData = new SkinModelData;
	m_modelData->LoadModelData("Assets/modelData/Bullet2.X", NULL);
	//}

	m_rotation = game->GetPlayer()->GetRot(); //プレイヤーの回転
	m_model.Init(m_modelData);
	m_model.SetLight(&m_light);

}

bool Missile::Update()
{

	return true;
}

void Missile::Render()
{

}