/*!
*@brief	�X�L�����f��
*/
#pragma once

#include "myEngine/Graphics/Animation.h"


class SkinModelData;
class RenderContext;
class Light;


//�t�H�O�B
enum EFogFunc {
	enFogFuncNone,		//�t�H�O�Ȃ��B
	enFogFuncDist,		//�����t�H�O�B
	enFogFuncHeight,	//�����t�H�O�B
	enFogFuncNum,
};
/*!
*@brief	�X�L�����f��
*/
class SkinModel {
public:
	SkinModel();
	~SkinModel();
	/*!
	*@brief	������
	*/
	void Init(SkinModelData* modelData);
	/*!
	*@brief	�`��B
	*/
	void Draw(D3DXMATRIX* viewMatrix, D3DXMATRIX* projMatrix);
	
	/*!
	*@brief	���[���h�s����X�V�B
	*@details
	* ���CMotion�N���X�Ɉړ������܂��B
	*@param[in]		trans	���s�ړ��B
	*@param[in]		rot		��]�B
	*@param[in]		scale	�g��B
	*/
	void UpdateWorldMatrix( const D3DXVECTOR3& trans, const D3DXQUATERNION& rot, const D3DXVECTOR3& scale );
	/*!
	*@brief	���C�g��ݒ�B
	*/
	void SetLight(Light* light)
	{
		this->light = light;
	}
	D3DXMATRIX GetMatrix()
	{
		return worldMatrix;
	}

	D3DXMATRIX GetWorldMatrix()
	{
		return worldMatrix;
	}
	/*!
	* @brief	�t�H�O�p�����[�^��ݒ�B
	*@param[in]	fogFunc		�t�H�O�̎�ށBEFogFunc���Q�ƁB
	*@param[in]	param0		�t�H�O�p�����[�^0
	* fogFunc��enFogFuncDist�̏ꍇ�̓t�H�O���|����n�߂鋗���AfogFunc��enFogFuncHeight�̏ꍇ�̓t�H�O���|����n�߂鍂���B
	*@param[in]	param1		�t�H�O�p�����[�^�P
	* fogFunc��enFogFuncDist�̏ꍇ�̓t�H�O���|����؂鋗���AfogFunc��enFogFuncHeight�̏ꍇ�̓t�H�O���|����؂鍂���B
	*/
	void SetFogParam(EFogFunc fogFunc, float param0, float param1)
	{
		m_fogFunc = fogFunc;
		m_fogParam[0] = param0;
		m_fogParam[1] = param1;
	}
	//�V���h�E�}�b�v�Z�b�g
	void SetShadowMap(bool ssm)
	{
		IsDrawShadowMap = ssm;
	}
	//�V���h�E���V�[�u�Z�b�g
	void SetShadowRecieve(bool ssr)
	{
		IsRecieveShadow = ssr;
	}
	void SetWave(bool Wave)
	{
		IsWave = Wave;
	}
	
	LPDIRECT3DTEXTURE9 GetWaveTex()
	{
		return WaveTex;
	}
	void SetWaveTexture()
	{
		//D3DXIMAGE_INFO imgInfo;
	     HRESULT hr = D3DXCreateTextureFromFileA(
			g_pd3dDevice,
			"Assets/modelData/NormalMap4.png",
			&WaveTex
		 );	//�e�N�X�`���Ǎ�
		 this->NormalMap = WaveTex;
	}
	void SetSkyTexture(LPDIRECT3DCUBETEXTURE9 cube)
	{
		this->CubeMap = cube;
		this->IsSky = true;

	}

	void SetTexture(LPDIRECT3DTEXTURE9 settex)
	{
		this->NormalMap = settex;
	}


	/*!
	*@brief	�I���W�i�����b�V���̐擪���擾����B
	*/
	LPD3DXMESH GetOrgMeshFirst();
private:
	
	D3DXMATRIX			worldMatrix;				//!<���[���h�s��B
	D3DXMATRIX			rotationMatrix;				//!<��]�s��B
	SkinModelData*		skinModelData = nullptr;	//!<�X�L�����f���f�[�^�B
	ID3DXEffect*		pEffect = nullptr;			//!<�G�t�F�N�g�B
	Animation			animation;					//!<�A�j���[�V�����B
	Light*				light = nullptr;			//!<���C�g�B
	EFogFunc						m_fogFunc;							//!<�t�H�O�̎�ށB0�Ȃ�t�H�O�Ȃ��A1�Ȃ狗���t�H�O�A2�Ȃ獂���t�H�O�B
	float							m_fogParam[2];						//!<�t�H�O�̃p�����[�^

	//�e�p�̔���
	bool				IsDrawShadowMap = false; 
	bool                IsRecieveShadow = false;
	//���ʂ̔���
	bool                IsWave = false;
	LPDIRECT3DTEXTURE9      NormalMap = NULL;
	LPDIRECT3DTEXTURE9      WaveTex = NULL;

	//��p�̔���
	bool                 IsSky = false;
	LPDIRECT3DCUBETEXTURE9      CubeMap = NULL;
	
};
