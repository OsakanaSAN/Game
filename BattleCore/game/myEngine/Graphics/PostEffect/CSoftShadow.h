#pragma once
class CSoftShadow
{
public:
	CSoftShadow();
	~CSoftShadow();
	void Render();
	void UpdateWeight(float dispersion);

private:
	LPD3DXEFFECT		m_softEffect;						//!<�V�F�[�_�[�G�t�F�N�g�B
	CRenderTarget		m_RenderTarget;				//�[�x���������ރ����_�����O�^�[�Q�b�g
	float weights[8];						//!<�K�E�X�u���[�Ŏg���d�݃e�[�u���B

};

