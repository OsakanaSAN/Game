#pragma once
class CZoomBlur
{
public:
	CZoomBlur();
	~CZoomBlur();
	void Render();

private:

	LPD3DXEFFECT	m_ZoomEffect;		//�V�F�[�_�[�G�t�F�N�g�B
	CRenderTarget	m_ZoomRenderTarget;			//�����_�����O�^�[�Q�b�g�B
	float			m_BlurPower = 0.0f;

};

