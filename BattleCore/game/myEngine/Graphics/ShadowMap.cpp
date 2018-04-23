#include "stdafx.h"
#include "ShadowMap.h"
#include "myEngine\Graphics\SkinModel.h"


int shadowCount = 1; //

CShadowMap::CShadowMap()
{
	D3DXMatrixIdentity(&lightViewMatrix);
	D3DXMatrixIdentity(&lightProjMatrix);
	m_lightDirection = { 0.0f, -1.0f, 0.0f };
	viewPosition.x = 0.0f;
	viewPosition.y = 0.0f;
	viewPosition.z = 0.0f;
	viewTarget.x = 0.0f;
	viewTarget.y = 0.0f;
	viewTarget.z = 0.0f;

	Near = 1.0f;
	Far = 2000.0f;
	m_viewAngle = 60.0f * (3.14159265358979323846 / 180.0f);
	aspect = (float)FRAME_BUFFER_WIDTH / (float)FRAME_BUFFER_HEIGHT;
	viewPosition = { 0.0f, 3.0f, 0.0f };
	viewTarget = { 0.0f,0.0f,0.0f };


}
CShadowMap::~CShadowMap()
{
}

//�������B0
void CShadowMap::Init()
{
	//�����_�����O�^�[�Q�b�g���������B
	SrenderTarget[0].Create(
		1280,
		1280,
		1,						//�����_�����O�^�[�Q�b�g�ɂ̓~�b�v�}�b�v�͕s�v�Ȃ̂ňꖇ�̂݁B
		D3DFMT_R16F,		    //Z�̐��x
		D3DFMT_D24S8,		    //�w���̂o�b��24bit�̐[�x�o�b�t�@���쐬�ł��Ȃ������̂ŁA16�r�b�g�Ő[�x�o�b�t�@���쐬����B
		D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�͂Ȃ��B
		0						//�}���`�T���v�����O���Ȃ��̂łO���w��B
	);
	for (int i = 1;i < 3;i++)
	{
		int W = 1280;
		int H = 1280;
		SrenderTarget[1].Create(
			W,
			H,
			1,						//�����_�����O�^�[�Q�b�g�ɂ̓~�b�v�}�b�v�͕s�v�Ȃ̂ňꖇ�̂݁B
			D3DFMT_R16F,		    //Z�̐��x
			D3DFMT_D24S8,		    //�w���̂o�b��24bit�̐[�x�o�b�t�@���쐬�ł��Ȃ������̂ŁA16�r�b�g�Ő[�x�o�b�t�@���쐬����B
			D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�͂Ȃ��B
			0						//�}���`�T���v�����O���Ȃ��̂łO���w��B
		);

	}
}


//�X�V�B
void CShadowMap::Update()
{
	if (game->GetPlayer() == NULL) { return; }
	

	D3DXVECTOR3 target = game->GetPlayer()->GetPos();
	D3DXVECTOR3 viewPos = target;
	viewPos.y += 100;
	shadowCount++;
	if (shadowCount == 3)
	{
		shadowCount = 1;
	}

	g_shadowMap.SetLightViewPosition(viewPos);
	g_shadowMap.SetLightViewTarget(target);


	//���C�g�r���[�v���W�F�N�V�����s����X�V�B
	//���ʂ̃J�����Ɠ����B
	//�J�����̏���������߂�v�Z��������Ă����B
	D3DXVECTOR3 tmp = viewTarget - viewPosition;
	D3DXVECTOR3 lightViewUp;
	D3DXVec3Normalize(&tmp, &tmp);
	if (fabsf(tmp.y) > 0.9999f) {
		//�J�������قڐ^��or�^���������Ă���B
		D3DXMatrixLookAtLH(&lightViewMatrix, &viewPosition, &viewTarget, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
		D3DXVec3Cross(&lightViewUp, &tmp, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
	}
	else {
		D3DXMatrixLookAtLH(&lightViewMatrix, &viewPosition, &viewTarget, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
		D3DXVec3Cross(&lightViewUp, &tmp, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
	}
	D3DXVec3Normalize(&lightViewUp, &lightViewUp);
	D3DXVECTOR3 lightViewRight;
	D3DXVec3Cross(&lightViewRight, &D3DXVECTOR3(0.0f, 1.0f, 0.0f), &tmp);
	D3DXVec3Normalize(&lightViewRight, &lightViewRight);

	D3DXMATRIX lightViewRot;
	//���C�g�r���[�̉���ݒ肷��B
	lightViewRot.m[0][0] = lightViewRight.x;
	lightViewRot.m[0][1] = lightViewRight.y;
	lightViewRot.m[0][2] = lightViewRight.z;
	lightViewRot.m[0][3] = 0.0f;
	//���C�g�r���[�̏��ݒ肷��B
	lightViewRot.m[1][0] = lightViewUp.x;
	lightViewRot.m[1][1] = lightViewUp.y;
	lightViewRot.m[1][2] = lightViewUp.z;
	lightViewRot.m[1][3] = 0.0f;
	//���C�g�r���[�̑O��ݒ肷��B
	lightViewRot.m[2][0] = tmp.x;
	lightViewRot.m[2][1] = tmp.y;
	lightViewRot.m[2][2] = tmp.z;
	lightViewRot.m[2][3] = 0.0f;

	//D3DXMATRIXA16 matProj;
	D3DXMatrixPerspectiveFovLH(
		&lightProjMatrix, 
		m_viewAngle, 
		aspect, 
		Near, 
		Far
	    );


}

//�V���h�E�}�b�v�ɏ������݁B
void CShadowMap::Draw()
{
	if (game->GetPlayer() == NULL) { return; }
		Update();
		g_pd3dDevice->BeginScene();
		LPDIRECT3DSURFACE9 renderTargetBackup;
		LPDIRECT3DSURFACE9 depthBufferBackup;
		g_pd3dDevice->GetRenderTarget(0, &renderTargetBackup);		//���X�̃����_�����O�^�[�Q�b�g��ۑ��B��Ŗ߂��K�v������̂ŁB
		g_pd3dDevice->GetDepthStencilSurface(&depthBufferBackup);	//���X�̃f�v�X�X�e���V���o�b�t�@��ۑ��B��Ŗ߂��K�v������̂ŁB
																	//�����_�����O�^�[�Q�b�g��ύX����B
		g_pd3dDevice->SetRenderTarget(0, SrenderTarget[0].GetRenderTarget());
		g_pd3dDevice->SetDepthStencilSurface(SrenderTarget[0].GetDepthStencilBuffer());
		//�������ݐ��ύX�����̂ŃN���A�B
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);

		game->GetPlayer()->LightEyePosRender(lightViewMatrix, lightProjMatrix);

		for (const auto& Enemy : game->GetEnemys()) {
			Enemy->LightEyePosRender(lightViewMatrix, lightProjMatrix);
		}
		if (game->GetEnemys().size() == 0)
		{
			for (const auto& Enemys : game->GetMap()->GetGroupenemy())
			{
				Enemys->LightEyePosRender(lightViewMatrix, lightProjMatrix);
			}
		}
		/*for (auto bullet : game->GetPlayerBullet()) {
			bullet->LightEyePosRender(lightViewMatrix, lightProjMatrix);
		}*/
		//game->GetMap()->ShadowMapRender(lightViewMatrix, lightProjMatrix);

		g_pd3dDevice->SetRenderTarget(0, renderTargetBackup);		//�߂��B
		g_pd3dDevice->SetDepthStencilSurface(depthBufferBackup);	//�߂��B
		g_pd3dDevice->EndScene();
	
}
