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
	CPrimitive			primitive;			//!<�v���~�e�B�u�B
	LPDIRECT3DTEXTURE9	texture;			//!<�e�N�X�`���B
	ID3DXEffect*		shaderEffect;		//!<�V�F�[�_�[�G�t�F�N�g�B

	D3DXVECTOR3			moveSpeed;			//!<���x�B
	D3DXVECTOR3			position;			//!<���W�B
	float				m_EndTime;			//��������
	float				m_brightness = 0.1f;//�P�x
	float				Time;
	float				m_alpha = 1.0f;
	bool                isDete = false;

};

