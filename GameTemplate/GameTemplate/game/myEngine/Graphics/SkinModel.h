/*!
*@brief	スキンモデル
*/
#pragma once

#include "myEngine/Graphics/Animation.h"


class SkinModelData;
class RenderContext;
class Light;


//フォグ。
enum EFogFunc {
	enFogFuncNone,		//フォグなし。
	enFogFuncDist,		//距離フォグ。
	enFogFuncHeight,	//高さフォグ。
	enFogFuncNum,
};
/*!
*@brief	スキンモデル
*/
class SkinModel {
public:
	SkinModel();
	~SkinModel();
	/*!
	*@brief	初期化
	*/
	void Init(SkinModelData* modelData);
	/*!
	*@brief	描画。
	*/
	void Draw(D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix);
	
	/*!
	*@brief	ワールド行列を更新。
	*@details
	* 後でCMotionクラスに移動させます。
	*@param[in]		trans	平行移動。
	*@param[in]		rot		回転。
	*@param[in]		scale	拡大。
	*/
	void UpdateWorldMatrix( const D3DXVECTOR3& trans, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale );
	/*!
	*@brief	ライトを設定。
	*/
	void SetLight(Light* light)
	{
		this->light = light;
	}
	D3DXMATRIX GetMatrix()
	{
		return worldMatrix;
	}

	D3DXMATRIX GetWorldMatrix()
	{
		return worldMatrix;
	}
	/*!
	* @brief	フォグパラメータを設定。
	*@param[in]	fogFunc		フォグの種類。EFogFuncを参照。
	*@param[in]	param0		フォグパラメータ0
	* fogFuncがenFogFuncDistの場合はフォグが掛かり始める距離、fogFuncがenFogFuncHeightの場合はフォグが掛かり始める高さ。
	*@param[in]	param1		フォグパラメータ１
	* fogFuncがenFogFuncDistの場合はフォグが掛かり切る距離、fogFuncがenFogFuncHeightの場合はフォグが掛かり切る高さ。
	*/
	void SetFogParam(EFogFunc fogFunc, float param0, float param1)
	{
		m_fogFunc = fogFunc;
		m_fogParam[0] = param0;
		m_fogParam[1] = param1;
	}
	//シャドウマップセット
	void SetShadowMap(bool ssm)
	{
		IsDrawShadowMap = ssm;
	}
	//シャドウレシーブセット
	void SetShadowRecieve(bool ssr)
	{
		IsRecieveShadow = ssr;
	}
	void SetWave(bool Wave)
	{
		IsWave = Wave;
	}
	
	LPDIRECT3DTEXTURE9 GetWaveTex()
	{
		return WaveTex;
	}
	void SetWaveTexture()
	{
		//D3DXIMAGE_INFO imgInfo;
	     HRESULT hr = D3DXCreateTextureFromFileA(
			g_pd3dDevice,
			"Assets/modelData/NormalMap4.png",
			&WaveTex
		 );	//テクスチャ読込
		 this->NormalMap = WaveTex;
	}
	void SetSkyTexture(LPDIRECT3DCUBETEXTURE9 cube)
	{
		this->CubeMap = cube;
		this->IsSky = true;

	}

	void SetTexture(LPDIRECT3DTEXTURE9 settex)
	{
		this->NormalMap = settex;
	}


	/*!
	*@brief	オリジナルメッシュの先頭を取得する。
	*/
	LPD3DXMESH GetOrgMeshFirst();
private:
	
	D3DXMATRIX			worldMatrix;				//!<ワールド行列。
	D3DXMATRIX			rotationMatrix;				//!<回転行列。
	SkinModelData*		skinModelData = nullptr;	//!<スキンモデルデータ。
	ID3DXEffect*		pEffect = nullptr;			//!<エフェクト。
	Animation			animation;					//!<アニメーション。
	Light*				light = nullptr;			//!<ライト。
	EFogFunc						m_fogFunc;							//!<フォグの種類。0ならフォグなし、1なら距離フォグ、2なら高さフォグ。
	float							m_fogParam[2];						//!<フォグのパラメータ

	//影用の判定
	bool				IsDrawShadowMap = false; 
	bool                IsRecieveShadow = false;
	//水面の判定
	bool                IsWave = false;
	LPDIRECT3DTEXTURE9      NormalMap = NULL;
	LPDIRECT3DTEXTURE9      WaveTex = NULL;

	//空用の判定
	bool                 IsSky = false;
	LPDIRECT3DCUBETEXTURE9      CubeMap = NULL;
	
};
