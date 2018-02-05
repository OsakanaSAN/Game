#include "stdafx.h"
#include "Skydoom.h"


Skydoom::Skydoom()
{
}


Skydoom::~Skydoom()
{


}


void Skydoom::Init(D3DXVECTOR3 pos, D3DXQUATERNION rot)
{
	position = pos;
	rotation = rot;

	
	modelData.LoadModelData("Assets/modelData/sky.X",NULL);
	model.Init(&modelData);
	CreateCubeTexture();
	light.SetAmbientLight({ 0.5f,1.0f,1.0f,1.0f });
	model.SetLight(&light);
	model.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	

}
void Skydoom::Update()
{
	
	position.x = game->GetPlayer()->GetPos().x;
	position.z = game->GetPlayer()->GetPos().z;

	//D3DXQuaternionRotationAxis(&rotation, &D3DXVECTOR3{0.0f,1.0f,0.0f},rotSky);
	model.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(20.0f, 20.0f, 20.0f));
}

void Skydoom::Drow()
{
	model.SetWave(false);
	model.Draw(&game->GetCamera()->GetViewMatrix(), &game->GetCamera()->GetProjectionMatrix());
}