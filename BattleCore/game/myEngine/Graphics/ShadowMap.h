#pragma once
#include "RenderTarget.h"

class CShadowMap
{
public:
	CShadowMap();
	~CShadowMap();
	//�e�N�X�`�����擾�B
	LPDIRECT3DTEXTURE9 GetTexture()
	{
		return SrenderTarget[0].GetTexture();
	}
	//�������B
	void Init();
	//���C�g�r���[�̎��_��ݒ�B
	void SetLightViewPosition(const D3DXVECTOR3& lightViewPosition)
	{
		viewPosition = lightViewPosition;
	}
	//���C�g�r���[�̒����_��ݒ�B
	void SetLightViewTarget(const D3DXVECTOR3& lightViewTarget)
	{
		viewTarget = lightViewTarget;
	}
	//���C�g�r���[�s����擾�B
	const D3DXMATRIX& GetLightViewMatrix()
	{
		return lightViewMatrix;
	}
	//���C�g�v���W�F�N�V�����s����擾�B
	const D3DXMATRIX& GetLightProjectionMatrix()
	{
		return lightProjMatrix;
	}
	

	//�X�V�B
	void Update();
	//�V���h�E�}�b�v�ɏ������݁B
	void Draw();
private:
	static const int			NUM_SHADOW_MAP = 3;
	CRenderTarget SrenderTarget[NUM_SHADOW_MAP];		//�V���h�E�}�b�v���������ރ����_�����O�^�[�Q�b�g�B
	D3DXMATRIX  lightViewMatrix;	//���C�g�r���[�}�g���N�X�B
	D3DXMATRIX	lightProjMatrix;	//���C�g�v���W�F�N�V�����}�g���N�X�B
	D3DXVECTOR3 viewPosition;		//���C�g�r���[�̎��_�B
	D3DXVECTOR3 viewTarget;			//���C�g�r���[�̒����_�B
	D3DXVECTOR3 m_lightDirection;	//!<���C�g�̌���
	Camera      Scamera;            //�J����

	float        Movelight = 0.0f;       //���C�g�̎��_���ړ�������
	float					Far;				//!<�����ʁB
	float					Near;				//!<�ߕ��ʁB
	float			        m_viewAngle;		//!<��p(���W�A��)�B
	float					aspect;				//!<�A�X�y�N�g��
};

