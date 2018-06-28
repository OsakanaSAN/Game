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

//初期化。0
void CShadowMap::Init()
{
	//レンダリングターゲットを初期化。
	float HW = 100.0f;
	float resolution = 1024;
	for (int i = 0;i < NUM_SHADOW_MAP;i++)
	{
		SrenderTarget[i].Create(
			resolution,
			resolution,
			1,						//レンダリングターゲットにはミップマップは不要なので一枚のみ。
			D3DFMT_R16F,		    //Zの精度
			D3DFMT_D24S8,		    //学生のＰＣで24bitの深度バッファを作成できなかったので、16ビットで深度バッファを作成する。
			D3DMULTISAMPLE_NONE,	//マルチサンプリングはなし。
			0						//マルチサンプリングしないので０を指定。
		);
		//正射影行列
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


//更新。
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


			//ライトビュープロジェクション行列を更新。
			//普通のカメラと同じ。
			//カメラの上方向を決める計算だけ入れておく。
			D3DXVECTOR3 tmp = viewTarget - viewPosition;
			D3DXVECTOR3 lightViewUp;
			D3DXVec3Normalize(&tmp, &tmp);


			if (fabsf(tmp.y) > 0.9999f) {
				//カメラがほぼ真上or真下を向いている。
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
			//ライトビューの横を設定する。
			lightViewRot.m[0][0] = lightViewRight.x;
			lightViewRot.m[0][1] = lightViewRight.y;
			lightViewRot.m[0][2] = lightViewRight.z;
			lightViewRot.m[0][3] = 0.0f;
			//ライトビューの上を設定する。
			lightViewRot.m[1][0] = lightViewUp.x;
			lightViewRot.m[1][1] = lightViewUp.y;
			lightViewRot.m[1][2] = lightViewUp.z;
			lightViewRot.m[1][3] = 0.0f;
			//ライトビューの前を設定する。
			lightViewRot.m[2][0] = tmp.x;
			lightViewRot.m[2][1] = tmp.y;
			lightViewRot.m[2][2] = tmp.z;
			lightViewRot.m[2][3] = 0.0f;

			//カメラの高さ
			float shadowAreaTbl[] = {
				80.0f,
				160.0f,
				240.0f
			};

			//ライトビューのターゲットを計算。
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
				//単位行列
				D3DXMATRIX mLightView = { 1.0f, 0.0f, 0.0f, 0.0f ,
										  0.0f, 1.0f, 0.0f, 0.0f ,
										  0.0f, 0.0f, 1.0f, 0.0f ,
										  0.0f, 0.0f, 0.0f, 1.0f };

				//半分のアングルを求める
				float halfViewAngle = game->GetGameCamara()->Getcamera()->GetViewAngle() * 0.5f;
				//視推台の8頂点をライト空間に変換してAABBを求めて、正射影の幅と高さを求める。
				float w, h;
				float fFar, fNear;
				D3DXVECTOR3 v[8];
				{
					float t = tan(halfViewAngle);
					D3DXVECTOR3 toUpperNear, toUpperFar;
					toUpperNear = cameraUp * t * nearPlaneZ;
					toUpperFar = cameraUp * t * farPlaneZ;
					t *= game->GetGameCamara()->Getcamera()->GetAspect();
					//近平面の中央座標を計算。
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

					//ライト行列を作成。
					lightPos *= 0.5f;
					lightPos += m_lightDirection * -120.0f;

					mLightView = lightViewRot;
					mLightView.m[3][0] = lightPos.x;
					mLightView.m[3][1] = lightPos.y;
					mLightView.m[3][2] = lightPos.z;
					mLightView.m[3][3] = 1.0f;
					D3DXMatrixInverse(&mLightView, NULL, &mLightView);//ライトビュー完成。
													//視推台を構成する8頂点が計算できたので、ライト空間に座標変換して、AABBを求める。
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

				//正射影行列
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

//シャドウマップに書き込み。
void CShadowMap::Draw()
{
	if (game->GetPlayer() == NULL) { return; }
		Update();
		
		LPDIRECT3DSURFACE9 renderTargetBackup;
		LPDIRECT3DSURFACE9 depthBufferBackup;
		g_pd3dDevice->GetRenderTarget(0, &renderTargetBackup);		//元々のレンダリングターゲットを保存。後で戻す必要があるので。
		g_pd3dDevice->GetDepthStencilSurface(&depthBufferBackup);	//元々のデプスステンシルバッファを保存。後で戻す必要があるので。

		for (int i = 0;i < NUM_SHADOW_MAP;i++)
		{
			g_pd3dDevice->BeginScene();
			//レンダリングターゲットを変更する。
			g_pd3dDevice->SetRenderTarget(0, SrenderTarget[i].GetRenderTarget());
			g_pd3dDevice->SetDepthStencilSurface(SrenderTarget[i].GetDepthStencilBuffer());
			//書き込み先を変更したのでクリア。
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
		g_pd3dDevice->SetRenderTarget(0, renderTargetBackup);		//戻す。
		g_pd3dDevice->SetDepthStencilSurface(depthBufferBackup);	//戻す。
		
	
}
