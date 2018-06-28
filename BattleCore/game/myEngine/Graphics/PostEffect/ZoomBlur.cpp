#include "stdafx.h"
#include "ZoomBlur.h"


CZoomBlur::CZoomBlur()
{
	m_ZoomRenderTarget.Create(
		1280,					//�t���[���o�b�t�@�̕�
		720,					//�t���[���o�b�t�@�̍���
		1,						//�~�b�v�}�b�v���x��
		D3DFMT_A8B8G8R8,	//�J���[�o�b�t�@�̃t�H�[�}�b�g
		D3DFMT_D24S8,			//�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g
		D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�̎��
		0						//�}���`�T���v�����O�̕i��
	);

	m_ZoomEffect = g_effectManager->LoadEffect("Assets/Shader/ZoomFilter.fx");

}


CZoomBlur::~CZoomBlur()
{

}

void CZoomBlur::Render()
{

	// Z�e�X�g�Ŏ��s���Ă�������獢��̂ŁAZ�e�X�g�͖����ɂ��Ă����B
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// ���u�����h������Ȃ��B
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	CRenderTarget* rt = g_PostEffect->GetMainRenderTarget();

	g_pd3dDevice->SetRenderTarget(0, rt->GetRenderTarget());
	g_pd3dDevice->SetDepthStencilSurface(rt->GetDepthStencilBuffer());

	//Monochrome��ݒ�B
	m_ZoomEffect->SetTechnique("Default");
	m_ZoomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_ZoomEffect->BeginPass(0);
	//�V�[���e�N�X�`����ݒ肷��
	m_ZoomEffect->SetTexture("g_scene",rt->GetTexture());
	m_ZoomEffect->SetFloat("g_BlurPower", 5.0f);
	//�萔���W�X�^�ւ̕ύX���R�~�b�g����B
	m_ZoomEffect->CommitChanges();
	g_PostEffect->RenderPrimitive();

	m_ZoomEffect->EndPass();
	m_ZoomEffect->End();

	//�Z�s�A
	//m_SepiaEffct->EndPass();
	//m_SepiaEffct->End();
	// �ύX���������_�����O�X�e�[�g�����ɖ߂��B
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

}
