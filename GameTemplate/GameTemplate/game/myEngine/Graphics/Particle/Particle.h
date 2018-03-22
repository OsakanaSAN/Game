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
	void SetEndTime(float et)
	{
		m_EndTime = et;
	}

	bool IsDete()
	{
		return isDete;
	}

	void Delete();
	void Init(const SParicleEmitParameter& param, const LPDIRECT3DTEXTURE9 texture, ID3DXEffect& effct);
	void Update();
	void Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix);
private:
	CPrimitive			primitive;			//!<プリミティブ。
	LPDIRECT3DTEXTURE9	texture;			//!<テクスチャ。
	ID3DXEffect*		shaderEffect;		//!<シェーダーエフェクト。

	D3DXVECTOR3			moveSpeed;			//!<速度。
	D3DXVECTOR3			position;			//!<座標。
	float				m_EndTime;			//消す時間
	float				m_brightness = 0.1f;//輝度
	float				Time;
	float				m_alpha = 1.0f;
	bool                isDete = false;

};

