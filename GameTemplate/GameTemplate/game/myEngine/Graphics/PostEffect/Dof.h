#pragma once
class CDof
{
public:
	CDof();
	~CDof();
	void Render();

private:
	LPD3DXEFFECT		m_DofEffect;						//!<�V�F�[�_�[�G�t�F�N�g�B
	CRenderTarget		m_DepthRenderTarget;				//�[�x���������ރ����_�����O�^�[�Q�b�g
	CRenderTarget		m_combineRenderTarget;				//�{�P�����p�̃����_�����O�^�[�Q�b�g�B


};

