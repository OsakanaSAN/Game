#include "stdafx.h"
#include "myEngine/Graphics/SkinModel.h"
#include "myEngine/Graphics/SkinModelData.h"
#include "myEngine/Graphics/Light.h"

extern UINT                 g_NumBoneMatricesMax;
extern D3DXMATRIXA16*       g_pBoneMatrices ;
LPDIRECT3DCUBETEXTURE9      ppCubeTexture;

static const int		LIGHT_NUM = 4;
D3DXVECTOR4 			g_diffuseLightDirection[LIGHT_NUM];	//ライトの方向。
D3DXVECTOR4				g_diffuseLightColor[LIGHT_NUM];		//ライトの色。
D3DXVECTOR4				g_ambientLight;						//環境光
float                   g_Moveuv = 0.00f;                   //波の法線マップの移動
D3DXVECTOR2             g_farNera;
bool                    ReWave = false;


namespace {
	void DrawMeshContainer(
		IDirect3DDevice9* pd3dDevice, 
		LPD3DXMESHCONTAINER pMeshContainerBase, 
		LPD3DXFRAME pFrameBase,
		ID3DXEffect* pEffect,
		D3DXMATRIX* worldMatrix,
		D3DXMATRIX* rotationMatrix,
		D3DXMATRIX* viewMatrix,
		D3DXMATRIX* projMatrix,
		Light* light,
		bool   IsDrawShadowMap,
		bool   IsRecieveShadow,
		bool   IsWave,
		LPDIRECT3DTEXTURE9 NormalMap,
		bool   IsSky,
		LPDIRECT3DCUBETEXTURE9      CubeMap

	)
	{
		D3DXMESHCONTAINER_DERIVED* pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerBase;
		D3DXFRAME_DERIVED* pFrame = (D3DXFRAME_DERIVED*)pFrameBase;
		UINT iAttrib;
		LPD3DXBONECOMBINATION pBoneComb;

		D3DXMATRIXA16 matTemp;
		D3DCAPS9 d3dCaps;
		pd3dDevice->GetDeviceCaps(&d3dCaps);
		D3DXMATRIX viewProj;
		D3DXMatrixMultiply(&viewProj, viewMatrix, projMatrix);
		
		//フォグ値
		D3DXVECTOR2             fog;
		//フォグの計算
		fog.x = game->GetCamera()->GetFar() / (game->GetCamera()->GetFar() - game->GetCamera()->GetNear());
		fog.y = -1 / (game->GetCamera()->GetFar() - game->GetCamera()->GetNear());

		//テクニックを設定。
		{

			if (pMeshContainer->pSkinInfo != NULL) {

				if (IsDrawShadowMap)
					{
							//シャドウマップへの書き込みテクニック。
							pEffect->SetTechnique("SkinModelRenderToShadowMap");
					}

					else
					{

							pEffect->SetTechnique("SkinModel");
					}
			}
			else {

				if (IsWave)
				{
					pEffect->SetTechnique("NoSkinModelRenderToWave");
				}

				else if (IsSky)
				{
					pEffect->SetTechnique("NoSkinModelRenderToSkyMap");
				}

				else if (IsDrawShadowMap)
				{
					//シャドウマップへの書き込みテクニック。
					pEffect->SetTechnique("NoSkinModelRenderToShadowMap");
				}

				else
				{

					pEffect->SetTechnique("NoSkinModel");
				}
			}
				
				
			
			

		}

		//共通の定数レジスタを設定
		{
			//ビュープロジェクション
			pEffect->SetMatrix("g_mViewProj", &viewProj);
			//ライト
			pEffect->SetValue(
				"g_light",
				light,
				sizeof(Light)
			);

		}
	
		if (IsWave)
		{
			//法線マップの有無判定
		    pEffect->SetBool("g_isWave", true);
			pEffect->SetTexture("g_normalTexture", NormalMap); //法線マップ
			pEffect->SetFloat("g_moveUV", g_Moveuv);		   //法線マップを動かすための処理
			g_Moveuv += 0.0001f;
			if (g_Moveuv == 0.01)
			{
				g_Moveuv = 0;
			}
		}
		else {
			pEffect->SetBool("g_isWave", false);

		}

		if (IsRecieveShadow) {

			pEffect->SetTexture("g_shadowMapTexture", g_shadowMap.GetTexture());
			pEffect->SetMatrix("g_lightViewMatrix", &g_shadowMap.GetLightViewMatrix());
			pEffect->SetMatrix("g_lightProjectionMatrix", &g_shadowMap.GetLightProjectionMatrix());

		}

		pEffect->SetTexture("g_CubeTexture",CubeMap);
		pEffect->SetFloat("g_Fog_X", fog.x);
		pEffect->SetFloat("g_Fog_Y", fog.y);

		if (pMeshContainer->pSkinInfo != NULL)
		{
			//スキン情報有り。
			pBoneComb = reinterpret_cast<LPD3DXBONECOMBINATION>(pMeshContainer->pBoneCombinationBuf->GetBufferPointer());
			for (iAttrib = 0; iAttrib < pMeshContainer->NumAttributeGroups; iAttrib++)
			{
				// first calculate all the world matrices
				for (DWORD iPaletteEntry = 0; iPaletteEntry < pMeshContainer->NumPaletteEntries; ++iPaletteEntry)
				{
					DWORD iMatrixIndex = pBoneComb[iAttrib].BoneId[iPaletteEntry];
					if (iMatrixIndex != UINT_MAX)
					{
						D3DXMatrixMultiply(
							&g_pBoneMatrices[iPaletteEntry],
							&pMeshContainer->pBoneOffsetMatrices[iMatrixIndex],
							pMeshContainer->ppBoneMatrixPtrs[iMatrixIndex]
						);
						//D3DXMatrixMultiply(&g_pBoneMatrices[iPaletteEntry], &matTemp, &g_matView);
					}
				}
			
				Camera camera;
				camera.SetFar(100);
				camera.SetEyePt({ 0.0f, 15000.0f, 0.0f });
				D3DXVECTOR4 g_pos = camera.GetEyePt();//game->GetCamera()->GetEyePt();
				g_pos.w = 1.0f;
				pEffect->SetVector("g_Eyeposition", &g_pos);
				pEffect->SetMatrixArray("g_mWorldMatrixArray", g_pBoneMatrices, pMeshContainer->NumPaletteEntries);
				pEffect->SetInt("g_numBone", pMeshContainer->NumInfl);
				// ディフューズテクスチャ。
				pEffect->SetTexture("g_diffuseTexture", pMeshContainer->ppTextures[pBoneComb[iAttrib].AttribId]);

				///////////////////////////////////////////////////////////////////////////
				// ボーン数。
				pEffect->SetInt("CurNumBones", pMeshContainer->NumInfl - 1);
				D3DXMATRIX viewRotInv;
				D3DXMatrixInverse(&viewRotInv, NULL, viewMatrix);
				viewRotInv.m[3][0] = 0.0f;
				viewRotInv.m[3][1] = 0.0f;
				viewRotInv.m[3][2] = 0.0f;
				viewRotInv.m[3][3] = 1.0f;
				D3DXMatrixTranspose(&viewRotInv, &viewRotInv);
				pEffect->SetMatrix("g_viewMatrixRotInv", &viewRotInv);
				pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
				pEffect->BeginPass(0);
				pEffect->CommitChanges();
				// draw the subset with the current world matrix palette and material state
				pMeshContainer->MeshData.pMesh->DrawSubset(iAttrib);
				pEffect->EndPass();
				pEffect->End();

			}
		}

		else
		{
						
			D3DXMATRIX mWorld;
			if (pFrame != NULL) {
				mWorld = pFrame->CombinedTransformationMatrix;
			}
			else {
				mWorld = *worldMatrix;
			}
			//シャドウレシーバーフラグを転送。
			pEffect->SetInt("g_isShadowReciever", IsRecieveShadow);
			pEffect->SetMatrix("g_worldMatrix", &mWorld);
			pEffect->SetMatrix("g_rotationMatrix", rotationMatrix);
			pEffect->Begin(0, D3DXFX_DONOTSAVESTATE);
			pEffect->BeginPass(0);

			

			for (DWORD i = 0; i < pMeshContainer->NumMaterials; i++) {
				
				pEffect->SetTexture("g_diffuseTexture", pMeshContainer->ppTextures[i]);
				pEffect->CommitChanges();
				pMeshContainer->MeshData.pMesh->DrawSubset(i);
			}
			pEffect->EndPass();
			pEffect->End();
		}
	}

	void DrawFrame(
		IDirect3DDevice9* pd3dDevice, 
		LPD3DXFRAME pFrame, 
		ID3DXEffect* pEffect,
		D3DXMATRIX* worldMatrix,
		D3DXMATRIX* rotationMatrix,
		D3DXMATRIX* viewMatrix, 
		D3DXMATRIX* projMatrix,
		Light*	light,
		bool	IsDrawShadowMap,
		bool	IsRecieveShadow,
		bool	IsWave,
		LPDIRECT3DTEXTURE9  NormalMap,
		bool   IsSky,
		LPDIRECT3DCUBETEXTURE9      CubeMap)
	{
		LPD3DXMESHCONTAINER pMeshContainer;

		pMeshContainer = pFrame->pMeshContainer;
		while (pMeshContainer != NULL)
		{
			DrawMeshContainer(
				pd3dDevice, 
				pMeshContainer, 
				pFrame, 
				pEffect,
				worldMatrix,
				rotationMatrix,
				viewMatrix,
				projMatrix,
				light,
				IsDrawShadowMap,
				IsRecieveShadow,
				IsWave,
				NormalMap,
				IsSky,
				CubeMap

				);

			pMeshContainer = pMeshContainer->pNextMeshContainer;
		}

		if (pFrame->pFrameSibling != NULL)
		{
			DrawFrame(
				pd3dDevice, 
				pFrame->pFrameSibling, 
				pEffect,
				worldMatrix,
				rotationMatrix,
				viewMatrix,
				projMatrix,
				light,
				IsDrawShadowMap,
				IsRecieveShadow,
				IsWave,
				NormalMap,
				IsSky,
				CubeMap
				);
		}

		if (pFrame->pFrameFirstChild != NULL)
		{
			DrawFrame(
				pd3dDevice, 
				pFrame->pFrameFirstChild, 
				pEffect,
				worldMatrix,
				rotationMatrix,
				viewMatrix,
				projMatrix,
				light,
				IsDrawShadowMap,
				IsRecieveShadow,
				IsWave,
				NormalMap, IsSky,
				CubeMap
				);
		}
	}
}
SkinModel::SkinModel() :
	skinModelData(nullptr),
	light(nullptr),
	pEffect(nullptr)
{
}
SkinModel::~SkinModel()
{

}
void InitLight()
{
	memset(g_diffuseLightDirection, 0, sizeof(g_diffuseLightDirection));
	memset(g_diffuseLightColor, 0, sizeof(g_diffuseLightColor));
	g_diffuseLightDirection[0] = D3DXVECTOR4(-1.0f, -1.0f, 0.0f, 0.0f);
	D3DXVec3Normalize((D3DXVECTOR3*)&g_diffuseLightDirection[0], (D3DXVECTOR3*)&g_diffuseLightDirection[0]);

	//ディフューズライト。
	g_diffuseLightColor[0] = D3DXVECTOR4(0.0f, 0.5f, 0.5f, 1.0f);

	//環境光。
	g_ambientLight = D3DXVECTOR4(0.1f, 0.1f, 0.1f, 1.0f);
}
void SkinModel::Init(SkinModelData* modelData)
{
	pEffect = g_effectManager->LoadEffect("Assets/Shader/Model.fx");
	skinModelData = modelData;
	InitLight();
}
void SkinModel::UpdateWorldMatrix(const D3DXVECTOR3& trans, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale)
{
	//g_pos = game->GetCamera()->GetEyePt();
	D3DXMATRIX mTrans, mScale;
	D3DXMatrixScaling(&mScale, scale.x, scale.y, scale.z);
	D3DXMatrixTranslation(&mTrans, trans.x, trans.y, trans.z);
	D3DXMatrixRotationQuaternion(&rotationMatrix, &rot);

	worldMatrix = mScale * rotationMatrix * mTrans;

	if (skinModelData) {
		skinModelData->UpdateBoneMatrix(worldMatrix);	//ボーン行列を更新。
	}
}

void SkinModel::Draw(D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix)
{
	
	if (IsWave)
	{

		g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	}
	else
	{
		g_pd3dDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	}

	if (skinModelData) {
		DrawFrame(
			g_pd3dDevice, 
			skinModelData->GetFrameRoot(), 
			pEffect,
			&worldMatrix,
			&rotationMatrix,
			viewMatrix,
			projMatrix,
			light,
			IsDrawShadowMap,
			IsRecieveShadow,
			IsWave,
			NormalMap,
			IsSky,
			CubeMap
		);
	}
}
LPD3DXMESH SkinModel::GetOrgMeshFirst()
{
	if(skinModelData != nullptr){
		return skinModelData->GetOrgMeshFirst();
	}
	return nullptr;
}