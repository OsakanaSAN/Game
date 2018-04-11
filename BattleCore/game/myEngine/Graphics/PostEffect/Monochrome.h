#pragma once
#include "myEngine/Graphics/RenderTarget.h"

class CMonochrome
{
public:
	CMonochrome();
	~CMonochrome();
	void Update();
	void Render();

private:

	LPD3DXEFFECT	m_MonochromeEffect;		//�V�F�[�_�[�G�t�F�N�g�B
	CRenderTarget	m_RenderTarget;			//�����_�����O�^�[�Q�b�g�B
	float			m_AlphaPower = 1.0f;
};

