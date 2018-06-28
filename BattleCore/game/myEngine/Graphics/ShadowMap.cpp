#include "stdafx.h"
#include "ShadowMap.h"
#include "myEngine\Graphics\SkinModel.h"


CShadowMap::CShadowMap()
{
	for (int i = 0;i < NUM_SHADOW_MAP;i++)
	{
		D3DXMatrixIdentity(&lightViewMatrix[i]);
		D3DXMatrixIdentity(&lightProjMatrix[i]);
	}
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
	float HW = 100.0f;
	float resolution = 1024;
	for (int i = 0;i < NUM_SHADOW_MAP;i++)
	{
		SrenderTarget[i].Create(
			resolution,
			resolution,
			1,						//�����_�����O�^�[�Q�b�g�ɂ̓~�b�v�}�b�v�͕s�v�Ȃ̂ňꖇ�̂݁B
			D3DFMT_R16F,		    //Z�̐��x
			D3DFMT_D24S8,		    //�w���̂o�b��24bit�̐[�x�o�b�t�@���쐬�ł��Ȃ������̂ŁA16�r�b�g�Ő[�x�o�b�t�@���쐬����B
			D3DMULTISAMPLE_NONE,	//�}���`�T���v�����O�͂Ȃ��B
			0						//�}���`�T���v�����O���Ȃ��̂łO���w��B
		);
		//���ˉe�s��
		D3DXMatrixOrthoLH(
			&lightProjMatrix[i],
			HW,
			HW,
			Near,
			Far

		);
		HW += 300;
		resolution -= 200;
	}
	
}


//�X�V�B
void CShadowMap::Update()
{
	if (game->GetPlayer() == NULL) { return; }
	
	
		D3DXVECTOR3 target = game->GetGameCamara()->Getcamera()->GetLookatPt();
		D3DXVECTOR3 viewPos = game->GetMap()->GetSun()->GetPosition();
		viewPos.y += 400;
		
		for (int i = 0; i < NUM_SHADOW_MAP;i++)
		{
			

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
				D3DXMatrixLookAtLH(&lightViewMatrix[i], &viewPosition, &viewTarget, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
				D3DXVec3Cross(&lightViewUp, &tmp, &D3DXVECTOR3(1.0f, 0.0f, 0.0f));
			}
			else {
				D3DXMatrixLookAtLH(&lightViewMatrix[i], &viewPosition, &viewTarget, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
				D3DXVec3Cross(&lightViewUp, &tmp, &D3DXVECTOR3(0.0f, 1.0f, 0.0f));
			}

			
			/*

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

			//�J�����̍���
			float shadowAreaTbl[] = {
				80.0f,
				160.0f,
				240.0f
			};

			//���C�g�r���[�̃^�[�Q�b�g���v�Z�B
			D3DXVECTOR3 lightTarget;
			lightTarget = target;
			D3DXVECTOR3 lightPos = lightTarget + tmp * -120.0f;


			SShadowCb shadowCB;
			float nearPlaneZ = 0.0f;
			float farPlaneZ;

			D3DXVECTOR3 cameraUp;

			D3DXVec3Cross(&cameraUp, &game->GetGameCamara()->Getcamera()->GetRight(), &game->GetGameCamara()->Getcamera()->GetFront());
			
			//for (int i = 0; i < 3; i++) {

				
				farPlaneZ = nearPlaneZ + shadowAreaTbl[i];
				//�P�ʍs��
				D3DXMATRIX mLightView = { 1.0f, 0.0f, 0.0f, 0.0f ,
										  0.0f, 1.0f, 0.0f, 0.0f ,
										  0.0f, 0.0f, 1.0f, 0.0f ,
										  0.0f, 0.0f, 0.0f, 1.0f };

				//�����̃A���O�������߂�
				float halfViewAngle = game->GetGameCamara()->Getcamera()->GetViewAngle() * 0.5f;
				//�������8���_�����C�g��Ԃɕϊ�����AABB�����߂āA���ˉe�̕��ƍ��������߂�B
				float w, h;
				float fFar, fNear;
				D3DXVECTOR3 v[8];
				{
					float t = tan(halfViewAngle);
					D3DXVECTOR3 toUpperNear, toUpperFar;
					toUpperNear = cameraUp * t * nearPlaneZ;
					toUpperFar = cameraUp * t * farPlaneZ;
					t *= game->GetGameCamara()->Getcamera()->GetAspect();
					//�ߕ��ʂ̒������W���v�Z�B
					D3DXVECTOR3 vWk = game->GetGameCamara()->Getcamera()->GetEyePt() + tmp * nearPlaneZ;
					lightPos = vWk;
					v[0] = vWk + game->GetGameCamara()->Getcamera()->GetRight() * t * nearPlaneZ + toUpperNear;
					v[1] = v[0] - toUpperNear * 2.0f;

					v[2] = vWk + game->GetGameCamara()->Getcamera()->GetRight() * -t * nearPlaneZ + toUpperNear;
					v[3] = v[2] - toUpperNear * 2.0f;

					vWk = game->GetGameCamara()->Getcamera()->GetEyePt() + tmp* farPlaneZ;
					lightPos += vWk;
					v[4] = vWk + game->GetGameCamara()->Getcamera()->GetRight() * t * farPlaneZ + toUpperFar;
					v[5] = v[4] - toUpperFar * 2.0f;
					v[6] = vWk + game->GetGameCamara()->Getcamera()->GetRight() * -t * farPlaneZ + toUpperFar;
					v[7] = v[6] - toUpperFar * 2.0f;

					//���C�g�s����쐬�B
					lightPos *= 0.5f;
					lightPos += m_lightDirection * -120.0f;

					mLightView = lightViewRot;
					mLightView.m[3][0] = lightPos.x;
					mLightView.m[3][1] = lightPos.y;
					mLightView.m[3][2] = lightPos.z;
					mLightView.m[3][3] = 1.0f;
					D3DXMatrixInverse(&mLightView, NULL, &mLightView);//���C�g�r���[�����B
													//��������\������8���_���v�Z�ł����̂ŁA���C�g��Ԃɍ��W�ϊ����āAAABB�����߂�B
					D3DXVECTOR3 vMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
					D3DXVECTOR3 vMin = { FLT_MAX, FLT_MAX, FLT_MAX };
					for (auto& vInLight : v) {
						vInLight.x = vInLight.x * mLightView.m[0][0] + vInLight.y * mLightView.m[1][0] + vInLight.z * mLightView.m[2][0] + mLightView.m[3][0];
						vInLight.x = vInLight.x * mLightView.m[0][1] + vInLight.y * mLightView.m[1][1] + vInLight.z * mLightView.m[2][1] + mLightView.m[3][1];
						vInLight.x = vInLight.x * mLightView.m[0][2] + vInLight.y * mLightView.m[1][2] + vInLight.z * mLightView.m[2][2] + mLightView.m[3][2];
						vMax.x = max(vMax.x, vInLight.x);
						vMax.y = max(vMax.y, vInLight.y);
						vMax.z = max(vMax.z, vInLight.z);
						vMin.x = min(vMin.x, vInLight.x);
						vMin.y = min(vMin.y, vInLight.y);
						vMin.z = min(vMin.z, vInLight.z);

					}

					w = vMax.x - vMin.x;
					h = vMax.y - vMin.y;
				}

				//���ˉe�s��
				D3DXMatrixOrthoLH(
					&lightProjMatrix[i],
					w,
					h,
					Near,
					Far

				);
				*/
		
				
		}
}

//�V���h�E�}�b�v�ɏ������݁B
void CShadowMap::Draw()
{
	if (game->GetPlayer() == NULL) { return; }
		Update();
		
		LPDIRECT3DSURFACE9 renderTargetBackup;
		LPDIRECT3DSURFACE9 depthBufferBackup;
		g_pd3dDevice->GetRenderTarget(0, &renderTargetBackup);		//���X�̃����_�����O�^�[�Q�b�g��ۑ��B��Ŗ߂��K�v������̂ŁB
		g_pd3dDevice->GetDepthStencilSurface(&depthBufferBackup);	//���X�̃f�v�X�X�e���V���o�b�t�@��ۑ��B��Ŗ߂��K�v������̂ŁB

		for (int i = 0;i < NUM_SHADOW_MAP;i++)
		{
			g_pd3dDevice->BeginScene();
			//�����_�����O�^�[�Q�b�g��ύX����B
			g_pd3dDevice->SetRenderTarget(0, SrenderTarget[i].GetRenderTarget());
			g_pd3dDevice->SetDepthStencilSurface(SrenderTarget[i].GetDepthStencilBuffer());
			//�������ݐ��ύX�����̂ŃN���A�B
			g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);


			game->GetPlayer()->LightEyePosRender(lightViewMatrix[i], lightProjMatrix[i]);

			for (const auto& Enemy : game->GetEnemys()) {
				Enemy->LightEyePosRender(lightViewMatrix[i], lightProjMatrix[i]);
			}
			if (game->GetMap()->GetnomalEnemyCount()== 0)
			{
				for (const auto& Enemys : game->GetMap()->GetGroupenemy())
				{
					Enemys->LightEyePosRender(lightViewMatrix[i], lightProjMatrix[i]);
				}
			}
			/*for (auto bullet : game->GetPlayerBullet()) {
				bullet->LightEyePosRender(lightViewMatrix, lightProjMatrix);
			}*/
			game->GetMap()->ShadowMapRender(lightViewMatrix[i], lightProjMatrix[i]);
			g_pd3dDevice->EndScene();

		}
		g_pd3dDevice->SetRenderTarget(0, renderTargetBackup);		//�߂��B
		g_pd3dDevice->SetDepthStencilSurface(depthBufferBackup);	//�߂��B
		
	
}
