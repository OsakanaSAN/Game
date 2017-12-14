#pragma once
class Skydoom
{
public:
	Skydoom();
	~Skydoom();
	void Init(D3DXVECTOR3 pos, D3DXQUATERNION rot);
	void Update();
	void Drow();

	void CreateCubeTexture()
	{
		HRESULT hr = D3DXCreateCubeTextureFromFile(
			g_pd3dDevice,
			"Assets/modelData/CubeMap2.dds",
			&this->CubeMap
		);
		model.SetSkyTexture(CubeMap);
	}

private:
	SkinModel model;                   
	CharacterController  skycontroller; //空のキャラコン
	SkinModelData modelData;
	D3DXVECTOR3	position;
	D3DXQUATERNION rotation;
	Light light;
	float rotSky = 0.5f;
	LPDIRECT3DCUBETEXTURE9      CubeMap = NULL;

};

