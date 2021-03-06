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
	

	light.SetAmbientLight({ 1.0f,1.0f,1.0f,1.0f });
	model.SetLight(&light);
	model.SetSkyTexture();

	
	

}
void Skydoom::Update()
{
	
	//position.x = game->GetPlayer()->GetPos().x;
	//position.z = game->GetPlayer()->GetPos().z;

	//D3DXQuaternionRotationAxis(&rotation, &D3DXVECTOR3{0.0f,1.0f,0.0f},rotSky);
	model.UpdateWorldMatrix(position, rotation, D3DXVECTOR3(10.0f, 10.0f, 10.0f));
}

void Skydoom::Render()
{
	model.SetWave(false);
	model.Draw(&game->GetGameCamara()->Getcamera()->GetViewMatrix(), &game->GetGameCamara()->Getcamera()->GetProjectionMatrix());
}

void Skydoom::RadarMapRender(D3DXMATRIX&  RederCameraMatrix, D3DXMATRIX&	RederCameraProjMatrix)
{

	model.SetWave(false);
	model.Draw(&RederCameraMatrix,&RederCameraProjMatrix);
}
