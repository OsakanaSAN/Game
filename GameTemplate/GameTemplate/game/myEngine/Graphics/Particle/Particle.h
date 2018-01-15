#pragma once
#include "myEngine/Graphics/Sprite/Primitive.h"

struct SParicleEmitParameter;

class CParticle
{
public:
	CParticle();
	~CParticle();
	void Init(const SParicleEmitParameter& param);
	void Update();
	void Render(const D3DXMATRIX& viewMatrix, const D3DXMATRIX& projMatrix);
private:
	CPrimitive			primitive;			//!<�v���~�e�B�u�B
	LPDIRECT3DTEXTURE9	texture;			//!<�e�N�X�`���B
	ID3DXEffect*		shaderEffect;		//!<�V�F�[�_�[�G�t�F�N�g�B
	D3DXVECTOR3			moveSpeed;			//!<���x�B
	D3DXVECTOR3			position;			//!<���W�B

};

