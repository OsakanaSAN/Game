#pragma once
#include "myEngine/Graphics/RenderTarget.h"

class CBloom
{
public:
	CBloom();
	~CBloom();
	//�`��
	void Render();

	//�d�݂̍X�V
	void UpdateWeight(float dispersion);
private:
	static const int MGF_DOWN_SAMPLE_COUNT = 1;     //�_�E���T���v�����O�̉�
	LPD3DXEFFECT  m_BloomEffect;						//!<�V�F�[�_�[�G�t�F�N�g�B
	CRenderTarget m_luminanceRenderTarget;			//!<�P�x�𒊏o���邽�߂̃����_�����O�^�[�Q�b�g�B
	CRenderTarget m_downSamplingRenderTarget[MGF_DOWN_SAMPLE_COUNT][2];	//!<�u���[�������邽�߂̃_�E���T���v�����O�p�̃����_�����O�^�[�Q�b�g�B
	static const int NUM_WEIGHTS = 8;				//!<�K�E�X�u���[�̏d�݁B
	float weights[NUM_WEIGHTS];						//!<�K�E�X�u���[�Ŏg���d�݃e�[�u���B
};

