#include "stdafx.h"
#include "CSoftShadow.h"

void CSoftShadow::UpdateWeight(float dispersion)
{
	float total = 0;
	for (int i = 0; i<8; i++) {
		weights[i] = expf(-0.5f*(float)(i*i) / dispersion);
		total += 2.0f*weights[i];

	}
	// �K�i��
	for (int i = 0; i < 8; i++) {
		weights[i] /= total;
	}
}

CSoftShadow::CSoftShadow()
{
	
	
	m_softEffect = g_effectManager->LoadEffect("Assets/Shader/SoftShadow.fx");

}


CSoftShadow::~CSoftShadow()
{
}


void CSoftShadow::Render()
{
	if (game->GetPlayer() == NULL) { return; }
	// Z�e�X�g�Ŏ��s���Ă�������獢��̂ŁAZ�e�X�g�͖����ɂ��Ă����B
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// ���u�����h������Ȃ��B
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	//CRenderTarget* rt = g_PostEffect->GetMainRenderTarget();

	LPDIRECT3DSURFACE9 renderTargetBackup;
	g_pd3dDevice->GetRenderTarget(0, &renderTargetBackup);		//���X�̃����_�����O�^�[�Q�b�g��ۑ��B��Ŗ߂��K�v������̂ŁB

	UpdateWeight(25.0f);
																//�����_�����O�^�[�Q�b�g��ύX����B
	g_pd3dDevice->SetRenderTarget(0, g_shadowMap.GetRenderTarget().GetRenderTarget());
	m_softEffect->SetTechnique("XBlur");
	m_softEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
	m_softEffect->BeginPass(0);

	//�P�x�̃e�N�X�`���T�C�Y�]��
	float size[2] = {
		(float)(1280.0f),
		(float)(720.0f)
	};
	m_softEffect->SetValue("g_luminanceTexSize", size, sizeof(size));

	//�I�t�Z�b�g�]��
	float offset[] = {
		16.0f / size[0],
		0.0f
	};
	m_softEffect->SetValue("g_offset", offset, sizeof(size));

	//�����_�����O�^�[�Q�b�g�̃T�C�Y��]��
	float renderTargetSize[2];
	renderTargetSize[0] = (float)g_shadowMap.GetRenderTarget().GetWidth();
	renderTargetSize[1] = (float)g_shadowMap.GetRenderTarget().GetHeight();
	m_softEffect->SetValue("g_renderTargetSize", renderTargetSize, sizeof(renderTargetSize));

	m_softEffect->SetValue("g_weight", weights, sizeof(weights));
	//�V�[���e�N�X�`����ݒ肷��
	m_softEffect->SetTexture("g_scene", g_shadowMap.GetTexture(0));
	//�萔���W�X�^�ւ̕ύX���R�~�b�g����B
	m_softEffect->CommitChanges();
	g_PostEffect->RenderPrimitive();

	m_softEffect->EndPass();
	m_softEffect->End();

	// �ύX���������_�����O�X�e�[�g�����ɖ߂��B
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);


	g_pd3dDevice->SetRenderTarget(0, renderTargetBackup);
	


}
