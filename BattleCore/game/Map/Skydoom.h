#pragma once
class Skydoom
{
public:
	Skydoom();
	~Skydoom();
	void Init(D3DXVECTOR3 pos, D3DXQUATERNION rot);
	void Update();
	void Render();
	void RadarMapRender(D3DXMATRIX&  RederCameraMatrix, D3DXMATRIX&	RederCameraProjMatrix);


private:
	SkinModel model;                   
	CharacterController  skycontroller; //‹ó‚ÌƒLƒƒƒ‰ƒRƒ“
	SkinModelData modelData;
	D3DXVECTOR3	position;
	D3DXQUATERNION rotation;
	Light light;
	float rotSky = 0.5f;
	LPDIRECT3DCUBETEXTURE9*      CubeMap = NULL;

};

