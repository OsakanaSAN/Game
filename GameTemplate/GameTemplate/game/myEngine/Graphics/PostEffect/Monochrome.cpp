#include "stdafx.h"
#include "Monochrome.h"


CMonochrome::CMonochrome()
{
	

	m_RenderTarget.Create(
		1280,					//�t���[���o�b�t�@�̕�
		720,					//�t���[���o�b�t�@�̍���
		1,					//�~�b�v�}�b�v���x��
		D3DFMT_A16B16G16R16F,	//�J���[�o�b�t�@�̃t�H�[�}�b�g
		D3DFMT_D24S8,			//�f�v�X�X�e���V���o�b�t�@�̃t�H�[�}�b�g
		D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�̎��
		0						//�}���`�T���v�����O�̕i��
	);

	m_MonochromeEffect = g_effectManager->LoadEffect("Assets/Shader/Monochrome.fx");

}


CMonochrome::~CMonochrome()
{

	
}

void CMonochrome::Render()
{
	if (game->GetPlayer() == NULL || game->GetPlayer()->GetHp() >= 100)
	{
		m_AlphaPower = 1.0f;
		return; }
	if (m_AlphaPower >= 0)
	{
		m_AlphaPower -= GameTime().GetFrameDeltaTime() / 2;
	}
	// Z�e�X�g�Ŏ��s���Ă�������獢��̂ŁAZ�e�X�g�͖����ɂ��Ă����B
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// ���u�����h������Ȃ��B
	//g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	CRenderTarget* rt = g_PostEffect->GetMainRenderTarget();

	g_pd3dDevice->SetRenderTarget(0, rt->GetRenderTarget());
	g_pd3dDevice->SetDepthStencilSurface(rt->GetDepthStencilBuffer());

	//Monochrome��ݒ�B
	m_MonochromeEffect->SetTechnique("Default");
	m_MonochromeEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_MonochromeEffect->BeginPass(0);
	//�V�[���e�N�X�`����ݒ肷��
	m_MonochromeEffect->SetTexture("g_scene", rt->GetTexture());
	m_MonochromeEffect->SetFloat("g_sceneTexSizeX", 1280.0f);
	m_MonochromeEffect->SetFloat("g_sceneTexSizeY", 720.0f);
	m_MonochromeEffect->SetFloat("g_AlphaPower",m_AlphaPower);
	//�萔���W�X�^�ւ̕ύX���R�~�b�g����B
	m_MonochromeEffect->CommitChanges();
	g_PostEffect->RenderPrimitive();
	
	m_MonochromeEffect->EndPass();
	m_MonochromeEffect->End();

	//�Z�s�A
	//m_SepiaEffct->EndPass();
	//m_SepiaEffct->End();
	// �ύX���������_�����O�X�e�[�g�����ɖ߂��B
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	//g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
}
