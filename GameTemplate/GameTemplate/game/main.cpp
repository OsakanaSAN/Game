/*!
 *@brief	�Q�[���e���v���[�g�B
 */
#include "stdafx.h"
#include "myEngine/Graphics/Camera.h"
#include "myEngine/Graphics/Light.h"
#include "game.h"
#include "myEngine/Graphics/PostEffect/PostEffect.h"

Game* game;
CPostEffect* g_PostEffect;
CSoundEngine* g_SoundEngine;

//-----------------------------------------------------------------------------
// Name: �Q�[�����������B
//-----------------------------------------------------------------------------
void Init()
{
	g_PostEffect = new CPostEffect;
	//���C�������_�����O�^�[�Q�b�g���쐬
	g_PostEffect->InitMainRenderTarget();
	//�|���̏�����
	g_PostEffect->InitPrimitive();
	//�|�X�g�G�t�F�N�g�p�̃V�F�[�_�[���[�h
	g_PostEffect->LoadShader();
	
	
	game = new Game;
	game->Start();
}
//-----------------------------------------------------------------------------
// Name: �`�揈���B
//-----------------------------------------------------------------------------
VOID Render()
{

	//�V���h�E�}�b�v�Ƀ����_�����O�B
	g_shadowMap.Draw();

	// ��ʂ��N���A�B
	g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);
	
	//�V�[���̕`��J�n�B
	
	////�����������̐ݒ�B
	//g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	if (game->GetPlayer() != NULL)
	{

		g_PostEffect->Render();

	}
	else
	{
		g_pd3dDevice->BeginScene();
		game->Render();
		game->Render2D();
	}
	

	//// �V�[���̕`��I���B
	g_pd3dDevice->EndScene();
	//// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�����ւ���B
	g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

}
/*!-----------------------------------------------------------------------------
 *@brief	�X�V�����B
 -----------------------------------------------------------------------------*/
void Update()
{
	
	game->Update();
}
//-----------------------------------------------------------------------------
//�Q�[�����I������Ƃ��ɌĂ΂�鏈���B
//-----------------------------------------------------------------------------
void Terminate()
{
	delete game;
	delete g_effectManager;
}
