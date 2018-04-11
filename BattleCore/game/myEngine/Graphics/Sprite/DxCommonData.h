#pragma once
#include"stdafx.h"
#define WINDOWS_WIDTH 720 //幅
#define WINDOWS_HEIGHT 480 //高さ


//頂点フォーマット(トランスフォーム済座標とディフーズカラー)
#define FVF_CUSTOMVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE| D3DFVF_TEX1)

//カスタム頂点データ用構造体
typedef struct CUSTOMVERTEX
{
	float x, y, z;	// 頂点の変換後の座標
	DWORD color;		// 頂点カラー
	float u, v;			//テクスチャ座標
}CUSTOMVERTEX;

//メッシュ用構造体
typedef struct MESH
{
	LPDIRECT3DTEXTURE9* pTextures;
	D3DMATERIAL9* pMaterials;
	LPD3DXMESH pMesh;
	DWORD materialNum;

};