#include "stdafx.h"
#include "Bloom.h"


void CBloom::UpdateWeight(float dispersion)
{
	float total = 0;
	for (int i = 0; i<NUM_WEIGHTS; i++) {
		weights[i] = expf(-0.5f*(float)(i*i) / dispersion);
		total += 2.0f*weights[i];

	}
	// �K�i��
	for (int i = 0; i < NUM_WEIGHTS; i++) {
		weights[i] /= total;
	}
}

CBloom::CBloom()
{
	//�u���[���̃V�F�[�_�[�����[�h
	m_BloomEffect = g_effectManager->LoadEffect("Assets/Shader/Bloom.fx");

	//�P�x���o�p�̃����_�����O�^�[�Q�b�g���쐬����B
	m_luminanceRenderTarget.Create(
		1280,		//���ƍ����̓t���[���o�b�t�@�Ɠ����ɂ��Ă����B
		 720,
		1,
		D3DFMT_A16B16G16R16F,	//���������������_�o�b�t�@�ɂ���B
		D3DFMT_D16,				//�g��Ȃ��̂�16bit�B�{���͍쐬����K�v���Ȃ��B
		D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�̎�ށB����̓}���`�T���v�����O�͍s��Ȃ��̂�D3DMULTISAMPLE_NONE�ł����B
		0						//�}���`�T���v�����O�̕i�����x���B����̓}���`�T���v�����O�͍s��Ȃ��̂�0�ł����B
	);
	//�P�x���o�p�̃����_�����O�^�[�Q�b�g���쐬����B
	m_combineRenderTarget.Create(
		640,		//���ƍ����̓t���[���o�b�t�@�Ɠ����ɂ��Ă����B
		360,
		1,
		D3DFMT_A16B16G16R16F,	//���������������_�o�b�t�@�ɂ���B
		D3DFMT_D16,				//�g��Ȃ��̂�16bit�B�{���͍쐬����K�v���Ȃ��B
		D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�̎�ށB����̓}���`�T���v�����O�͍s��Ȃ��̂�D3DMULTISAMPLE_NONE�ł����B
		0						//�}���`�T���v�����O�̕i�����x���B����̓}���`�T���v�����O�͍s��Ȃ��̂�0�ł����B
	);

	int W = 1280;
	int H =  720;
	for (int i = 0;i < MGF_DOWN_SAMPLE_COUNT;i++)
	{
		//�u���[�������邽�߂̃_�E���T���v�����O�p�̃����_�����O�^�[�Q�b�g���쐬�B
		//���u���[�p�B
		W /= 2;
		m_downSamplingRenderTarget[i][0].Create(
			W,	//���̉𑜓x���t���[���o�b�t�@�̔����ɂ���B
			H,
			1,
			D3DFMT_A16B16G16R16F,	//���������������_�o�b�t�@�ɂ���B
			D3DFMT_D16,				//�g��Ȃ��̂�16bit�B�{���͍쐬����K�v���Ȃ��B
			D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�̎�ށB����̓}���`�T���v�����O�͍s��Ȃ��̂�D3DMULTISAMPLE_NONE�ł����B
			0						//�}���`�T���v�����O�̕i�����x���B����̓}���`�T���v�����O�͍s��Ȃ��̂�0�ł����B
		);

		H /= 2;
		//�c�u���[�p�B
		m_downSamplingRenderTarget[i][1].Create(
			W,	//�c�Ɖ��̉𑜓x���t���[���o�b�t�@�̔����ɂ���B
			H,
			1,
			D3DFMT_A16B16G16R16F,	//���������������_�o�b�t�@�ɂ���B
			D3DFMT_D16,				//�g��Ȃ��̂�16bit�B�{���͍쐬����K�v���Ȃ��B
			D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�̎�ށB����̓}���`�T���v�����O�͍s��Ȃ��̂�D3DMULTISAMPLE_NONE�ł����B
			0						//�}���`�T���v�����O�̕i�����x���B����̓}���`�T���v�����O�͍s��Ȃ��̂�0�ł����B
		);
	}
}
CBloom::~CBloom()
{
}

void CBloom::Render()
{

	// Z�e�X�g�Ŏ��s���Ă�������獢��̂ŁAZ�e�X�g�͖����ɂ��Ă����B
	//g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	// ���u�����h������Ȃ��B
	g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	CRenderTarget* rt = g_PostEffect->GetMainRenderTarget();
	//�܂��͋P�x�𒊏o����B
	{
		
		//�P�x���o�p�̃����_�����O�^�[�Q�b�g�ɕύX����B
		g_pd3dDevice->SetRenderTarget(0, m_luminanceRenderTarget.GetRenderTarget());
		g_pd3dDevice->SetDepthStencilSurface(m_luminanceRenderTarget.GetDepthStencilBuffer());
		//���ŃN���A�B
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		// �P�x���o�e�N�j�b�N���Z�b�g�B
		m_BloomEffect->SetTechnique("SamplingLuminance");
		m_BloomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		m_BloomEffect->BeginPass(0);
		//�V�[���e�N�X�`����ݒ肷��B
		m_BloomEffect->SetTexture("g_scene",rt->GetTexture());
		//�萔���W�X�^�ւ̕ύX���R�~�b�g�B
		m_BloomEffect->CommitChanges();
		g_PostEffect->RenderPrimitive();

		m_BloomEffect->EndPass();
		m_BloomEffect->End();
		// �ύX���������_�����O�X�e�[�g�����ɖ߂��B
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	}

	//�K�E�X�u���[�Ŏg���d�݃e�[�u�����X�V�B
	UpdateWeight(25.0f);
	CRenderTarget* prevRenderTarget = &m_luminanceRenderTarget;
	//�P�x�𒊏o�����e�N�X�`����X�u���[
	for (int i = 0;i < MGF_DOWN_SAMPLE_COUNT;i++)
	{
		{
			g_pd3dDevice->SetRenderTarget(0, m_downSamplingRenderTarget[i][0].GetRenderTarget());

			m_BloomEffect->SetTechnique("XBlur");
			m_BloomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			m_BloomEffect->BeginPass(0);

			//�P�x�̃e�N�X�`���T�C�Y�]��
			float size[2] = {
				(float)(prevRenderTarget->GetWidth()),
				(float)(prevRenderTarget->GetHeight())
			};
			m_BloomEffect->SetValue("g_luminanceTexSize", size, sizeof(size));

			//�I�t�Z�b�g�]��
			float offset[] = {
				16.0f / size[0],
				0.0f
			};
			m_BloomEffect->SetValue("g_offset", offset, sizeof(size));

			//�����_�����O�^�[�Q�b�g�̃T�C�Y��]��
			float renderTargetSize[2];
			renderTargetSize[0] = (float)m_downSamplingRenderTarget[i][0].GetWidth();
			renderTargetSize[1] = (float)m_downSamplingRenderTarget[i][0].GetHeight();
			m_BloomEffect->SetValue("g_renderTargetSize", renderTargetSize, sizeof(renderTargetSize));

			m_BloomEffect->SetValue("g_weight", weights, sizeof(weights));

			m_BloomEffect->SetTexture("g_blur", prevRenderTarget->GetTexture());
			m_BloomEffect->CommitChanges();
			g_PostEffect->RenderPrimitive();
			//DrawQuadPrimitive();

			m_BloomEffect->EndPass();
			m_BloomEffect->End();
		}

		prevRenderTarget = &m_downSamplingRenderTarget[i][0];
		//�P�x�𒊏o�����e�N�X�`����Y�u���[
		{
			g_pd3dDevice->SetRenderTarget(0, m_downSamplingRenderTarget[i][1].GetRenderTarget());

			m_BloomEffect->SetTechnique("YBlur");
			m_BloomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
			m_BloomEffect->BeginPass(0);
			//�P�x�e�N�X�`���]��
			float size[2] = {
				(float)(prevRenderTarget->GetWidth()),
				(float)(prevRenderTarget->GetHeight())
			};
			m_BloomEffect->SetValue("g_luminanceTexSize", size, sizeof(size));

			//�I�t�Z�b�g�]��
			float offset[] = {
				0.0f ,
				16.0f / size[1],
			};
			m_BloomEffect->SetValue("g_offset", offset, sizeof(size));

			//�����_�����O�^�[�Q�b�g�̃T�C�Y��]���B
			float renderTargetSize[2];
			renderTargetSize[0] = (float)m_downSamplingRenderTarget[i][1].GetWidth();
			renderTargetSize[1] = (float)m_downSamplingRenderTarget[i][1].GetHeight();
			m_BloomEffect->SetValue("g_renderTargetSize", renderTargetSize, sizeof(renderTargetSize));


			m_BloomEffect->SetValue("g_weight", weights, sizeof(weights));

			m_BloomEffect->SetTexture("g_blur", prevRenderTarget->GetTexture());
			m_BloomEffect->CommitChanges();
			g_PostEffect->RenderPrimitive();

			m_BloomEffect->EndPass();
			m_BloomEffect->End();

		}
		prevRenderTarget = &m_downSamplingRenderTarget[i][1];
	}

	//�{�P�t�B���^�[�̍���
	{
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderTarget(0, m_combineRenderTarget.GetRenderTarget());
		//g_pd3dDevice->Clear(0, nullptr, D3DCLEAR_TARGET, 0, 1.0f, 0);

		float offset[] = {
			0.5f / (float)(m_combineRenderTarget.GetWidth()) ,
			0.5f / (float)(m_combineRenderTarget.GetHeight()),
		};
		m_BloomEffect->SetTechnique("Combine");
		m_BloomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
		m_BloomEffect->BeginPass(0);

		m_BloomEffect->SetTexture("g_combineTex00", m_downSamplingRenderTarget[0][1].GetTexture());
		m_BloomEffect->SetTexture("g_combineTex01", m_downSamplingRenderTarget[1][1].GetTexture());
		m_BloomEffect->SetTexture("g_combineTex02", m_downSamplingRenderTarget[2][1].GetTexture());
		m_BloomEffect->SetTexture("g_combineTex03", m_downSamplingRenderTarget[3][1].GetTexture());
		//m_BloomEffect->SetTexture("g_combineTex04", m_downSamplingRenderTarget[4][1].GetTexture());

		m_BloomEffect->SetValue("g_offset", offset, sizeof(offset));
		m_BloomEffect->CommitChanges();
		g_PostEffect->RenderPrimitive();
		m_BloomEffect->EndPass();
		m_BloomEffect->End();
	}


	{
				//�ŏI�����B
				float offset[] = {
					0.5f / (float)m_combineRenderTarget.GetWidth() ,
					0.5f / (float)m_combineRenderTarget.GetHeight()
				};
				//�߂��B
				g_pd3dDevice->SetRenderTarget(0, rt->GetRenderTarget());
				g_pd3dDevice->SetDepthStencilSurface(rt->GetDepthStencilBuffer());
				//���Z�����B
				g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
				g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
				g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
				m_BloomEffect->SetTechnique("Final");
				m_BloomEffect->Begin(NULL, D3DXFX_DONOTSAVESHADERSTATE);
				m_BloomEffect->BeginPass(0);
				m_BloomEffect->SetTexture("g_blur", m_combineRenderTarget.GetTexture());
				m_BloomEffect->SetValue("g_offset", offset, sizeof(offset));
				m_BloomEffect->CommitChanges();

				g_PostEffect->RenderPrimitive();
				//DrawQuadPrimitive();

				m_BloomEffect->EndPass();
				m_BloomEffect->End();

				g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
				g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
				g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			}
	
	g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
}
