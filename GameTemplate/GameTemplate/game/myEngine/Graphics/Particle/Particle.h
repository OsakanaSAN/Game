#pragma once
#include "myEngine/Graphics/Sprite/Primitive.h"

struct SParicleEmitParameter;

class CParticle
{
public:
	CParticle();
	~CParticle();
	void SetPosition(D3DXVECTOR3 setpos)
	{
		position = setpos;
	}
	bool IsDete()
	{
		return isDete;
	}
	void Delete();
	void Init(const SParicleEmitParameter& param);
	void Update();
	void Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix);
private:
	CPrimitive			primitive;			//!<プリミティブ。
	LPDIRECT3DTEXTURE9	texture;			//!<テクスチャ。
	ID3DXEffect*		shaderEffect;		//!<シェーダーエフェクト。

	D3DXVECTOR3			moveSpeed;			//!<速度。
	D3DXVECTOR3			position;			//!<座標。
	float				Time;
	bool                isDete = false;

};

